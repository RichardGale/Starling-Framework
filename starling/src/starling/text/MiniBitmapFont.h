#if !defined(__STARLING_SRC_STARLING_TEXT_MINIBITMAPFONT_AS)
#define __STARLING_SRC_STARLING_TEXT_MINIBITMAPFONT_AS
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
namespace flash
{
    namespace display
    {
        class BitmapData;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}
namespace flash
{
    namespace utils
    {
        class ByteArray;
    }
}

namespace starling
{
    namespace textures
    {
        class Texture;
    }
}

/** @private
 *  This class contains constants for the 'MINI' bitmap font. It's done that way to avoid
 *  a dependency on the 'mx.core' library (which is required for the 'Embed' statement).
 *
 *  <p>The font is based on "uni05_53.ttf" from Craig Kroeger (http://www.miniml.com) and was
 *  converted to a Bitmap Font with "GlyphDesigner" from 71squared (http://www.71squared.com).
 *  </p> */
using namespace flash::display;
using namespace flash::geom;
using namespace flash::utils;
using namespace starling::textures;

namespace starling
{
    namespace text
    {
        class MiniBitmapFont
        {
        private:
            static const int BITMAP_WIDTH;
        private:
            static const int BITMAP_HEIGHT;
        private:
            static const std::vector<void *> BITMAP_DATA;

        private:
            static const XML *XML_DATA





































































































































































































        public:
            static Texture *texture();

        public:
            static XML  *xml();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXT_MINIBITMAPFONT_AS
#endif // __cplusplus

