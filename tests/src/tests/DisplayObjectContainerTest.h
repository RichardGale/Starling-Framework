#if !defined(__TESTS_SRC_TESTS_DISPLAYOBJECTCONTAINERTEST_AS)
#define __TESTS_SRC_TESTS_DISPLAYOBJECTCONTAINERTEST_AS
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
        class DisplayObject;
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
    namespace events
    {
        class Event;
    }
}

using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::events;

namespace tests
{
    class DisplayObjectContainerTest
    {
    private:
        static const float E;

    private:
        int mAdded;
    private:
        int mAddedToStage;
    private:
        int mAddedChild;
    private:
        int mRemoved;
    private:
        int mRemovedFromStage;
    private:
        int mRemovedChild;


    public:
        void
        ////[Before]    setUp();


    public:
        void                     ////[After]tearDown();


    public:
        void
        ////[Test]      testChildParentHandling();


    public:
        void
        ////[Test]      testRemoveChildren();


    public:
        void
        ////[Test]      testGetChildByName();


    public:
        void
        ////[Test]      testSetChildIndex();


    public:
        void
        ////[Test]      testSwapChildren();


    public:
        void
        ////[Test]      testWidthAndHeight();


    public:
        void
        ////[Test]      testBounds();


    public:
        void
        ////[Test]      testBoundsInSpace();


    public:
        void
        ////[Test]      testBoundsOfEmptyContainer();


    public:
        void
        ////[Test]      testSize();


    public:
        void
        ////[Test]      testSort();


    public:
        void
        ////[Test]      testAddExistingChild();


    public:
        void
        ////[Test]      testRemoveWithEventHandler();


    public:
        void
        ////[Test(expects="ArgumentError")]testIllegalRecursion();


    public:
        void
        ////[Test(expects="ArgumentError")]testAddAsChildToSelf();


    public:
        void
        ////[Test]      testDisplayListEvents();


    public:
        void
        ////[Test]      testRemovedFromStage();

    private:
        void     onAdded(Event *event);
    private:
        void     onAddedToStage(Event *event);
    private:
        void     onAddedChild(Event *event);

    private:
        void     onRemoved(Event *event);
    private:
        void     onRemovedFromStage(Event *event);
    private:
        void     onRemovedChild(Event *event);
    };
}

#endif // __TESTS_SRC_TESTS_DISPLAYOBJECTCONTAINERTEST_AS
#endif // __cplusplus

