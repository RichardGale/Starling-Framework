#if !defined(__STARLING_SRC_STARLING_TEXT_BITMAPCHAR_AS)
#define __STARLING_SRC_STARLING_TEXT_BITMAPCHAR_AS
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



#include "flex11.6.h"
namespace flash
{
    namespace utils
    {
        class Dictionary;
    }
}

namespace starling
{
    namespace display
    {
        class Image;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}

/** A BitmapChar contains the information about one char of a bitmap font.
 *  <em>You don't have to use this class directly in most cases.
 *  The TextField class contains methods that handle bitmap fonts for you.</em>
 */

using namespace flash::utils;
using namespace starling::display;
using namespace starling::textures;

namespace starling
{
    namespace text
    {
        class BitmapChar
        {
        private:
            Texture *mTexture;
        private:
            int mCharID;
        private:
            float mXOffset;
        private:
            float mYOffset;
        private:
            float mXAdvance;
        private:
            std::map<std::string, void *> mKernings;

            /** Creates a char with a texture and its properties. */
        public:
            BitmapChar(int id, Texture *texture,
                       float xOffset, float yOffset, float xAdvance);

            /** Adds kerning information relative to a specific other character ID. */
        public:
            void     addKerning(int charID, float amount);

            /** Retrieve kerning information relative to the given character ID. */
        public:
            float   getKerning(int charID);

            /** Creates an image of the char. */
        public:
            Image    *createImage();

            /** The unicode ID of the char. */
        public:
            int          charID();

            /** The number of points to move the char in x direction on character arrangement. */
        public:
            float       xOffset();

            /** The number of points to move the char in y direction on character arrangement. */
        public:
            float       yOffset();

            /** The number of points the cursor has to be moved to the right for the next char. */
        public:
            float       xAdvance();

            /** The texture of the character. */
        public:
            Texture      *texture();

            /** The width of the character in points. */
        public:
            float       width();

            /** The height of the character in points. */
        public:
            float       height();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXT_BITMAPCHAR_AS
#endif // __cplusplus

