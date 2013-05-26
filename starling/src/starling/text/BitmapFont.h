#if !defined(__STARLING_SRC_STARLING_TEXT_BITMAPFONT_AS)
#define __STARLING_SRC_STARLING_TEXT_BITMAPFONT_AS
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
    namespace geom
    {
        class Rectangle;
    }
}
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
    namespace display
    {
        class QuadBatch;
    }
}
namespace starling
{
    namespace display
    {
        class Sprite;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace textures
    {
        class TextureSmoothing;
    }
}
namespace starling
{
    namespace utils
    {
        class HAlign;
    }
}
namespace starling
{
    namespace utils
    {
        class VAlign;
    }
}

namespace starling
{
    namespace text
    {
        class BitmapChar;
    }
}

/** The BitmapFont class parses bitmap font files and arranges the glyphs
 *  in the form of a text.
 *
 *  The class parses the XML format as it is used in the
 *  <a href="http://www.angelcode.com/products/bmfont/">AngelCode Bitmap Font Generator</a> or
 *  the <a href="http://glyphdesigner.71squared.com/">Glyph Designer</a>.
 *  This is what the file format looks like:
 *
 *  <pre>
 *  &lt;font&gt;
 *    &lt;info face="BranchingMouse" size="40" /&gt;
 *    &lt;common lineHeight="40" /&gt;
 *    &lt;pages&gt;  &lt;!-- currently, only one page is supported --&gt;
 *      &lt;page id="0" file="texture.png" /&gt;
 *    &lt;/pages&gt;
 *    &lt;chars&gt;
 *      &lt;char id="32" x="60" y="29" width="1" height="1" xoffset="0" yoffset="27" xadvance="8" /&gt;
 *      &lt;char id="33" x="155" y="144" width="9" height="21" xoffset="0" yoffset="6" xadvance="9" /&gt;
 *    &lt;/chars&gt;
 *    &lt;kernings&gt; &lt;!-- Kerning is optional --&gt;
 *      &lt;kerning first="83" second="83" amount="-4"/&gt;
 *    &lt;/kernings&gt;
 *  &lt;/font&gt;
 *  </pre>
 *
 *  Pass an instance of this class to the method <code>registerBitmapFont</code> of the
 *  TextField class. Then, set the <code>fontName</code> property of the text field to the
 *  <code>name</code> value of the bitmap font. This will make the text field use the bitmap
 *  font.
 */

using namespace flash::geom;
using namespace flash::utils;
using namespace starling::display;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace text
    {
        class BitmapFont
        {
            /** Use this constant for the <code>fontSize</code> property of the TextField class to
             *  render the bitmap font in exactly the size it was created. */
        public:
            static const int NATIVE_SIZE;

            /** The font name of the embedded minimal bitmap font. Use this e.g. for debug output. */
        public:
            static const std::string MINI;

        private:
            static const int CHAR_SPACE;
        private:
            static const int CHAR_TAB;
        private:
            static const int CHAR_NEWLINE;
        private:
            static const int CHAR_CARRIAGE_RETURN;

        private:
            Texture *mTexture;
        private:
            std::map<std::string, void *> mChars;
        private:
            std::string mName;
        private:
            float mSize;
        private:
            float mLineHeight;
        private:
            float mBaseline;
        private:
            Image *mHelperImage;
        private:
            std::vector<CharLocation *> mCharLocationPool;

            /** Creates a bitmap font by parsing an XML file and uses the specified texture.
             *  If you don't pass any data, the "mini" font will be created. */
        public:
            BitmapFont(Texture *texture=NULL, XML *fontXml=NULL);

            /** Disposes the texture of the bitmap font! */
        public:
            void     dispose();

        private:
            void     parseFontXml(XML *fontXml);

            /** Returns a single bitmap char with a certain character ID. */
        public:
            BitmapChar *getChar(int charID);

            /** Adds a bitmap char with a certain character ID. */
        public:
            void     addChar(int charID, BitmapChar *bitmapChar);

            /** Creates a sprite that contains a certain text, made up by one image per char. */
        public:
            Sprite   *createSprite(float width, float height, std::string text,
                                   float fontSize=-1, unsigned int color=0xffffff,
                                   std::string hAlign="center", std::string vAlign="center",
                                   bool autoScale=true,
                                   bool kerning=true);

            /** Draws text into a QuadBatch. */
        public:
            void     fillQuadBatch(QuadBatch *quadBatch, float width, float height, std::string text,
                                   float fontSize=-1, unsigned int color=0xffffff,
                                   std::string hAlign="center", std::string vAlign="center",
                                   bool autoScale=true,
                                   bool kerning=true);

            /** Arranges the characters of a text inside a rectangle, adhering to the given settings.
             *  Returns a Vector of CharLocations. */
        private:
            std::vector<CharLocation *> arrangeChars(float width, float height, std::string text, float fontSize=-1,
                    std::string hAlign="center", std::string vAlign="center",
                    bool autoScale=true, bool kerning=true);

            /** The name of the font as it was parsed from the font file. */
        public:
            std::string       name();

            /** The native size of the font. */
        public:
            float       size();

            /** The height of one line in pixels. */
        public:
            float       lineHeight();
        public:
            void         lineHeight(float value);

            /** The smoothing filter that is used for the texture. */
        public:
            std::string       smoothing();
        public:
            void         smoothing(std::string value);

            /** The baseline of the font. */
        public:
            float       baseline();
        };
    }
}


class CharLocation
{
public:
    BitmapChar *char;
public:
    float scale;
public:
    float x;
public:
    float y;

public:
    CharLocation(BitmapChar *char);
};
#endif // __STARLING_SRC_STARLING_TEXT_BITMAPFONT_AS
#endif // __cplusplus

