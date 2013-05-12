#if !defined(__STARLING_SRC_STARLING_UTILS_TRANSFORMCOORDS_AS)
#define __STARLING_SRC_STARLING_UTILS_TRANSFORMCOORDS_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash
{
    namespace geom
    {
        class Matrix;
    }
}
namespace flash
{
    namespace geom
    {
        class Point;
    }
}

/** Uses a matrix to transform 2D coordinates into a different space. If you pass a
 *  'resultPoint', the result will be stored in this point instead of creating a new object.*/
public:
Point   *transformCoords(Matrix *matrix, float x, float y,
                         Point *resultPoint=NULL);


bool deprecationNotified;
#endif // __STARLING_SRC_STARLING_UTILS_TRANSFORMCOORDS_AS
#endif // __cplusplus

