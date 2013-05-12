// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextField.h"
#include "flash/display/BitmapData.h"
#include "flash/display/StageQuality.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Rectangle.h"
#include "flash/text/AntiAliasType.h"
#include "flash/text/TextField.h"
#include "flash/text/TextFormat.h"
#include "flash/utils/Dictionary.h"

#include "starling/core/RenderSupport.h"
#include "starling/core/Starling.h"
#include "starling/display/DisplayObject.h"
#include "starling/display/DisplayObjectContainer.h"
#include "starling/display/Image.h"
#include "starling/display/Quad.h"
#include "starling/display/QuadBatch.h"
#include "starling/display/Sprite.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

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
using namespace flash::display;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::text;
using namespace flash::text;
using namespace flash::text;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;
using namespace starling::utils;

namespace starling
{
    namespace text
    {


        // the name container with the registered bitmap fonts
        const std::string TextField::BITMAP_FONT_DATA_NAME="starling.display.TextField.BitmapFonts";

        // this object will be used for text rendering
        flash::text::TextField *TextField::sNativeTextField=newflash->text->TextField();

        /** Create a new text field with the given properties. */
        TextField::TextField(int width, int height, std::string text, std::string fontName,
                             float fontSize, unsigned int color, bool bold)
        {
            mText = text ? text : "";
            mFontSize = fontSize;
            mColor = color;
            mHAlign = HAlign::CENTER;
            mVAlign = VAlign::CENTER;
            mBorder = NULL;
            mKerning = true;
            mBold = bold;
            mAutoSize = TextFieldAutoSize->NONE;
            this->fontName= fontName;

            mHitArea = new Quad(width, height);
            mHitArea->alpha= 0.0;
            addChild(mHitArea);

            addEventListener(Event::FLATTEN,onFlatten);
        }

        /** Disposes the underlying texture data. */
        void TextField::dispose()
        {
            removeEventListener(Event::FLATTEN,onFlatten);
            if (mImage) mImage->texture->dispose();
            if (mQuadBatch) mQuadBatch->dispose();
            super->dispose();
        }

        void TextField::onFlatten()
        {
            if (mRequiresRedraw) redraw();
        }

        /** @inheritDoc */
        void TextField::render(RenderSupport *support, float parentAlpha)
        {
            if (mRequiresRedraw) redraw();
            super->render(support,parentAlpha);
        }

        /** Forces the text field to be constructed right away. Normally,
         *  it will only do so lazily, i.e. before being rendered. */
        void TextField::redraw()
        {
            if (mRequiresRedraw)
            {
                if (mIsRenderedText) createRenderedContents();
                else                 createComposedContents();

                updateBorder();
                mRequiresRedraw = false;
            }
        }

        void TextField::createRenderedContents()
        {
            if (mQuadBatch)
            {
                mQuadBatch->removeFromParent(true);
                mQuadBatch = NULL;
            }

            float scale  = Starling::contentScaleFactor;
            float width  = mHitArea->width * scale;
            float height = mHitArea->height* scale;
            std::string hAlign=mHAlign;
            std::string vAlign=mVAlign;

            if (isHorizontalAutoSize)
            {
                width = int->MAX_VALUE;
                hAlign = HAlign::LEFT;
            }
            if (isVerticalAutoSize)
            {
                height = int->MAX_VALUE;
                vAlign = VAlign::TOP;
            }

            TextFormat *textFormat=new TextFormat(mFontName,
                                                  mFontSize * scale, mColor, mBold, mItalic, mUnderline, NULL, NULL, hAlign);
            textFormat->kerning= mKerning;

            sNativeTextField->defaultTextFormat= textFormat;
            sNativeTextField->width= width;
            sNativeTextField->height= height;
            sNativeTextField->antiAliasType= AntiAliasType::ADVANCED;
            sNativeTextField->selectable= false;
            sNativeTextField->multiline= true;
            sNativeTextField->wordWrap= true;
            sNativeTextField->text= mText;
            sNativeTextField->embedFonts= true;
            sNativeTextField->filters= mNativeFilters;

            // we try embedded fonts first, non-embedded fonts are just a fallback
            if (sNativeTextField->textWidth== 0.0 || sNativeTextField->textHeight== 0.0)
                sNativeTextField->embedFonts= false;

            formatText(sNativeTextField, textFormat);

            if (mAutoScale)
                autoScaleNativeTextField(sNativeTextField);

            float textWidth  = sNativeTextField->textWidth;
            float textHeight = sNativeTextField->textHeight;

            if (isHorizontalAutoSize)
                sNativeTextField->width= width = Math::ceil(textWidth+ 5);
            if (isVerticalAutoSize)
                sNativeTextField->height= height = Math::ceil(textHeight+ 4);

            float xOffset = 0.0;
            if (hAlign == HAlign::LEFT)       xOffset = 2; // flash adds a 2 pixel offset
            else if (hAlign == HAlign::CENTER)xOffset = (width - textWidth) / 2.0;
            else if (hAlign == HAlign::RIGHT) xOffset =  width - textWidth - 2;

            float yOffset = 0.0;
            if (vAlign == VAlign::TOP)        yOffset = 2; // flash adds a 2 pixel offset
            else if (vAlign == VAlign::CENTER)yOffset = (height - textHeight) / 2.0;
            else if (vAlign == VAlign::BOTTOM)yOffset =  height - textHeight - 2;

            BitmapData *bitmapData=new BitmapData(width, height, true, 0x0);
            Matrix *drawMatrix=new Matrix(1, 0, 0, 1, 0, int(yOffset)-2);
            Function *drawWithQualityFunc=
                "drawWithQuality" in bitmapData ? bitmapData["drawWithQuality"] : NULL;

            // Beginning with AIR 3.3, we can force a drawing quality. Since "LOW" produces
            // wrong output oftentimes, we force "MEDIUM" if possible.

            if (dynamic_cast<Function>(drawWithQualityFunc))
                drawWithQualityFunc->call(bitmapData,sNativeTextField, drawMatrix,
                                          NULL, NULL, NULL, false, StageQuality::MEDIUM);
            else
                bitmapData->draw(sNativeTextField,drawMatrix);

            sNativeTextField->text= "";

            // update textBounds rectangle
            if (mTextBounds == NULL) mTextBounds = new Rectangle();
            mTextBounds->setTo(xOffset  / scale, yOffset    / scale,
                               textWidth / scale, textHeight / scale);

            // update hit area
            mHitArea->width = width  / scale;
            mHitArea->height= height / scale;

            Texture *texture=Texture::fromBitmapData(bitmapData,false, false, scale);

            if (mImage == NULL)
            {
                mImage = new Image(texture);
                mImage->touchable= false;
                addChild(mImage);
            }
            else
            {
                mImage->texture->dispose();
                mImage->texture= texture;
                mImage->readjustSize();
            }
        }

        /** formatText is called immediately before the text is rendered. The intent of formatText
         *  is to be overridden in a subclass, so that you can provide custom formatting for TextField.
         *  <code>textField</code> is the flash.text.TextField object that you can specially format;
         *  <code>textFormat</code> is the default TextFormat for <code>textField</code>.
         */
        void TextField::formatText(flash::text::TextField *textField, TextFormat *textFormat)      {}

        void TextField::autoScaleNativeTextField(flash::text::TextField *textField)
        {
            float size   = Number(textField->defaultTextFormat->size);
            int maxHeight= textField->height- 4;
            int maxWidth = textField->width- 4;

            while (textField->textWidth> maxWidth || textField->textHeight> maxHeight)
            {
                if (size <= 4) break;

                TextFormat *format=textField->defaultTextFormat;
                format->size= size--;
                textField->setTextFormat(format);
            }
        }

        void TextField::createComposedContents()
        {
            if (mImage)
            {
                mImage->removeFromParent(true);
                mImage = NULL;
            }

            if (mQuadBatch == NULL)
            {
                mQuadBatch = new QuadBatch();
                mQuadBatch->touchable= false;
                addChild(mQuadBatch);
            }
            else
                mQuadBatch->reset();

            BitmapFont *bitmapFont=bitmapFonts[mFontName];
            if (bitmapFont == NULL) throw new Error("Bitmap font not registered: " + mFontName);

            float width  = mHitArea->width;
            float height = mHitArea->height;
            std::string hAlign=mHAlign;
            std::string vAlign=mVAlign;

            if (isHorizontalAutoSize)
            {
                width = int->MAX_VALUE;
                hAlign = HAlign::LEFT;
            }
            if (isVerticalAutoSize)
            {
                height = int->MAX_VALUE;
                vAlign = VAlign::TOP;
            }

            bitmapFont->fillQuadBatch(mQuadBatch,
                                      width, height, mText, mFontSize, mColor, hAlign, vAlign, mAutoScale, mKerning);

            if (mAutoSize != TextFieldAutoSize->NONE)
            {
                mTextBounds = mQuadBatch->getBounds(mQuadBatch,mTextBounds);

                if (isHorizontalAutoSize)
                    mHitArea->width = mTextBounds->x+ mTextBounds->width;
                if (isVerticalAutoSize)
                    mHitArea->height= mTextBounds->y+ mTextBounds->height;
            }
            else
            {
                // hit area doesn't change, text bounds can be created on demand
                mTextBounds = NULL;
            }
        }

        void TextField::updateBorder()
        {
            if (mBorder == NULL) return;

            float width  = mHitArea->width;
            float height = mHitArea->height;

            Quad *topLine  = mBorder->getChildAt(0)as Quad;
            Quad *rightLine= mBorder->getChildAt(1)as Quad;
            Quad *bottomLine=mBorder->getChildAt(2)as Quad;
            Quad *leftLine = mBorder->getChildAt(3)as Quad;

            topLine->width   = width;
            topLine->height   = 1;
            bottomLine->width= width;
            bottomLine->height= 1;
            leftLine->width  = 1;
            leftLine->height  = height;
            rightLine->width = 1;
            rightLine->height = height;
            rightLine->x = width  - 1;
            bottomLine->y= height - 1;
            topLine->color= rightLine->color= bottomLine->color= leftLine->color= mColor;
        }

        bool TextField::isHorizontalAutoSize()
        {
            return mAutoSize == TextFieldAutoSize->HORIZONTAL||
                   mAutoSize == TextFieldAutoSize->BOTH_DIRECTIONS;
        }

        bool TextField::isVerticalAutoSize()
        {
            return mAutoSize == TextFieldAutoSize->VERTICAL||
                   mAutoSize == TextFieldAutoSize->BOTH_DIRECTIONS;
        }

        /** Returns the bounds of the text within the text field. */
        Rectangle *TextField::textBounds()
        {
            if (mRequiresRedraw) redraw();
            if (mTextBounds == NULL) mTextBounds = mQuadBatch->getBounds(mQuadBatch);
            return mTextBounds->clone();
        }

        /** @inheritDoc */
        Rectangle *TextField::getBounds(DisplayObject *targetSpace, Rectangle *resultRect)
        {
            if (mRequiresRedraw) redraw();
            return mHitArea->getBounds(targetSpace,resultRect);
        }

        /** @inheritDoc */
        void TextField::width(float value)
        {
            // different to ordinary display objects, changing the size of the text field should
            // not change the scaling, but make the texture bigger/smaller, while the size
            // of the text/font stays the same (this applies to the height, as well).

            mHitArea->width= value;
            mRequiresRedraw = true;
        }

        /** @inheritDoc */
        void TextField::height(float value)
        {
            mHitArea->height= value;
            mRequiresRedraw = true;
        }

        /** The displayed text. */
        std::string TextField::text()
        {
            return mText;
        }
        void TextField::text(std::string value)
        {
            if (value == NULL) value = "";
            if (mText != value)
            {
                mText = value;
                mRequiresRedraw = true;
            }
        }

        /** The name of the font (true type or bitmap font). */
        std::string TextField::fontName()
        {
            return mFontName;
        }
        void TextField::fontName(std::string value)
        {
            if (mFontName != value)
            {
                if (value == BitmapFont->MINI&& bitmapFonts[value] == undefined)
                    registerBitmapFont(new BitmapFont());

                mFontName = value;
                mRequiresRedraw = true;
                mIsRenderedText = bitmapFonts[value] == undefined;
            }
        }

        /** The size of the font. For bitmap fonts, use <code>BitmapFont.NATIVE_SIZE</code> for
         *  the original size. */
        float TextField::fontSize()
        {
            return mFontSize;
        }
        void TextField::fontSize(float value)
        {
            if (mFontSize != value)
            {
                mFontSize = value;
                mRequiresRedraw = true;
            }
        }

        /** The color of the text. For bitmap fonts, use <code>Color.WHITE</code> to use the
         *  original, untinted color. @default black */
        unsigned int TextField::color()
        {
            return mColor;
        }
        void TextField::color(unsigned int value)
        {
            if (mColor != value)
            {
                mColor = value;
                mRequiresRedraw = true;
            }
        }

        /** The horizontal alignment of the text. @default center @see starling.utils.HAlign */
        std::string TextField::hAlign()
        {
            return mHAlign;
        }
        void TextField::hAlign(std::string value)
        {
            if (!HAlign::isValid(value))
                throw new ArgumentError("Invalid horizontal align: " + value);

            if (mHAlign != value)
            {
                mHAlign = value;
                mRequiresRedraw = true;
            }
        }

        /** The vertical alignment of the text. @default center @see starling.utils.VAlign */
        std::string TextField::vAlign()
        {
            return mVAlign;
        }
        void TextField::vAlign(std::string value)
        {
            if (!VAlign::isValid(value))
                throw new ArgumentError("Invalid vertical align: " + value);

            if (mVAlign != value)
            {
                mVAlign = value;
                mRequiresRedraw = true;
            }
        }

        /** Draws a border around the edges of the text field. Useful for visual debugging.
         *  @default false */
        bool TextField::border()
        {
            return mBorder != NULL;
        }
        void TextField::border(bool value)
        {
            if (value && mBorder == NULL)
            {
                mBorder = new Sprite();
                addChild(mBorder);

                for ( int i=0; i<4; ++i)
                    mBorder->addChild(newQuad(1.0, 1.0));

                updateBorder();
            }
            else if (!value && mBorder != NULL)
            {
                mBorder->removeFromParent(true);
                mBorder = NULL;
            }
        }

        /** Indicates whether the text is bold. @default false */
        bool TextField::bold()
        {
            return mBold;
        }
        void TextField::bold(bool value)
        {
            if (mBold != value)
            {
                mBold = value;
                mRequiresRedraw = true;
            }
        }

        /** Indicates whether the text is italicized. @default false */
        bool TextField::italic()
        {
            return mItalic;
        }
        void TextField::italic(bool value)
        {
            if (mItalic != value)
            {
                mItalic = value;
                mRequiresRedraw = true;
            }
        }

        /** Indicates whether the text is underlined. @default false */
        bool TextField::underline()
        {
            return mUnderline;
        }
        void TextField::underline(bool value)
        {
            if (mUnderline != value)
            {
                mUnderline = value;
                mRequiresRedraw = true;
            }
        }

        /** Indicates whether kerning is enabled. @default true */
        bool TextField::kerning()
        {
            return mKerning;
        }
        void TextField::kerning(bool value)
        {
            if (mKerning != value)
            {
                mKerning = value;
                mRequiresRedraw = true;
            }
        }

        /** Indicates whether the font size is scaled down so that the complete text fits
         *  into the text field. @default false */
        bool TextField::autoScale()
        {
            return mAutoScale;
        }
        void TextField::autoScale(bool value)
        {
            if (mAutoScale != value)
            {
                mAutoScale = value;
                mRequiresRedraw = true;
            }
        }

        /** Specifies the type of auto-sizing the TextField will do.
         *  Note that any auto-sizing will make auto-scaling useless. Furthermore, it has
         *  implications on alignment: horizontally auto-sized text will always be left-,
         *  vertically auto-sized text will always be top-aligned. @default "none" */
        std::string TextField::autoSize()
        {
            return mAutoSize;
        }
        void TextField::autoSize(std::string value)
        {
            if (mAutoSize != value)
            {
                mAutoSize = value;
                mRequiresRedraw = true;
            }
        }

        /** The native Flash BitmapFilters to apply to this TextField.
         *  Only available when using standard (TrueType) fonts! */
        std::vector<void *> TextField::nativeFilters()
        {
            return mNativeFilters;
        }
        void TextField::nativeFilters(std::vector<void *> value)
        {
            if (!mIsRenderedText)
                throw(new Error("The TextField.nativeFilters property cannot be used on Bitmap fonts."));

            mNativeFilters = value->concat();
            mRequiresRedraw = true;
        }

        /** Makes a bitmap font available at any TextField in the current stage3D context.
         *  The font is identified by its <code>name</code>.
         *  Per default, the <code>name</code> property of the bitmap font will be used, but you
         *  can pass a custom name, as well. @returns the name of the font. */
        std::string TextField::registerBitmapFont(BitmapFont *bitmapFont, std::string name)
        {
            if (name == NULL) name = bitmapFont->name;
            bitmapFonts[name] = bitmapFont;
            return name;
        }

        /** Unregisters the bitmap font and, optionally, disposes it. */
        void TextField::unregisterBitmapFont(std::string name, bool dispose)
        {
            if (dispose && bitmapFonts[name] != undefined)
                bitmapFonts[name]->dispose();

            delete bitmapFonts[name];
        }

        /** Returns a registered bitmap font (or null, if the font has not been registered). */
        BitmapFont *TextField::getBitmapFont(std::string name)
        {
            return bitmapFonts[name];
        }

        /** Stores the currently available bitmap fonts. Since a bitmap font will only work
         *  in one Stage3D context, they are saved in Starling's 'contextData' property. */
        std::map<std::string, void *> TextField::bitmapFonts()
        {
            std::map<std::string, void *> fonts=Starling::current->static_cast<std::map<std::string, void *>>(contextData[BITMAP_FONT_DATA_NAME]);

            if (fonts.empty())
            {
                fonts.clear();
                Starling::current->contextData[BITMAP_FONT_DATA_NAME]=fonts;
            }

            return fonts;
        }
    }
}

