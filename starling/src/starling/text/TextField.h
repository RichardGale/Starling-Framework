#if !defined(__STARLING_SRC_STARLING_TEXT_TEXTFIELD_AS)
#define __STARLING_SRC_STARLING_TEXT_TEXTFIELD_AS
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
    namespace display
    {
        class BitmapData;
    }
}
namespace flash
{
    namespace display
    {
        class StageQuality;
    }
}
namespace flash
{
    namespace geom
    {
        class Matrix;
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
    namespace text
    {
        class AntiAliasType;
    }
}
namespace flash
{
    namespace text
    {
        class TextField;
    }
}
namespace flash
{
    namespace text
    {
        class TextFormat;
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
    namespace core
    {
        class RenderSupport;
    }
}
namespace starling
{
    namespace core
    {
        class Starling;
    }
}
namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
#include "starling/display/DisplayObjectContainer.h"
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
        class Quad;
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
    namespace events
    {
        class Event;
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
        class BitmapFont;
    }
}

/** A TextField displays text, either using standard true type fonts or custom bitmap fonts.
 *
 *  <p>You can set all properties you are used to, like the font name and size, a color, the
 *  horizontal and vertical alignment, etc. The border property is helpful during development,
 *  because it lets you see the bounds of the textfield.</p>
 *
 *  <p>There are two types of fonts that can be displayed:</p>
 *
 *  <ul>
 *    <li>Standard true type fonts. This renders the text just like a conventional Flash
 *        TextField. It is recommended to embed the font, since you cannot be sure which fonts
 *        are available on the client system, and since this enhances rendering quality.
 *        Simply pass the font name to the corresponding property.</li>
 *    <li>Bitmap fonts. If you need speed or fancy font effects, use a bitmap font instead.
 *        That is a font that has its glyphs rendered to a texture atlas. To use it, first
 *        register the font with the method <code>registerBitmapFont</code>, and then pass
 *        the font name to the corresponding property of the text field.</li>
 *  </ul>
 *
 *  For bitmap fonts, we recommend one of the following tools:
 *
 *  <ul>
 *    <li>Windows: <a href="http://www.angelcode.com/products/bmfont">Bitmap Font Generator</a>
 *       from Angel Code (free). Export the font data as an XML file and the texture as a png
 *       with white characters on a transparent background (32 bit).</li>
 *    <li>Mac OS: <a href="http://glyphdesigner.71squared.com">Glyph Designer</a> from
 *        71squared or <a href="http://http://www.bmglyph.com">bmGlyph</a> (both commercial).
 *        They support Starling natively.</li>
 *  </ul>
 */

using namespace flash::display;
using namespace flash::geom;
using namespace flash::text;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace text
    {
        class TextField : public starling::display::DisplayObjectContainer
        {
            // the name container with the registered bitmap fonts
        private:
            static const std::string BITMAP_FONT_DATA_NAME;

        private:
            float mFontSize;
        private:
            unsigned int mColor;
        private:
            std::string mText;
        private:
            std::string mFontName;
        private:
            std::string mHAlign;
        private:
            std::string mVAlign;
        private:
            bool mBold;
        private:
            bool mItalic;
        private:
            bool mUnderline;
        private:
            bool mAutoScale;
        private:
            std::string mAutoSize;
        private:
            bool mKerning;
        private:
            std::vector<void *> mNativeFilters;
        private:
            bool mRequiresRedraw;
        private:
            bool mIsRenderedText;
        private:
            Rectangle *mTextBounds;

        private:
            DisplayObject *mHitArea;
        private:
            DisplayObjectContainer *mBorder;

        private:
            Image *mImage;
        private:
            QuadBatch *mQuadBatch;

            // this object will be used for text rendering
        private:
            static flash::text::TextField *sNativeTextField;

            /** Create a new text field with the given properties. */
        public:
            TextField(int width, int height, std::string text, std::string fontName="Verdana",
                      float fontSize=12, unsigned int color=0x0, bool bold=false);

            /** Disposes the underlying texture data. */
        public:
            virtual void     dispose();

        private:
            void     onFlatten();

            /** @inheritDoc */
        public:
            virtual void     render(RenderSupport *support, float parentAlpha);

            /** Forces the text field to be constructed right away. Normally,
             *  it will only do so lazily, i.e. before being rendered. */
        public:
            void     redraw();

        private:
            void     createRenderedContents();

            /** formatText is called immediately before the text is rendered. The intent of formatText
             *  is to be overridden in a subclass, so that you can provide custom formatting for TextField.
             *  <code>textField</code> is the flash.text.TextField object that you can specially format;
             *  <code>textFormat</code> is the default TextFormat for <code>textField</code>.
             */
        protected:
            void     formatText(flash::text::TextField *textField, TextFormat *textFormat);

        private:
            void     autoScaleNativeTextField(flash::text::TextField *textField);

        private:
            void     createComposedContents();

        private:
            void     updateBorder();

        private:
            bool      isHorizontalAutoSize();

        private:
            bool      isVerticalAutoSize();

            /** Returns the bounds of the text within the text field. */
        public:
            Rectangle    *textBounds();

            /** @inheritDoc */
        public:
            virtual Rectangle *getBounds(DisplayObject *targetSpace, Rectangle *resultRect=NULL);

            /** @inheritDoc */
        public:
            virtual void         width(float value);

            /** @inheritDoc */
        public:
            virtual void         height(float value);

            /** The displayed text. */
        public:
            std::string       text();
        public:
            void         text(std::string value);

            /** The name of the font (true type or bitmap font). */
        public:
            std::string       fontName();
        public:
            void         fontName(std::string value);

            /** The size of the font. For bitmap fonts, use <code>BitmapFont.NATIVE_SIZE</code> for
             *  the original size. */
        public:
            float       fontSize();
        public:
            void         fontSize(float value);

            /** The color of the text. For bitmap fonts, use <code>Color.WHITE</code> to use the
             *  original, untinted color. @default black */
        public:
            unsigned int         color();
        public:
            void         color(unsigned int value);

            /** The horizontal alignment of the text. @default center @see starling.utils.HAlign */
        public:
            std::string       hAlign();
        public:
            void         hAlign(std::string value);

            /** The vertical alignment of the text. @default center @see starling.utils.VAlign */
        public:
            std::string       vAlign();
        public:
            void         vAlign(std::string value);

            /** Draws a border around the edges of the text field. Useful for visual debugging.
             *  @default false */
        public:
            bool      border();
        public:
            void         border(bool value);

            /** Indicates whether the text is bold. @default false */
        public:
            bool      bold();
        public:
            void         bold(bool value);

            /** Indicates whether the text is italicized. @default false */
        public:
            bool      italic();
        public:
            void         italic(bool value);

            /** Indicates whether the text is underlined. @default false */
        public:
            bool      underline();
        public:
            void         underline(bool value);

            /** Indicates whether kerning is enabled. @default true */
        public:
            bool      kerning();
        public:
            void         kerning(bool value);

            /** Indicates whether the font size is scaled down so that the complete text fits
             *  into the text field. @default false */
        public:
            bool      autoScale();
        public:
            void         autoScale(bool value);

            /** Specifies the type of auto-sizing the TextField will do.
             *  Note that any auto-sizing will make auto-scaling useless. Furthermore, it has
             *  implications on alignment: horizontally auto-sized text will always be left-,
             *  vertically auto-sized text will always be top-aligned. @default "none" */
        public:
            std::string       autoSize();
        public:
            void         autoSize(std::string value);

            /** The native Flash BitmapFilters to apply to this TextField.
             *  Only available when using standard (TrueType) fonts! */
        public:
            std::vector<void *>        nativeFilters();
        public:
            void         nativeFilters(std::vector<void *> value);

            /** Makes a bitmap font available at any TextField in the current stage3D context.
             *  The font is identified by its <code>name</code>.
             *  Per default, the <code>name</code> property of the bitmap font will be used, but you
             *  can pass a custom name, as well. @returns the name of the font. */
        public:
            static std::string   registerBitmapFont(BitmapFont *bitmapFont, std::string name="");

            /** Unregisters the bitmap font and, optionally, disposes it. */
        public:
            static void     unregisterBitmapFont(std::string name, bool dispose=true);

            /** Returns a registered bitmap font (or null, if the font has not been registered). */
        public:
            static BitmapFont *getBitmapFont(std::string name);

            /** Stores the currently available bitmap fonts. Since a bitmap font will only work
             *  in one Stage3D context, they are saved in Starling's 'contextData' property. */
        private:
            static std::map<std::string, void *>   bitmapFonts();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXT_TEXTFIELD_AS
#endif // __cplusplus

