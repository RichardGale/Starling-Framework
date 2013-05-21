// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "BitmapFont.h"
#include "flash/geom/Rectangle.h"
#include "flash/utils/Dictionary.h"

#include "starling/display/Image.h"
#include "starling/display/QuadBatch.h"
#include "starling/display/Sprite.h"
#include "starling/textures/Texture.h"
#include "starling/textures/TextureSmoothing.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

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
using namespace starling::textures;
using namespace starling::utils;

namespace starling {
namespace text {


        /** Use this constant for the <code>fontSize</code> property of the TextField class to 
         *  render the bitmap font in exactly the size it was created. */
        const int BitmapFont::NATIVE_SIZE = -1;

        /** The font name of the embedded minimal bitmap font. Use this e.g. for debug output. */
        const std::string BitmapFont::MINI="mini";

        const int BitmapFont::CHAR_SPACE           = 32;
        const int BitmapFont::CHAR_TAB             =  9;
        const int BitmapFont::CHAR_NEWLINE         = 10;
        const int BitmapFont::CHAR_CARRIAGE_RETURN = 13;

                    
                    
                    
                    
                    
                    
                    
                    

        /** Creates a bitmap font by parsing an XML file and uses the specified texture. 
         *  If you don't pass any data, the "mini" font will be created. */
        BitmapFont::BitmapFont(Texture* texture, XML* fontXml)
        {
            // if no texture is passed in, we create the minimal, embedded font
            if (texture == NULL && fontXml == NULL)
            {
                texture = MiniBitmapFont()->texture;
                fontXml = MiniBitmapFont()->xml();
            }

            mName = "unknown";
            mLineHeight = mSize = mBaseline = 14;
            mTexture = texture;
            mChars.clear();
            mHelperImage = new Image(texture);
            mCharLocationPool.clear();

            if (fontXml) parseFontXml(fontXml);
        }

        /** Disposes the texture of the bitmap font! */
        void BitmapFont::dispose()
        {
            if (mTexture)
                mTexture->dispose();
        }

        void BitmapFont::parseFontXml(XML* fontXml)
        {
             float scale  = mTexture->scale;
             Rectangle* frame= mTexture->frame;

            mName = fontXml->info()->attribute("face");
            mSize = parseFloat(fontXml->info()->attribute("size")) / scale;
            mLineHeight = parseFloat(fontXml->common()->attribute("lineHeight")) / scale;
            mBaseline = parseFloat(fontXml->common()->attribute("base")) / scale;

            if (fontXml->info()->attribute("smooth")->toString() == "0")
                smoothing = TextureSmoothing::NONE;

            if (mSize <= 0)
            {
                trace("[Starling] Warning: invalid font size in '" + mName + "' font.");
                mSize = (mSize == 0.0 ? 16.0 : mSize * -1.0);
            }

            for each (var XML* charElementin fontXml->chars()->char())
            {
                 int id = parseInt(charElement->attribute("id"));
                 float xOffset  = parseFloat(charElement->attribute("xoffset")) / scale;
                 float yOffset  = parseFloat(charElement->attribute("yoffset")) / scale;
                 float xAdvance  = parseFloat(charElement->attribute("xadvance")) / scale;

                 Rectangle* region= new Rectangle();
                region->x ( parseFloat(charElement->attribute("x")) / scale + frame->x());
                region->y ( parseFloat(charElement->attribute("y")) / scale + frame->y());
                region->width  ( parseFloat(charElement->attribute("width")) / scale);
                region->height ( parseFloat(charElement->attribute("height")) / scale);

                 Texture* texture= Texture::fromTexture(mTexture, region);
                 BitmapChar* bitmapChar= new BitmapChar(id, texture, xOffset, yOffset, xAdvance);
                addChar(id, bitmapChar);
            }

            for each (var XML* kerningElementin fontXml->kernings()->kerning())
            {
                 int first = parseInt(kerningElement->attribute("first"));
                 int second = parseInt(kerningElement->attribute("second"));
                 float amount  = parseFloat(kerningElement->attribute("amount")) / scale;
                if (second in !mChars.empty()) getChar(second)->addKerning(first, amount);
            }
        }

        /** Returns a single bitmap char with a certain character ID. */
        BitmapChar* BitmapFont::getChar(int charID)
        {
            return mChars[charID];
        }

        /** Adds a bitmap char with a certain character ID. */
        void BitmapFont::addChar(int charID, BitmapChar* bitmapChar)
        {
            mChars[charID] = bitmapChar;
        }

        /** Creates a sprite that contains a certain text, made up by one image per char. */
        Sprite* BitmapFont::createSprite(float width, float height, std::string text,
                                     float fontSize, unsigned int color,
                                     std::string hAlign, std::string vAlign,
                                     bool autoScale,
                                     bool kerning)
        {
             std::vector<CharLocation*> charLocations=arrangeChars(width,height,text,fontSize,
                                                                   hAlign, vAlign, autoScale, kerning);
             int numChars = charLocations.size();
             Sprite* sprite= new Sprite();

            for ( int i=0; i<numChars; ++i)
            {
                 CharLocation* charLocation= charLocations[i];
                 Image* char= charLocation->char->createImage();
                char->x ( charLocation->x());
                char->y ( charLocation->y());
                char->scaleX ( char->scaleY ( charLocation->scale()));
                char->color ( color);
                sprite->addChild(char);
            }

            return sprite;
        }

        /** Draws text into a QuadBatch. */
        void BitmapFont::fillQuadBatch(QuadBatch* quadBatch, float width, float height, std::string text,
                                      float fontSize, unsigned int color,
                                      std::string hAlign, std::string vAlign,
                                      bool autoScale,
                                      bool kerning)
        {
             std::vector<CharLocation*> charLocations=arrangeChars(width,height,text,fontSize,
                                                                   hAlign, vAlign, autoScale, kerning);
             int numChars = charLocations.size();
            mHelperImage->color ( color);

            if (numChars > 8192)
                throw new ArgumentError("Bitmap Font text is limited to 8192 characters.");

            for ( int i=0; i<numChars; ++i)
            {
                 CharLocation* charLocation= charLocations[i];
                mHelperImage->texture ( charLocation->char()->texture());
                mHelperImage->readjustSize();
                mHelperImage->x ( charLocation->x());
                mHelperImage->y ( charLocation->y());
                mHelperImage->scaleX ( mHelperImage->scaleY ( charLocation->scale()));
                quadBatch->addImage(mHelperImage);
            }
        }

        /** Arranges the characters of a text inside a rectangle, adhering to the given settings. 
         *  Returns a Vector of CharLocations. */
        std::vector<CharLocation*> BitmapFont::arrangeChars(float width, float height, std::string text, float fontSize,
                                      std::string hAlign, std::string vAlign,
                                      bool autoScale, bool kerning)
        {
            if (text == NULL || text.length() == 0) return new <CharLocation*>[];
            if (fontSize < 0) fontSize *= -mSize;

             std::vector<std::vector<CharLocation*>> lines;
             bool finished    = false;
             CharLocation* charLocation;
             int numChars;
             float containerWidth ;
             float containerHeight ;
             float scale ;

            while (!finished)
            {
                scale = fontSize / mSize;
                containerWidth  = width / scale;
                containerHeight = height / scale;

                lines.clear();

                if (mLineHeight <= containerHeight)
                {
                     int lastWhiteSpace = -1;
                     int lastCharID = -1;
                     float currentX  = 0;
                     float currentY  = 0;
                     std::vector<CharLocation*> currentLine=new<CharLocation*>[];

                    numChars = text.length();
                    for ( int i=0; i<numChars; ++i)
                    {
                         bool lineFull    = false;
                         int charID = text.charCodeAt(i);
                         BitmapChar* char= getChar(charID);

                        if (charID == CHAR_NEWLINE || charID == CHAR_CARRIAGE_RETURN)
                        {
                            lineFull = true;
                        }
                        else if (char == NULL)
                        {
                            trace("[Starling] Missing character: " + charID);
                        }
                        else
                        {
                            if (charID == CHAR_SPACE || charID == CHAR_TAB)
                                lastWhiteSpace = i;

                            if (kerning)
                                currentX += char->getKerning(lastCharID);

                            charLocation = mCharLocationPool.length ?
                                mCharLocationPool.pop() : new CharLocation(char);

                            charLocation->char ( char);
                            charLocation->x ( currentX + char->xOffset());
                            charLocation->y ( currentY + char->yOffset());
                            currentLine.push_back(charLocation);

                            currentX += char->xAdvance();
                            lastCharID = charID;

                            if (charLocation->x() + char->width() > containerWidth)
                            {
                                // remove characters and add them again to next line
                                 int numCharsToRemove = lastWhiteSpace == -1 ? 1 : i - lastWhiteSpace;
                                 int removeIndex = currentLine.size() - numCharsToRemove;

                                currentLine.splice(removeIndex, numCharsToRemove);

                                if (currentLine.size() == 0)
                                    break;

                                i -= numCharsToRemove;
                                lineFull = true;
                            }
                        }

                        if (i == numChars - 1)
                        {
                            lines.push_back(currentLine);
                            finished = true;
                        }
                        else if (lineFull)
                        {
                            lines.push_back(currentLine);

                            if (lastWhiteSpace == i)
                                currentLine.pop();

                            if (currentY + 2*mLineHeight <= containerHeight)
                            {
                                currentLine.clear();
                                currentX = 0;
                                currentY += mLineHeight;
                                lastWhiteSpace = -1;
                                lastCharID = -1;
                            }
                            else
                            {
                                break;
                            }
                        }
                    } // for each char
                } // if (mLineHeight <= containerHeight)

                if (autoScale && !finished)
                {
                    fontSize -= 1;
                    lines.clear()    ;
                }
                else
                {
                    finished = true;
                }
            } // while (!finished)

             std::vector<CharLocation*> finalLocations=new<CharLocation*>[];
             int numLines = lines.size();
             float bottom  = currentY + mLineHeight;
             int yOffset = 0;

            if (vAlign == VAlign::BOTTOM)      yOffset =  containerHeight - bottom;
            else if (vAlign == VAlign::CENTER) yOffset = (containerHeight - bottom) / 2;

            for ( int lineID=0; lineID<numLines; ++lineID)
            {
                 std::vector<CharLocation*> line=lines[lineID];
                numChars = line.size();

                if (numChars == 0) continue;

                 int xOffset = 0;
                 CharLocation* lastLocation= line[line.size()-1];
                 float right  = lastLocation->x() - lastLocation->char->xOffset()
                                                  + lastLocation->char->xAdvance();

                if (hAlign == HAlign::RIGHT)       xOffset =  containerWidth - right;
                else if (hAlign == HAlign::CENTER) xOffset = (containerWidth - right) / 2;

                for ( int c=0; c<numChars; ++c)
                {
                    charLocation = line[c];
                    charLocation->x ( scale * (charLocation->x() + xOffset));
                    charLocation->y ( scale * (charLocation->y() + yOffset));
                    charLocation->scale ( scale);

                    if (charLocation->char->width > 0 && charLocation->char->height() > 0)
                        finalLocations.push_back(charLocation);

                    // return to pool for next call to "arrangeChars"
                    mCharLocationPool.push_back(charLocation);
                }
            }

            return finalLocations;
        }

        /** The name of the font as it was parsed from the font file. */
        std::string BitmapFont::name()        { return mName; }

        /** The native size of the font. */
        float BitmapFont::size()        { return mSize; }

        /** The height of one line in pixels. */
        float BitmapFont::lineHeight()        { return mLineHeight; }
        void BitmapFont::lineHeight(float value)      { mLineHeight = value; }

        /** The smoothing filter that is used for the texture. */
        std::string BitmapFont::smoothing()        { return mHelperImage->smoothing(); }
        void BitmapFont::smoothing(std::string value)      { mHelperImage->smoothing ( value);}

        /** The baseline of the font. */
        float BitmapFont::baseline()        { return mBaseline; }
}
}





               
               
               
               

    CharLocation::CharLocation(BitmapChar* char)
    {
        this()->char = char;
    }
