#if !defined(__STARLING_SRC_STARLING_UTILS_COLOR_AS)
#define __STARLING_SRC_STARLING_UTILS_COLOR_AS
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
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A utility class containing predefined colors and methods converting between different
 *  color representations. */
using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class Color
        {
        public:
            static const unsigned int WHITE;
        public:
            static const unsigned int SILVER;
        public:
            static const unsigned int GRAY;
        public:
            static const unsigned int BLACK;
        public:
            static const unsigned int RED;
        public:
            static const unsigned int MAROON;
        public:
            static const unsigned int YELLOW;
        public:
            static const unsigned int OLIVE;
        public:
            static const unsigned int LIME;
        public:
            static const unsigned int GREEN;
        public:
            static const unsigned int AQUA;
        public:
            static const unsigned int TEAL;
        public:
            static const unsigned int BLUE;
        public:
            static const unsigned int NAVY;
        public:
            static const unsigned int FUCHSIA;
        public:
            static const unsigned int PURPLE;

            /** Returns the alpha part of an ARGB color (0 - 255). */
        public:
            static int getAlpha(unsigned int color);

            /** Returns the red part of an (A)RGB color (0 - 255). */
        public:
            static int getRed(unsigned int color);

            /** Returns the green part of an (A)RGB color (0 - 255). */
        public:
            static int getGreen(unsigned int color);

            /** Returns the blue part of an (A)RGB color (0 - 255). */
        public:
            static int getBlue(unsigned int color);

            /** Creates an RGB color, stored in an unsigned integer. Channels are expected
             *  in the range 0 - 255. */
        public:
            static unsigned int rgb(int red, int green, int blue);

            /** Creates an ARGB color, stored in an unsigned integer. Channels are expected
             *  in the range 0 - 255. */
        public:
            static unsigned int argb(int alpha, int red, int green, int blue);

            /** @private */
        public:
            Color();
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_COLOR_AS
#endif // __cplusplus

