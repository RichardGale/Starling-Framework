#if !defined(__TESTS_SRC_TESTS_HELPERS_AS)
#define __TESTS_SRC_TESTS_HELPERS_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "flex11.6.h"
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
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}
namespace flash
{
    namespace geom
    {
        class Vector3D;
    }
}

namespace org
{
    namespace flexunit
    {
        class Assert;
    }
}
namespace org
{
    namespace flexunit
    {
        class assertThat;
    }
}
namespace org
{
    namespace flexunit
    {
        namespace asserts
        {
            class assertEquals;
        }
    }
}
namespace org
{
    namespace hamcrest
    {
        namespace number
        {
            class closeTo;
        }
    }
}

using namespace flash::geom;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;

namespace tests
{
    class Helpers
    {
    public:
        static void     compareRectangles(Rectangle *rect1, Rectangle *rect2,
                                          float e=0.0001);

    public:
        static void     comparePoints(Point *point1, Point *point2, float e=0.0001);

    public:
        static void     compareVector3Ds(Vector3D *v1, Vector3D *v2, float e=0.0001);

    public:
        static void     compareVectors(std::vector<float> vector1, std::vector<float> vector2,
                                       float e=0.0001);

    public:
        static void     compareMatrices(Matrix *matrix1, Matrix *matrix2, float e=0.0001);

    public:
        static void     assertDoesNotThrow(Function *block);
    };
}

#endif // __TESTS_SRC_TESTS_HELPERS_AS
#endif // __cplusplus

