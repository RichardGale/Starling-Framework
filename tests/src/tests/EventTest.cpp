// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "EventTest.h"
#include "flexunit/framework/Assert.h"

#include "starling/display/Sprite.h"
#include "starling/events/Event.h"
#include "starling/events/EventDispatcher.h"

using namespace flexunit::framework;
using namespace starling::display;
using namespace starling::events;

namespace tests {



        void EventTest::                testBubbling()
        {
            const std::string eventType="test";

             Sprite* grandParent= new Sprite();
             Sprite* parent= new Sprite();
             Sprite* child= new Sprite();

            grandParent->addChild(parent);
            parent->addChild(child);

             bool grandParentEventHandlerHit    = false;
             bool parentEventHandlerHit    = false;
             bool childEventHandlerHit    = false;
             int hitCount = 0;

            // bubble up

            grandParent->addEventListener(eventType, onGrandParentEvent);
            parent->addEventListener(eventType, onParentEvent);
            child->addEventListener(eventType, onChildEvent);

             Event* event= new Event(eventType, true);
            child->dispatchEvent(event);

            Assert::assertTrue(grandParentEventHandlerHit);
            Assert::assertTrue(parentEventHandlerHit);
            Assert::assertTrue(childEventHandlerHit);

            Assert::assertEquals(3, hitCount);

            // remove event handler

            parentEventHandlerHit = false;
            parent->removeEventListener(eventType, onParentEvent);
            child->dispatchEvent(event);

            Assert::assertFalse(parentEventHandlerHit);
            Assert::assertEquals(5, hitCount);

            // don't bubble

            event = new Event(eventType);

            grandParentEventHandlerHit = parentEventHandlerHit = childEventHandlerHit = false;
            parent->addEventListener(eventType, onParentEvent);
            child->dispatchEvent(event);

            Assert::assertEquals(6, hitCount);
            Assert::assertTrue(childEventHandlerHit);
            Assert::assertFalse(parentEventHandlerHit);
            Assert::assertFalse(grandParentEventHandlerHit);void EventTest::(Event* event)
            {
                grandParentEventHandlerHit = true;
                Assert::assertEquals(child, event->target());
                Assert::assertEquals(grandParent, event->currentTarget());
                hitCount++;
            }void EventTest::(Event* event)
            {
                parentEventHandlerHit = true;
                Assert::assertEquals(child, event->target());
                Assert::assertEquals(parent, event->currentTarget());
                hitCount++;
            }void EventTest::(Event* event)
            {
                childEventHandlerHit = true;
                Assert::assertEquals(child, event->target());
                Assert::assertEquals(child, event->currentTarget());
                hitCount++;
            }
        }


        void EventTest::                testStopPropagation()
        {
            const std::string eventType="test";

             Sprite* grandParent= new Sprite();
             Sprite* parent= new Sprite();
             Sprite* child= new Sprite();

            grandParent->addChild(parent);
            parent->addChild(child);

             int hitCount = 0;

            // stop propagation at parent

            child->addEventListener(eventType, onEvent);
            parent->addEventListener(eventType, onEvent_StopPropagation);
            parent->addEventListener(eventType, onEvent);
            grandParent->addEventListener(eventType, onEvent);

            child->dispatchEvent(new Event(eventType, true));

            Assert::assertEquals(3, hitCount);

            // stop immediate propagation at parent

            parent->removeEventListener(eventType, onEvent_StopPropagation);
            parent->removeEventListener(eventType, onEvent);

            parent->addEventListener(eventType, onEvent_StopImmediatePropagation);
            parent->addEventListener(eventType, onEvent);

            child->dispatchEvent(new Event(eventType, true));

            Assert::assertEquals(5, hitCount);void EventTest::(Event* event)
            {
                hitCount++;
            }void EventTest::(Event* event)
            {
                event->stopPropagation();
                hitCount++;
            }void EventTest::(Event* event)
            {
                event->stopImmediatePropagation();
                hitCount++;
            }
        }


        void EventTest::                testRemoveEventListeners()
        {
             int hitCount = 0;
             EventDispatcher* dispatcher= new EventDispatcher();

            dispatcher->addEventListener("Type1", onEvent);
            dispatcher->addEventListener("Type2", onEvent);
            dispatcher->addEventListener("Type3", onEvent);

            hitCount = 0;
            dispatcher->dispatchEvent(new Event("Type1"));
            Assert::assertEquals(1, hitCount);

            dispatcher->dispatchEvent(new Event("Type2"));
            Assert::assertEquals(2, hitCount);

            dispatcher->dispatchEvent(new Event("Type3"));
            Assert::assertEquals(3, hitCount);

            hitCount = 0;
            dispatcher->removeEventListener("Type1", onEvent);
            dispatcher->dispatchEvent(new Event("Type1"));
            Assert::assertEquals(0, hitCount);

            dispatcher->dispatchEvent(new Event("Type3"));
            Assert::assertEquals(1, hitCount);

            hitCount = 0;
            dispatcher->removeEventListeners();
            dispatcher->dispatchEvent(new Event("Type1"));
            dispatcher->dispatchEvent(new Event("Type2"));
            dispatcher->dispatchEvent(new Event("Type3"));
            Assert::assertEquals(0, hitCount);void EventTest::(Event* event)
            {
                ++hitCount;
            }
        }


        void EventTest::                testBlankEventDispatcher()
        {
             EventDispatcher* dispatcher= new EventDispatcher();

            Helpers()->assertDoesNotThrow(function()void
            {
                dispatcher->removeEventListener("Test", NULL);
            });

            Helpers()->assertDoesNotThrow(function()void
            {
                dispatcher->removeEventListeners("Test");
            });
        }


        void EventTest::                testDuplicateEventHandler()
        {
             EventDispatcher* dispatcher= new EventDispatcher();
             int callCount = 0;

            dispatcher->addEventListener("test", onEvent);
            dispatcher->addEventListener("test", onEvent);

            dispatcher->dispatchEvent(new Event("test"));
            Assert::assertEquals(1, callCount);void EventTest::(Event* event)
            {
                callCount++;
            }
        }


        void EventTest::                testBubbleWithModifiedChain()
        {
            const std::string eventType="test";

             Sprite* grandParent= new Sprite();
             Sprite* parent= new Sprite();
             Sprite* child= new Sprite();

            grandParent->addChild(parent);
            parent->addChild(child);

             int hitCount = 0;

            // listener on 'child' changes display list; bubbling must not be affected.

            grandParent->addEventListener(eventType, onEvent);
            parent->addEventListener(eventType, onEvent);
            child->addEventListener(eventType, onEvent);
            child->addEventListener(eventType, onEvent_removeFromParent);

            child->dispatchEvent(new Event(eventType, true));

            Assert::assertNull(parent->parent);
            Assert::assertEquals(3, hitCount);void EventTest::()
            {
                hitCount++;
            }void EventTest::()
            {
                parent->removeFromParent();
            }
        }


        void EventTest::                testRedispatch()
        {
            const std::string eventType="test";

             Sprite* grandParent= new Sprite();
             Sprite* parent= new Sprite();
             Sprite* child= new Sprite();

            grandParent->addChild(parent);
            parent->addChild(child);

            grandParent->addEventListener(eventType, onEvent);
            parent->addEventListener(eventType, onEvent);
            child->addEventListener(eventType, onEvent);
            parent->addEventListener(eventType, onEvent_redispatch);

             std::vector<void*> targets ;
             std::vector<void*> currentTargets ;

            child->dispatchEventWith(eventType, true);

            // main bubble
            Assert::assertEquals(targets[0], child);
            Assert::assertEquals(currentTargets[0], child);

            // main bubble
            Assert::assertEquals(targets[1], child);
            Assert::assertEquals(currentTargets[1], parent);

            // inner bubble
            Assert::assertEquals(targets[2], parent);
            Assert::assertEquals(currentTargets[2], parent);

            // inner bubble
            Assert::assertEquals(targets[3], parent);
            Assert::assertEquals(currentTargets[3], grandParent);

            // main bubble
            Assert::assertEquals(targets[4], child);
            Assert::assertEquals(currentTargets[4], grandParent);void EventTest::(Event* event)
            {
                targets.push(event->target());
                currentTargets.push(event->currentTarget());
            }void EventTest::(Event* event)
            {
                parent->removeEventListener(eventType, onEvent_redispatch);
                parent->dispatchEvent(event);
            }
        }
}

