#if !defined(__STARLING_SRC_STARLING_UTILS_RECTANGLEUTIL_AS)
#define __STARLING_SRC_STARLING_UTILS_RECTANGLEUTIL_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
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

namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A utility class containing methods related to the Rectangle class. */

using namespace flash::geom;
using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class RectangleUtil
        {
            /** @private */
        public:
            RectangleUtil();

            /** Calculates the intersection between two Rectangles. If the rectangles do not intersect,
             *  this method returns an empty Rectangle object with its properties set to 0. */
        public:
            static Rectangle *intersect(Rectangle *rect1, Rectangle *rect2,
                                        Rectangle *resultRect=NULL);

            /** Calculates a rectangle with the same aspect ratio as the given 'rectangle',
             *  centered within 'into'.
             *
             *  <p>This method is useful for calculating the optimal viewPort for a certain display
             *  size. You can use different scale modes to specify how the result should be calculated;
             *  furthermore, you can avoid pixel alignment errors by only allowing whole-number
             *  multipliers/divisors (e.g. 3, 2, 1, 1/2, 1/3).</p>
             *
             *  @see starling.utils.ScaleMode
             */
        public:
            static Rectangle *fit(Rectangle *rectangle, Rectangle *into,
                                  std::string scaleMode="showAll", bool pixelPerfect=false,
                                  Rectangle *resultRect=NULL);

            /** Calculates the next whole-number multiplier or divisor, moving either up or down. */
        private:
            static float   nextSuitableScaleFactor(float factor, bool up);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_RECTANGLEUTIL_AS
#endif // __cplusplus

