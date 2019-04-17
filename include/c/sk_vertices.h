#ifndef sk_vertices_DEFINED
#define sk_vertices_DEFINED

#include "sk_types.h"

SK_C_PLUS_PLUS_BEGIN_GUARD

/**
    Returns a new  set of vertex data used to paint on the canvas.

    @param cvertices            Array of (x,y) vertex coordinates.

    @param vertexCount          Number of vertices (and of texture coordinates).
                                `cvertices` contains `vertexCount*2` elements.

    @param textureCoordinates   Array of (u,v) texture coordinates.

    @param indices              If not NULL, specify the triangles indices associated
                                with this object.
*/
SK_API sk_vertices_t* sk_vertices_new(const float cvertices[], int vertexCount,
                               const float textureCoordinates[],
                               const int indices[], int indexCount);

/**
    Decrement reference count/releases the `sk_vertices_t*` resources.
*/
SK_API void sk_vertices_unref(sk_vertices_t* cvertices);

SK_C_PLUS_PLUS_END_GUARD

#endif
