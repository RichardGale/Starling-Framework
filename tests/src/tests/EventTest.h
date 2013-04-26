#if !defined(__TESTS_SRC_TESTS_EVENTTEST_AS)
#define __TESTS_SRC_TESTS_EVENTTEST_AS
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



namespace flexunit
{
    namespace framework
    {
        class Assert;
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
    namespace events
    {
        class Event;
    }
}
namespace starling
{
    namespace events
    {
        class EventDispatcher;
    }
}

using namespace flexunit::framework;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;

namespace tests
{
    class EventTest
    {

    public:
        void
        ////[Test]      testBubbling();


    public:
        void
        ////[Test]      testStopPropagation();


    public:
        void
        ////[Test]      testRemoveEventListeners();


    public:
        void
        ////[Test]      testBlankEventDispatcher();


    public:
        void
        ////[Test]      testDuplicateEventHandler();


    public:
        void
        ////[Test]      testBubbleWithModifiedChain();


    public:
        void
        ////[Test]      testRedispatch();
    };
}

#endif // __TESTS_SRC_TESTS_EVENTTEST_AS
#endif // __cplusplus

