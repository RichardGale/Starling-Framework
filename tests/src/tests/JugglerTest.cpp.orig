// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "JugglerTest.h"
#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/flexunit/asserts/assertFalse.h"
#include "org/flexunit/asserts/assertTrue.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/animation/Juggler.h"
#include "starling/animation/Tween.h"
#include "starling/display/Quad.h"
#include "starling/events/Event.h"

using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::animation;
using namespace starling::animation;
using namespace starling::display;
using namespace starling::events;

namespace tests {



        void JugglerTest::                testModificationWithinCallback()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);
             Tween* tween=new Tween(quad, 1.0);
             bool startReached   = false;
            juggler->add(tween);

            tween->onComplete= function()void
            {
                 Tween* otherTween=new Tween(quad, 1.0);
                otherTween->onStart= function()void
                {
                    startReached = true;
                };
                juggler->add(otherTween);
            };

            juggler->advanceTime(0.4);// -> 0.4 (start)
            juggler->advanceTime(0.4);// -> 0.8 (update)
            juggler->advanceTime(0.4);// -> 1.2 (complete)
            juggler->advanceTime(0.4);// -> 1.6 (start of new tween)

            Assert::assertTrue(startReached);
        }


        void JugglerTest::                testContains()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);
             Tween* tween=new Tween(quad, 1.0);

            Assert::assertFalse(juggler->contains(tween));
            juggler->add(tween);
            Assert::assertTrue(juggler->contains(tween));
        }


        void JugglerTest::                testPurge()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);

             Tween* tween1=new Tween(quad, 1.0);
             Tween* tween2=new Tween(quad, 2.0);

            juggler->add(tween1);
            juggler->add(tween2);

            tween1->animate("x",100);
            tween2->animate("y",100);

            Assert::assertTrue(tween1->hasEventListener(Event::REMOVE_FROM_JUGGLER));
            Assert::assertTrue(tween2->hasEventListener(Event::REMOVE_FROM_JUGGLER));

            juggler->purge();

            Assert::assertFalse(tween1->hasEventListener(Event::REMOVE_FROM_JUGGLER));
            Assert::assertFalse(tween2->hasEventListener(Event::REMOVE_FROM_JUGGLER));

            juggler->advanceTime(10);

            Assert::assertEquals(0,quad->x);
            Assert::assertEquals(0,quad->y);
        }


        void JugglerTest::                testPurgeFromAdvanceTime()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);

             Tween* tween1=new Tween(quad, 1.0);
             Tween* tween2=new Tween(quad, 1.0);
             Tween* tween3=new Tween(quad, 1.0);

            juggler->add(tween1);
            juggler->add(tween2);
            juggler->add(tween3);

            tween2->onUpdate= juggler->purge;

            // if this doesn't crash, we're fine =)
            juggler->advanceTime(0.5);
        }


        void JugglerTest::                testRemoveTweensWithTarget()
        {
             Juggler* juggler=new Juggler();

             Quad* quad1=new Quad(100, 100);
             Quad* quad2=new Quad(100, 100);

             Tween* tween1=new Tween(quad1, 1.0);
             Tween* tween2=new Tween(quad2, 1.0);

            tween1->animate("rotation",1.0);
            tween2->animate("rotation",1.0);

            juggler->add(tween1);
            juggler->add(tween2);

            juggler->removeTweens(quad1);
            juggler->advanceTime(1.0);

            assertThat(quad1->rotation,closeTo(0.0, E));
            assertThat(quad2->rotation,closeTo(1.0, E));
        }


        void JugglerTest::                testContainsTweens()
        {
             Juggler* juggler=new Juggler();
             Quad* quad1=new Quad(100, 100);
             Quad* quad2=new Quad(100, 100);
             Tween* tween=new Tween(quad1, 1.0);

            juggler->add(tween);

            assertTrue(juggler->containsTweens(quad1));
            assertFalse(juggler->containsTweens(quad2));
        }


        void JugglerTest::                testAddTwice()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);
             Tween* tween=new Tween(quad, 1.0);

            juggler->add(tween);
            juggler->add(tween);

            assertThat(tween->currentTime,closeTo(0.0, E));
            juggler->advanceTime(0.5);
            assertThat(tween->currentTime,closeTo(0.5, E));
        }


        void JugglerTest::                testModifyJugglerInCallback()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);

             Tween* tween1=new Tween(quad, 1.0);
            tween1->animate("x",100);

             Tween* tween2=new Tween(quad, 0.5);
            tween2->animate("y",100);

             Tween* tween3=new Tween(quad, 0.5);
            tween3->animate("scaleX",0.5);

            tween2->onComplete= function()void  {
                juggler->remove(tween1);
                juggler->add(tween3);
            };

            juggler->add(tween1);
            juggler->add(tween2);

            juggler->advanceTime(0.5);
            juggler->advanceTime(0.5);

            assertThat(quad->x,closeTo(50.0, E));
            assertThat(quad->y,closeTo(100.0, E));
            assertThat(quad->scaleX,closeTo(0.5, E));
        }


        void JugglerTest::                testModifyJugglerTwiceInCallback()
        {
            // https://github.com/PrimaryFeather/Starling-Framework/issues/155

             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);

             Tween* tween1=new Tween(quad, 1.0);
             Tween* tween2=new Tween(quad, 1.0);
            tween2->fadeTo(0);

            juggler->add(tween1);
            juggler->add(tween2);

            juggler->remove(tween1);// sets slot in array to null
            tween2->onUpdate= juggler->remove;
            tween2->onUpdateArgs= [tween2];

            juggler->advanceTime(0.5);
            juggler->advanceTime(0.5);

            assertThat(quad->alpha,closeTo(0.5, E));
        }


        void JugglerTest::                testTweenConvenienceMethod()
        {
             Juggler* juggler=new Juggler();
             Quad* quad=new Quad(100, 100);

             int completeCount= 0;
             int startCount= 0;

            juggler->tween(quad,1.0, {
                x: 100,
                onStart: onStart,
                onComplete: onComplete
            });

            juggler->advanceTime(0.5);
            assertEquals(1, startCount);
            assertEquals(0, completeCount);
            assertThat(quad->x,closeTo(50, E));

            juggler->advanceTime(0.5);
            assertEquals(1, startCount);
            assertEquals(1, completeCount);
            assertThat(quad->x,closeTo(100, E));

            void JugglerTest::()      { completeCount++; }
            void JugglerTest::()      { startCount++; }
        }
}

