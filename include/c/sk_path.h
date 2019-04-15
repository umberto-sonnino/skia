/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

// EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL
// DO NOT USE -- FOR INTERNAL TESTING ONLY

#ifndef sk_path_DEFINED
#define sk_path_DEFINED

#include "sk_types.h"

SK_C_PLUS_PLUS_BEGIN_GUARD

typedef enum {
    CW_SK_PATH_DIRECTION,
    CCW_SK_PATH_DIRECTION,
} sk_path_direction_t;

/** Create a new, empty path. */
SK_API sk_path_t* sk_path_new(void);
/** Release the memory used by a sk_path_t. */
SK_API void sk_path_delete(sk_path_t*);

/** Set the beginning of the next contour to the point (x,y). */
SK_API void sk_path_move_to(sk_path_t*, float x, float y);
/**
    Add a line from the last point to the specified point (x,y). If no
    sk_path_move_to() call has been made for this contour, the first
    point is automatically set to (0,0).
*/
SK_API void sk_path_line_to(sk_path_t*, float x, float y);
/**
    Add a quadratic bezier from the last point, approaching control
    point (x0,y0), and ending at (x1,y1). If no sk_path_move_to() call
    has been made for this contour, the first point is automatically
    set to (0,0).
*/
SK_API void sk_path_quad_to(sk_path_t*, float x0, float y0, float x1, float y1);
/**
    Add a conic curve from the last point, approaching control point
    (x0,y01), and ending at (x1,y1) with weight w.  If no
    sk_path_move_to() call has been made for this contour, the first
    point is automatically set to (0,0).
*/
SK_API void sk_path_conic_to(sk_path_t*, float x0, float y0, float x1, float y1, float w);
/**
    Add a cubic bezier from the last point, approaching control points
    (x0,y0) and (x1,y1), and ending at (x2,y2). If no
    sk_path_move_to() call has been made for this contour, the first
    point is automatically set to (0,0).
*/
SK_API void sk_path_cubic_to(sk_path_t*,
                             float x0, float y0,
                             float x1, float y1,
                             float x2, float y2);
/**
   Adds to `cpath` a new sub-path that consists of `csubpath` offset by
   `x`,`y`.
   If `sk_matrix_t` is specified the path will be transformed by this matrix
   after the matrix is translated by the given offset. The matrix is a 4x4
   matrix stored in column major order.
*/
SK_API void sk_path_add_path(sk_path_t* cpath, const sk_path_t* csubpath, float x, float y);

SK_API void sk_path_add_path_with_matrix(sk_path_t* cpath, const sk_path_t* csubpath, 
                                         float x, float y, sk_matrix_t*);

/**
   Close the current contour. If the current point is not equal to the
   first point of the contour, a line segment is automatically added.
*/
SK_API void sk_path_close(sk_path_t*);

/**
    Clears the sk_path_t object of all sub-paths, returning it to the
    same state it had when it was created. The current point is
    reset to the origin.
*/
SK_API void sk_path_reset(sk_path_t* cpath);

/**
    Sets the sk_path_t fill type as FillType::kEvenOdd_FillType or 
    FillType::kWinding_FillType depending on the value of `isEvenOdd`
*/
void sk_path_set_evenodd(sk_path_t* cpath, bool isEvenOdd);

/**
    Add a closed rectangle contour to the path.
*/
SK_API void sk_path_add_rect(sk_path_t*, const sk_rect_t*, sk_path_direction_t);
/**
    Add a closed oval contour to the path
*/
SK_API void sk_path_add_oval(sk_path_t*, const sk_rect_t*, sk_path_direction_t);

/**
 *  If the path is empty, return false and set the rect parameter to [0, 0, 0, 0].
 *  else return true and set the rect parameter to the bounds of the control-points
 *  of the path.
 */
SK_API bool sk_path_get_bounds(const sk_path_t*, sk_rect_t*);

SK_C_PLUS_PLUS_END_GUARD

#endif
