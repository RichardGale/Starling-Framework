// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextureAtlas.h"
#include "flash/geom/Rectangle.h"
#include "flash/utils/Dictionary.h"

    /** A texture atlas is a collection of many smaller textures in one big image. This class
     *  is used to access textures from such an atlas.
     *  
     *  <p>Using a texture atlas for your textures solves two problems:</p>
     *  
     *  <ul>
     *    <li>Whenever you switch between textures, the batching of image objects is disrupted.</li>
     *    <li>Any Stage3D texture has to have side lengths that are powers of two. Starling hides 
     *        this limitation from you, but at the cost of additional graphics memory.</li>
     *  </ul>
     *  
     *  <p>By using a texture atlas, you avoid both texture switches and the power-of-two 
     *  limitation. All textures are within one big "super-texture", and Starling takes care that 
     *  the correct part of this texture is displayed.</p>
     *  
     *  <p>There are several ways to create a texture atlas. One is to use the atlas generator 
     *  script that is bundled with Starling's sibling, the <a href="http://www.sparrow-framework.org">
     *  Sparrow framework</a>. It was only tested in Mac OS X, though. A great multi-platform 
     *  alternative is the commercial tool <a href="http://www.texturepacker.com">
     *  Texture Packer</a>.</p>
     *  
     *  <p>Whatever tool you use, Starling expects the following file format:</p>
     * 
     *  <listing>
     *  &lt;TextureAtlas imagePath='atlas.png'&gt;
     *    &lt;SubTexture name='texture_1' x='0'  y='0' width='50' height='50'/&gt;
     *    &lt;SubTexture name='texture_2' x='50' y='0' width='20' height='30'/&gt; 
     *  &lt;/TextureAtlas&gt;
     *  </listing>
     *  
     *  <p>If your images have transparent areas at their edges, you can make use of the 
     *  <code>frame</code> property of the Texture class. Trim the texture by removing the 
     *  transparent edges and specify the original texture size like this:</p>
     * 
     *  <listing>
     *  &lt;SubTexture name='trimmed' x='0' y='0' height='10' width='10'
     *      frameX='-10' frameY='-10' frameWidth='30' frameHeight='30'/&gt;
     *  </listing>
     */
using namespace flash::geom;
using namespace flash::utils;

namespace starling {
namespace textures {


        /** helper objects */

        /** Create a texture atlas from a texture by parsing the regions from an XML file. */
        TextureAtlas::TextureAtlas(Texture* texture, XML* atlasXml)
        {
            mTextureRegions.clear();
            mTextureFrames.clear();
            mAtlasTexture   = texture;

            if (atlasXml)
                parseAtlasXml(atlasXml);
        }

        /** Disposes the atlas texture. */
        void TextureAtlas::dispose()
        {
            mAtlasTexture->dispose();
        }

        /** This function is called by the constructor and will parse an XML in Starling's 
         *  default atlas file format. Override this method to create custom parsing logic
         *  (e.g. to support a different file format). */
        void TextureAtlas::parseAtlasXml(XML* atlasXml)
        {
             float scale = mAtlasTexture->scale;

            for each (var XML* subTexturein atlasXml->SubTexture)
            {
                 std::string name  = subTexture->attribute("name");
                 float x           = parseFloat(subTexture->attribute("x"))/ scale;
                 float y           = parseFloat(subTexture->attribute("y"))/ scale;
                 float width       = parseFloat(subTexture->attribute("width"))/ scale;
                 float height      = parseFloat(subTexture->attribute("height"))/ scale;
                 float frameX      = parseFloat(subTexture->attribute("frameX"))/ scale;
                 float frameY      = parseFloat(subTexture->attribute("frameY"))/ scale;
                 float frameWidth  = parseFloat(subTexture->attribute("frameWidth"))/ scale;
                 float frameHeight = parseFloat(subTexture->attribute("frameHeight"))/ scale;

                 Rectangle* region=new Rectangle(x, y, width, height);
                 Rectangle* frame= frameWidth > 0 && frameHeight > 0 ?
                        new Rectangle(frameX, frameY, frameWidth, frameHeight) : NULL;

                addRegion(name, region, frame);
            }
        }

        /** Retrieves a subtexture by name. Returns <code>null</code> if it is not found. */
        Texture* TextureAtlas::getTexture(std::string name)
        {
             Rectangle* region=mTextureRegions[name];

            if (region == NULL) return NULL;
            else return Texture->fromTexture(mAtlasTexture,region, mTextureFrames[name]);
        }

        /** Returns all textures that start with a certain string, sorted alphabetically
         *  (especially useful for "MovieClip"). */
        std::vector<Texture*>* TextureAtlas::getTextures(std::string prefix, std::vector<Texture*>* result)
        {
            if (result == NULL) result = new <Texture*>[];

            for each (var std::string nameingetNames(prefix,sNames))
                result->push(getTexture(name));

            sNames.clear()
            return result;
        }

        /** Returns all texture names that start with a certain string, sorted alphabetically. */
        std::vector<std::string>* TextureAtlas::getNames(std::string prefix, std::vector<std::string>* result)
        {
            if (result == NULL) result = new <std::string>[];

            for ( std::string nameinmTextureRegions)
                if (name->indexOf(prefix)== 0)
                    result->push(name);

            result->sort(Array->CASEINSENSITIVE);
            return result;
        }

        /** Returns the region rectangle associated with a specific name. */
        Rectangle* TextureAtlas::getRegion(std::string name)
        {
            return mTextureRegions[name];
        }

        /** Returns the frame rectangle of a specific region, or <code>null</code> if that region 
         *  has no frame. */
        Rectangle* TextureAtlas::getFrame(std::string name)
        {
            return mTextureFrames[name];
        }

        /** Adds a named region for a subtexture (described by rectangle with coordinates in 
         *  pixels) with an optional frame. */
        void TextureAtlas::addRegion(std::string name, Rectangle* region, Rectangle* frame)
        {
            mTextureRegions[name] = region;
            mTextureFrames[name]  = frame;
        }

        /** Removes a region with a certain name. */
        void TextureAtlas::removeRegion(std::string name)
        {
            delete mTextureRegions[name];
            delete mTextureFrames[name];
        }

        /** The base texture that makes up the atlas. */
        Texture* TextureAtlas::texture()         { return mAtlasTexture; }
}
}

