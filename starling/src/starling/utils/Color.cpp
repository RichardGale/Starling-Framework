// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Color.h"
#include "starling/errors/AbstractClassError.h"

    /** A utility class containing predefined colors and methods converting between different
     *  color representations. */

using namespace starling::errors;

namespace starling {
namespace utils {


        const unsigned int Color::WHITE   = 0xffffff;
        const unsigned int Color::SILVER  = 0xc0c0c0;
        const unsigned int Color::GRAY    = 0x808080;
        const unsigned int Color::BLACK   = 0x000000;
        const unsigned int Color::RED     = 0xff0000;
        const unsigned int Color::MAROON  = 0x800000;
        const unsigned int Color::YELLOW  = 0xffff00;
        const unsigned int Color::OLIVE   = 0x808000;
        const unsigned int Color::LIME    = 0x00ff00;
        const unsigned int Color::GREEN   = 0x008000;
        const unsigned int Color::AQUA    = 0x00ffff;
        const unsigned int Color::TEAL    = 0x008080;
        const unsigned int Color::BLUE    = 0x0000ff;
        const unsigned int Color::NAVY    = 0x000080;
        const unsigned int Color::FUCHSIA = 0xff00ff;
        const unsigned int Color::PURPLE  = 0x800080;

        /** Returns the alpha part of an ARGB color (0 - 255). */
        int Color::getAlpha(unsigned int color)     { return (color >> 24) & 0xff; }

        /** Returns the red part of an (A)RGB color (0 - 255). */
        int Color::getRed(unsigned int color)       { return (color >> 16) & 0xff; }

        /** Returns the green part of an (A)RGB color (0 - 255). */
        int Color::getGreen(unsigned int color)     { return (color >>  8) & 0xff; }

        /** Returns the blue part of an (A)RGB color (0 - 255). */
        int Color::getBlue(unsigned int color)      { return  color        & 0xff; }

        /** Creates an RGB color, stored in an unsigned integer. Channels are expected
         *  in the range 0 - 255. */
        unsigned int Color::rgb(int red, int green, int blue)
        {
            return (red << 16) | (green << 8) | blue;
        }

        /** Creates an ARGB color, stored in an unsigned integer. Channels are expected
         *  in the range 0 - 255. */
        unsigned int Color::argb(int alpha, int red, int green, int blue)
        {
            return (alpha << 24) | (red << 16) | (green << 8) | blue;
        }

        /** @private */
        Color::Color() { throw new AbstractClassError(); }
}
}

