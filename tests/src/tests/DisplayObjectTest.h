#if !defined(__TESTS_SRC_TESTS_DISPLAYOBJECTTEST_AS)
#define __TESTS_SRC_TESTS_DISPLAYOBJECTTEST_AS
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

namespace starling
{
    namespace display
    {
        class Quad;
    }
}
namespace starling
{
    namespace display
    {
        class Sprite;
    }
}
namespace starling
{
    namespace display
    {
        class Stage;
    }
}
namespace starling
{
    namespace utils
    {
        class deg2rad;
    }
}

using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::utils;

namespace tests
{
    class DisplayObjectTest
    {
    private:
        static const float E;


    public:
        void
        ////[Test]      testBase();


    public:
        void
        ////[Test]      testRootAndStage();


    public:
        void
        ////[Test]      testGetTransformationMatrix();


    public:
        void
        ////[Test]      testSetTransformationMatrix();


    public:
        void
        ////[Test]      testSetTransformationMatrixWithPivot();


    public:
        void
        ////[Test]      testBounds();


    public:
        void
        ////[Test]      testZeroSize();


    public:
        void
        ////[Test]      testLocalToGlobal();


    public:
        void
        ////[Test]      testGlobalToLocal();


    public:
        void
        ////[Test]      testHitTestPoint();


    public:
        void
        ////[Test]      testRotation();


    public:
        void
        ////[Test]      testPivotPoint();


    public:
        void
        ////[Test]      testPivotWithSkew();


    public:
        void
        ////[Test]      testName();
    };
}

#endif // __TESTS_SRC_TESTS_DISPLAYOBJECTTEST_AS
#endif // __cplusplus

