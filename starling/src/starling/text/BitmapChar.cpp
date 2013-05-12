// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "BitmapChar.h"
#include "flash/utils/Dictionary.h"

#include "starling/display/Image.h"
#include "starling/textures/Texture.h"

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


        /** Creates a char with a texture and its properties. */
        BitmapChar::BitmapChar(int id, Texture *texture,
                               float xOffset, float yOffset, float xAdvance)
        {
            mCharID = id;
            mTexture = texture;
            mXOffset = xOffset;
            mYOffset = yOffset;
            mXAdvance = xAdvance;
            mKernings.clear();
        }

        /** Adds kerning information relative to a specific other character ID. */
        void BitmapChar::addKerning(int charID, float amount)
        {
            if (mKernings.empty())
                mKernings.clear();

            mKernings[charID] = amount;
        }

        /** Retrieve kerning information relative to the given character ID. */
        float BitmapChar::getKerning(int charID)
        {
            if (mKernings.empty() || mKernings[charID] == undefined) return 0.0;
            else return mKernings[charID];
        }

        /** Creates an image of the char. */
        Image *BitmapChar::createImage()
        {
            return new Image(mTexture);
        }

        /** The unicode ID of the char. */
        int BitmapChar::charID()
        {
            return mCharID;
        }

        /** The number of points to move the char in x direction on character arrangement. */
        float BitmapChar::xOffset()
        {
            return mXOffset;
        }

        /** The number of points to move the char in y direction on character arrangement. */
        float BitmapChar::yOffset()
        {
            return mYOffset;
        }

        /** The number of points the cursor has to be moved to the right for the next char. */
        float BitmapChar::xAdvance()
        {
            return mXAdvance;
        }

        /** The texture of the character. */
        Texture *BitmapChar::texture()
        {
            return mTexture;
        }

        /** The width of the character in points. */
        float BitmapChar::width()
        {
            return mTexture->width;
        }

        /** The height of the character in points. */
        float BitmapChar::height()
        {
            return mTexture->height;
        }
    }
}

