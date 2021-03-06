/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/ops/GrMeshDrawOp.h"

#include "src/gpu/GrOpFlushState.h"
#include "src/gpu/GrOpsRenderPass.h"
#include "src/gpu/GrResourceProvider.h"

GrMeshDrawOp::GrMeshDrawOp(uint32_t classID) : INHERITED(classID) {}

void GrMeshDrawOp::onPrepare(GrOpFlushState* state) { this->onPrepareDraws(state); }

//////////////////////////////////////////////////////////////////////////////

GrMeshDrawOp::PatternHelper::PatternHelper(Target* target, GrPrimitiveType primitiveType,
                                           size_t vertexStride, sk_sp<const GrBuffer> indexBuffer,
                                           int verticesPerRepetition, int indicesPerRepetition,
                                           int repeatCount, int maxRepetitions) {
    this->init(target, primitiveType, vertexStride, std::move(indexBuffer), verticesPerRepetition,
               indicesPerRepetition, repeatCount, maxRepetitions);
}

void GrMeshDrawOp::PatternHelper::init(Target* target, GrPrimitiveType primitiveType,
                                       size_t vertexStride, sk_sp<const GrBuffer> indexBuffer,
                                       int verticesPerRepetition, int indicesPerRepetition,
                                       int repeatCount, int maxRepetitions) {
    SkASSERT(target);
    if (!indexBuffer) {
        return;
    }
    sk_sp<const GrBuffer> vertexBuffer;
    int firstVertex;
    int vertexCount = verticesPerRepetition * repeatCount;
    fVertices = target->makeVertexSpace(vertexStride, vertexCount, &vertexBuffer, &firstVertex);
    if (!fVertices) {
        SkDebugf("Vertices could not be allocated for patterned rendering.");
        return;
    }
    SkASSERT(vertexBuffer);
    fMesh = target->allocMesh(primitiveType);

    SkASSERT(maxRepetitions ==
             static_cast<int>(indexBuffer->size() / (sizeof(uint16_t) * indicesPerRepetition)));
    fMesh->setIndexedPatterned(std::move(indexBuffer), indicesPerRepetition, verticesPerRepetition,
                               repeatCount, maxRepetitions);
    fMesh->setVertexData(std::move(vertexBuffer), firstVertex);
}

void GrMeshDrawOp::PatternHelper::recordDraw(
        Target* target, sk_sp<const GrGeometryProcessor> gp) const {
    target->recordDraw(std::move(gp), fMesh);
}

void GrMeshDrawOp::PatternHelper::recordDraw(
        Target* target, sk_sp<const GrGeometryProcessor> gp,
        const GrPipeline::FixedDynamicState* fixedDynamicState) const {
    target->recordDraw(std::move(gp), fMesh, 1, fixedDynamicState, nullptr);
}

//////////////////////////////////////////////////////////////////////////////

GrMeshDrawOp::QuadHelper::QuadHelper(Target* target, size_t vertexStride, int quadsToDraw) {
    sk_sp<const GrGpuBuffer> indexBuffer = target->resourceProvider()->refNonAAQuadIndexBuffer();
    if (!indexBuffer) {
        SkDebugf("Could not get quad index buffer.");
        return;
    }
    this->init(target, GrPrimitiveType::kTriangles, vertexStride, std::move(indexBuffer),
               GrResourceProvider::NumVertsPerNonAAQuad(),
               GrResourceProvider::NumIndicesPerNonAAQuad(), quadsToDraw,
               GrResourceProvider::MaxNumNonAAQuads());
}

//////////////////////////////////////////////////////////////////////////////

GrPipeline::DynamicStateArrays* GrMeshDrawOp::Target::AllocDynamicStateArrays(
        SkArenaAlloc* arena, int numMeshes, int numPrimitiveProcessorTextures,
        bool allocScissors) {

    auto result = arena->make<GrPipeline::DynamicStateArrays>();

    if (allocScissors) {
        result->fScissorRects = arena->makeArray<SkIRect>(numMeshes);
    }

    if (numPrimitiveProcessorTextures) {
        result->fPrimitiveProcessorTextures = arena->makeArrayDefault<GrTextureProxy*>(
                        numPrimitiveProcessorTextures * numMeshes);
    }

    return result;
}

GrPipeline::FixedDynamicState* GrMeshDrawOp::Target::MakeFixedDynamicState(
        SkArenaAlloc* arena, const GrAppliedClip* clip, int numPrimProcTextures) {

    if ((clip && clip->scissorState().enabled()) || numPrimProcTextures) {
        const SkIRect& scissor = (clip) ? clip->scissorState().rect() : SkIRect::MakeEmpty();

        auto result = arena->make<GrPipeline::FixedDynamicState>(scissor);

        if (numPrimProcTextures) {
            result->fPrimitiveProcessorTextures = arena->makeArrayDefault<GrTextureProxy*>(
                        numPrimProcTextures);
        }
        return result;
    }
    return nullptr;
}
