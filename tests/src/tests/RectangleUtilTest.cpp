// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "RectangleUtilTest.h"
#include "flash/geom/Rectangle.h"

#include "starling/utils/RectangleUtil.h"
#include "starling/utils/ScaleMode.h"

using namespace flash::geom;
using namespace starling::utils;

namespace tests {



        void RectangleUtilTest::                testIntersection()
        {
            Rectangle* expectedRect;
            Rectangle* rect = new Rectangle(-5, -10, 10, 20);

            Rectangle* overlapRect = new Rectangle(-10, -15, 10, 10);
            Rectangle* identRect = new Rectangle(-5, -10, 10, 20);
            Rectangle* outsideRect = new Rectangle(10, 10, 10, 10);
            Rectangle* touchingRect = new Rectangle(5, 0, 10, 10);
            Rectangle* insideRect = new Rectangle(0, 0, 1, 2);

            expectedRect = new Rectangle(-5, -10, 5, 5);
            Helpers()->compareRectangles(expectedRect,
                RectangleUtil::intersect(rect, overlapRect));

            expectedRect = rect;
            Helpers()->compareRectangles(expectedRect,
                RectangleUtil::intersect(rect, identRect));

            expectedRect = new Rectangle();
            Helpers()->compareRectangles(expectedRect,
                RectangleUtil::intersect(rect, outsideRect));

            expectedRect = new Rectangle(5, 0, 0, 10);
            Helpers()->compareRectangles(expectedRect,
                RectangleUtil::intersect(rect, touchingRect));

            expectedRect = insideRect;
            Helpers()->compareRectangles(expectedRect,
                RectangleUtil::intersect(rect, insideRect));
        }


        void RectangleUtilTest::                testFit()
        {
            Rectangle* into = new Rectangle(50, 50, 200, 100);

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 200, 100), into),
                new Rectangle(50, 50, 200, 100));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 50, 50), into, ScaleMode::NONE),
                new Rectangle(125, 75, 50, 50));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 400, 200), into, ScaleMode::NONE),
                new Rectangle(-50, 0, 400, 200));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 50, 50), into, ScaleMode::SHOW_ALL),
                new Rectangle(100, 50, 100, 100));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 400, 200), into, ScaleMode::SHOW_ALL),
                new Rectangle(50, 50, 200, 100));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 800, 400), into, ScaleMode::SHOW_ALL),
                new Rectangle(50, 50, 200, 100));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 400, 200), into, ScaleMode::NO_BORDER),
                new Rectangle(50, 50, 200, 100));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 200, 200), into, ScaleMode::NO_BORDER),
                new Rectangle(50, 0, 200, 200));

            Helpers()->compareRectangles(
                RectangleUtil::fit(new Rectangle(0, 0, 800, 800), into, ScaleMode::NO_BORDER),
                new Rectangle(50, 0, 200, 200));
        }
}

