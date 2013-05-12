// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DelayedCallTest.h"
#include "flexunit/framework/Assert.h"

#include "org/flexunit/asserts/assertEquals.h"
#include "org/flexunit/asserts/assertFalse.h"
#include "org/flexunit/asserts/assertTrue.h"

#include "starling/animation/DelayedCall.h"

using namespace flexunit::framework;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace starling::animation;

namespace tests
{



    void DelayedCallTest::                testSimple()
    {
        int sum= 0;
        DelayedCall *dc=new DelayedCall(raiseSum, 1.0, [5]);

        dc->advanceTime(0.5);
        assertEquals(0, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(0.25);
        assertEquals(0, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(0.25);
        assertEquals(5, sum);
        assertTrue(dc->isComplete);

        void DelayedCallTest::(int by)
        {
            sum += by;
        }
    }


    void DelayedCallTest::                testRepeated()
    {
        int sum= 0;
        DelayedCall *dc=new DelayedCall(raiseSum, 1.0, [5]);
        dc->repeatCount= 3;

        dc->advanceTime(0.5);
        assertEquals(0, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(1.0);
        assertEquals(5, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(1.0);
        assertEquals(10, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(0.5);
        assertEquals(15, sum);
        assertTrue(dc->isComplete);

        dc->advanceTime(20);
        assertEquals(15, sum);

        void DelayedCallTest::(int by)
        {
            sum += by;
        }
    }


    void DelayedCallTest::                testIndefinitive()
    {
        int sum= 0;
        DelayedCall *dc=new DelayedCall(raiseSum, 1.0, [5]);
        dc->repeatCount= 0;

        dc->advanceTime(1.5);
        assertEquals(5, sum);
        assertFalse(dc->isComplete);

        dc->advanceTime(10.0);
        assertEquals(55, sum);
        assertFalse(dc->isComplete);

        void DelayedCallTest::(int by)
        {
            sum += by;
        }
    }
}

