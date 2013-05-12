#if !defined(__TESTS_SRC_TESTS_VERTEXDATATEST_AS)
#define __TESTS_SRC_TESTS_VERTEXDATATEST_AS
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
        class Point;
    }
}
namespace flash
{
    namespace geom
    {
        class Vector3D;
    }
}

namespace flexunit
{
    namespace framework
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
    namespace hamcrest
    {
        namespace number
        {
            class closeTo;
        }
    }
}

namespace starling
{
    namespace utils
    {
        class VertexData;
    }
}

using namespace flash::geom;
using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::hamcrest::number;
using namespace starling::utils;

namespace tests
{
    class VertexDataTest
    {
    private:
        static const float E;


    public:
        void
        ////[Test]      testInit();


    public:
        void
        ////[Test]      testGetNumVertices();


    public:
        void
        ////[Test(expects="RangeError")]testBoundsLow();


    public:
        void
        ////[Test(expects="RangeError")]testBoundsHigh();


    public:
        void
        ////[Test]      testPosition();


    public:
        void
        ////[Test]      testColor();


    public:
        void
        ////[Test]      testTexCoords();
    };
}

#endif // __TESTS_SRC_TESTS_VERTEXDATATEST_AS
#endif // __cplusplus

