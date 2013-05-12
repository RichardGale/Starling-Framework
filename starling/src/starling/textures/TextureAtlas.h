#if !defined(__STARLING_SRC_STARLING_TEXTURES_TEXTUREATLAS_AS)
#define __STARLING_SRC_STARLING_TEXTURES_TEXTUREATLAS_AS
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

namespace starling
{
    namespace textures
    {
        class TextureAtlas
        {
        private:
            Texture *mAtlasTexture;
        private:
            std::map<std::string, void *> mTextureRegions;
        private:
            std::map<std::string, void *> mTextureFrames;

            /** helper objects */
        private:
            std::vector<std::string> *sNames;

            /** Create a texture atlas from a texture by parsing the regions from an XML file. */
        public:
            TextureAtlas(Texture *texture, XML *atlasXml=NULL);

            /** Disposes the atlas texture. */
        public:
            void     dispose();

            /** This function is called by the constructor and will parse an XML in Starling's
             *  default atlas file format. Override this method to create custom parsing logic
             *  (e.g. to support a different file format). */
        protected:
            void     parseAtlasXml(XML *atlasXml);

            /** Retrieves a subtexture by name. Returns <code>null</code> if it is not found. */
        public:
            Texture *getTexture(std::string name);

            /** Returns all textures that start with a certain string, sorted alphabetically
             *  (especially useful for "MovieClip"). */
        public:
            std::vector<Texture *> *getTextures(std::string prefix="", std::vector<Texture *> *result=NULL);

            /** Returns all texture names that start with a certain string, sorted alphabetically. */
        public:
            std::vector<std::string> *getNames(std::string prefix="", std::vector<std::string> *result=NULL);

            /** Returns the region rectangle associated with a specific name. */
        public:
            Rectangle *getRegion(std::string name);

            /** Returns the frame rectangle of a specific region, or <code>null</code> if that region
             *  has no frame. */
        public:
            Rectangle *getFrame(std::string name);

            /** Adds a named region for a subtexture (described by rectangle with coordinates in
             *  pixels) with an optional frame. */
        public:
            void     addRegion(std::string name, Rectangle *region, Rectangle *frame=NULL);

            /** Removes a region with a certain name. */
        public:
            void     removeRegion(std::string name);

            /** The base texture that makes up the atlas. */
        public:
            Texture     *texture();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXTURES_TEXTUREATLAS_AS
#endif // __cplusplus

