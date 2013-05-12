#if !defined(__TESTS_SRC_TESTS_TWEENTEST_AS)
#define __TESTS_SRC_TESTS_TWEENTEST_AS
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
    namespace flexunit
    {
        namespace asserts
        {
            class assertFalse;
        }
    }
}
namespace org
{
    namespace flexunit
    {
        namespace asserts
        {
            class assertTrue;
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
    namespace animation
    {
        class Juggler;
    }
}
namespace starling
{
    namespace animation
    {
        class Transitions;
    }
}
namespace starling
{
    namespace animation
    {
        class Tween;
    }
}
namespace starling
{
    namespace display
    {
        class Quad;
    }
}

using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::animation;
using namespace starling::animation;
using namespace starling::animation;
using namespace starling::display;

namespace tests
{
    class TweenTest
    {
    private:
        const float E;


    public:
        void
        ////[Test]      testBasicTween();


    public:
        void
        ////[Test]      testSequentialTweens();


    public:
        void
        ////[Test]      testTweenFromZero();


    public:
        void
        ////[Test]      testResetTween();


    public:
        void
        ////[Test]      testResetTweenInOnComplete();


    public:
        void
        ////[Test]      testShortTween();


    public:
        void
        ////[Test]      testZeroTween();


    public:
        void
        ////[Test]      testCustomTween();


    public:
        void
        ////[Test]      testRepeatedTween();


    public:
        void
        ////[Test]      testReverseTween();


    public:
        void
        ////[Test]      testInfiniteTween();


    public:
        void
        ////[Test]      testGetEndValue();


    public:
        void
        ////[Test]      testProgress();

    private:
        void     executeTween(float time, float advanceTime);
    };
}

#endif // __TESTS_SRC_TESTS_TWEENTEST_AS
#endif // __cplusplus

