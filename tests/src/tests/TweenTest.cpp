// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TweenTest.h"
#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/flexunit/asserts/assertFalse.h"
#include "org/flexunit/asserts/assertTrue.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/animation/Juggler.h"
#include "starling/animation/Transitions.h"
#include "starling/animation/Tween.h"
#include "starling/display/Quad.h"

using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::animation;
using namespace starling::display;

namespace tests {


                      


        void TweenTest::                testBasicTween()
        {
            float startX = 10.0;
            float startY = 20.0;
            float endX = 100.0;
            float endY = 200.0;
            float startAlpha = 1.0;
            float endAlpha = 0.0;
            float totalTime = 2.0;

            int startCount = 0;
            int updateCount = 0;
            int completeCount = 0;

            Quad* quad = new Quad(100, 100);
            quad->x ( startX);
            quad->y ( startY);
            quad->alpha ( startAlpha);

            Tween* tween = new Tween(quad, totalTime, Transitions::LINEAR);
            tween->moveTo(endX, endY);
            tween->animate("alpha", endAlpha);
            tween->onStart    ( function()void  { startCount++; });
            tween->onUpdate   ( function()void  { updateCount++; });
            tween->onComplete ( function()void  { completeCount++; });

            tween->advanceTime(totalTime / 3.0);
            assertThat(quad->x(), closeTo(startX + (endX-startX)/3.0, E));
            assertThat(quad->y(), closeTo(startY + (endY-startY)/3.0, E));
            assertThat(quad->alpha(), closeTo(startAlpha + (endAlpha-startAlpha)/3.0, E));
            Assert::assertEquals(1, startCount);
            Assert::assertEquals(1, updateCount);
            Assert::assertEquals(0, completeCount);
            Assert::assertFalse(tween->isComplete());

            tween->advanceTime(totalTime / 3.0);
            assertThat(quad->x(), closeTo(startX + 2*(endX-startX)/3.0, E));
            assertThat(quad->y(), closeTo(startY + 2*(endY-startY)/3.0, E));
            assertThat(quad->alpha(), closeTo(startAlpha + 2*(endAlpha-startAlpha)/3.0, E));
            Assert::assertEquals(1, startCount);
            Assert::assertEquals(2, updateCount);
            Assert::assertEquals(0, completeCount);
            Assert::assertFalse(tween->isComplete());

            tween->advanceTime(totalTime / 3.0);
            assertThat(quad->x(), closeTo(endX, E));
            assertThat(quad->y(), closeTo(endY, E));
            assertThat(quad->alpha(), closeTo(endAlpha, E));
            Assert::assertEquals(1, startCount);
            Assert::assertEquals(3, updateCount);
            Assert::assertEquals(1, completeCount);
            Assert::assertTrue(tween->isComplete());
        }


        void TweenTest::                testSequentialTweens()
        {
            float startPos  = 0.0;
            float targetPos = 50.0;
            Quad* quad = new Quad(100, 100);

            // 2 tweens should move object up, then down
            Tween* tween1 = new Tween(quad, 1.0);
            tween1->animate("y", targetPos);

            Tween* tween2 = new Tween(quad, 1.0);
            tween2->animate("y", startPos);
            tween2->delay ( tween1->totalTime());

            tween1->advanceTime(1.0);
            assertThat(quad->y(), closeTo(targetPos, E));

            tween2->advanceTime(1.0);
            assertThat(quad->y(), closeTo(targetPos, E));

            tween2->advanceTime(0.5);
            assertThat(quad->y(), closeTo((targetPos - startPos)/2.0, E));

            tween2->advanceTime(0.5);
            assertThat(quad->y(), closeTo(startPos, E));
        }


        void TweenTest::                testTweenFromZero()
        {
            Quad* quad = new Quad(100, 100);
            quad->scaleX ( 0.0);

            Tween* tween = new Tween(quad, 1.0);
            tween->animate("scaleX", 1.0);

            tween->advanceTime(0.0);
            assertThat(quad->width(), closeTo(0.0, E));

            tween->advanceTime(0.5);
            assertThat(quad->width(), closeTo(50.0, E));

            tween->advanceTime(0.5);
            assertThat(quad->width(), closeTo(100.0, E));
        }


        void TweenTest::                testResetTween()
        {
            Quad* quad = new Quad(100, 100);

            Tween* tween = new Tween(quad, 1.0);
            tween->animate("x", 100);

            tween->advanceTime(0.5);
            assertThat(quad->x(), closeTo(50, E));

            tween->reset(this, 1.0);
            tween->advanceTime(0.5);

            // tween should no longer change quad.x
            assertThat(quad->x(), closeTo(50, E));
        }


        void TweenTest::                testResetTweenInOnComplete()
        {
            Quad* quad = new Quad(100, 100);
            Juggler* juggler = new Juggler();

            Tween* tween = new Tween(quad, 1.0);
            tween->animate("x", 100);
            tween->onComplete ( function()void
            {
                tween->reset(quad, 1.0);
                tween->animate("x", 0);
                juggler->add(tween);
            });

            juggler->add(tween);

            juggler->advanceTime(1.0);
            assertThat(quad->x(), closeTo(100, E));
            assertThat(tween->currentTime(), closeTo(0, E));

            juggler->advanceTime(1.0);
            assertThat(quad->x(), closeTo(0, E));
            assertTrue(tween->isComplete());
        }


        void TweenTest::                testShortTween()
        {
            executeTween(0.1, 0.1);
        }


        void TweenTest::                testZeroTween()
        {
            executeTween(0.0, 0.1);
        }


        void TweenTest::                testCustomTween()
        {
            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0, transition);
            tween->animate("x", 100);

            tween->advanceTime(0.1);
            assertThat(quad->x(), closeTo(10, E));

            tween->advanceTime(0.5);
            assertThat(quad->x(), closeTo(60, E));

            tween->advanceTime(0.4);
            assertThat(quad->x(), closeTo(100, E));

            assertEquals("custom", tween->transition());float TweenTest::(float ratio)
            {
                return ratio;
            }
        }


        void TweenTest::                testRepeatedTween()
        {
            int startCount = 0;
            int repeatCount = 0;
            int completeCount = 0;

            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0);
            tween->repeatCount ( 3);
            tween->onStart ( onStart);
            tween->onRepeat ( onRepeat);
            tween->onComplete ( onComplete);
            tween->animate("x", 100);

            tween->advanceTime(1.5);
            assertThat(quad->x(), closeTo(50, E));
            assertEquals(tween->repeatCount, 2);
            assertEquals(startCount, 1);
            assertEquals(repeatCount, 1);
            assertEquals(completeCount, 0);

            tween->advanceTime(0.75);
            assertThat(quad->x(), closeTo(25, E));
            assertEquals(tween->repeatCount, 1);
            assertEquals(startCount, 1);
            assertEquals(repeatCount, 2);
            assertEquals(completeCount, 0);
            assertFalse(tween->isComplete());

            tween->advanceTime(1.0);
            assertThat(quad->x(), closeTo(100, E));
            assertEquals(tween->repeatCount, 1);
            assertEquals(startCount, 1);
            assertEquals(repeatCount, 2);
            assertEquals(completeCount, 1);
            assertTrue(tween->isComplete());void TweenTest::()      { startCount++; }void TweenTest::()      { repeatCount++; }void TweenTest::()      { completeCount++; }
        }


        void TweenTest::                testReverseTween()
        {
            int startCount = 0;
            int completeCount = 0;

            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0);
            tween->repeatCount ( 4);
            tween->reverse ( true);
            tween->animate("x", 100);

            tween->advanceTime(0.75);
            assertThat(quad->x(), closeTo(75, E));

            tween->advanceTime(0.5);
            assertThat(quad->x(), closeTo(75, E));

            tween->advanceTime(0.5);
            assertThat(quad->x(), closeTo(25, E));
            assertFalse(tween->isComplete());

            tween->advanceTime(1.25);
            assertThat(quad->x(), closeTo(100, E));
            assertFalse(tween->isComplete());

            tween->advanceTime(10);
            assertThat(quad->x(), closeTo(0, E));
            assertTrue(tween->isComplete());
        }


        void TweenTest::                testInfiniteTween()
        {
            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0);
            tween->animate("x", 100);
            tween->repeatCount ( 0);

            tween->advanceTime(30.5);
            assertThat(quad->x(), closeTo(50, E));

            tween->advanceTime(100.5);
            assertThat(quad->x(), closeTo(100, E));
            assertFalse(tween->isComplete());
        }


        void TweenTest::                testGetEndValue()
        {
            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0);
            tween->animate("x", 100);
            tween->fadeTo(0);
            tween->scaleTo(1.5);

            Assert::assertEquals(100, tween->getEndValue("x"));
            Assert::assertEquals(0, tween->getEndValue("alpha"));
            Assert::assertEquals(1.5, tween->getEndValue("scaleX"));
            Assert::assertEquals(1.5, tween->getEndValue("scaleY"));
        }


        void TweenTest::                testProgress()
        {
            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, 1.0, easeIn);
            Assert::assertEquals(0.0, tween->progress());

            tween->advanceTime(0.5);
            assertThat(tween->progress(), closeTo(easeIn(0.5), E));

            tween->advanceTime(0.25);
            assertThat(tween->progress(), closeTo(easeIn(0.75), E));

            tween->advanceTime(1.0);
            assertThat(tween->progress(), closeTo(easeIn(1.0), E));float TweenTest::(float ratio)
            {
                return ratio * ratio * ratio;
            }
        }

        void TweenTest::executeTween(float time, float advanceTime)
        {
            Quad* quad = new Quad(100, 100);
            Tween* tween = new Tween(quad, time);
            tween->animate("x", 100);

            int startCount = 0;
            int updateCount = 0;
            int completeCount = 0;

            tween->onStart    ( function()void  { startCount++; });
            tween->onUpdate   ( function()void  { updateCount++ });
            tween->onComplete ( function()void  { completeCount++ });

            tween->advanceTime(advanceTime);

            Assert::assertEquals(1, updateCount);
            Assert::assertEquals(1, startCount);
            Assert::assertEquals(advanceTime >= time ? 1 : 0, completeCount);
        }
}

