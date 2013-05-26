#if !defined(__STARLING_SRC_STARLING_TEXTURES_TEXTURE_AS)
#define __STARLING_SRC_STARLING_TEXTURES_TEXTURE_AS
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
        class Bitmap;
    }
}
namespace flash
{
    namespace display
    {
        class BitmapData;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3DTextureFormat;
    }
}
namespace flash
{
    namespace display3D
    {
        namespace textures
        {
            class Texture;
        }
    }
}
namespace flash
{
    namespace display3D
    {
        namespace textures
        {
            class TextureBase;
        }
    }
}
namespace flash
{
    namespace display3D
    {
        namespace textures
        {
            class ConcreteTexture;
        }
    }
}
namespace flash
{
    namespace events
    {
        class Event;
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
        class Point;
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
    namespace system
    {
        class Capabilities;
    }
}
namespace flash
{
    namespace utils
    {
        class ByteArray;
    }
}
namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
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
    namespace errors
    {
        class AbstractClassError;
    }
}
namespace starling
{
    namespace errors
    {
        class MissingContextError;
    }
}
namespace starling
{
    namespace utils
    {
        class VertexData;
    }
}
namespace starling
{
    namespace utils
    {
        class getNextPowerOfTwo;
    }
}

/** <p>A texture stores the information that represents an image. It cannot be added to the
 *  display list directly; instead it has to be mapped onto a display object. In Starling,
 *  that display object is the class "Image".</p>
 *
 *  <strong>Texture Formats</strong>
 *
 *  <p>Since textures can be created from a "BitmapData" object, Starling supports any bitmap
 *  format that is supported by Flash. And since you can render any Flash display object into
 *  a BitmapData object, you can use this to display non-Starling content in Starling - e.g.
 *  Shape objects.</p>
 *
 *  <p>Starling also supports ATF textures (Adobe Texture Format), which is a container for
 *  compressed texture formats that can be rendered very efficiently by the GPU. Refer to
 *  the Flash documentation for more information about this format.</p>
 *
 *  <strong>Mip Mapping</strong>
 *
 *  <p>MipMaps are scaled down versions of a texture. When an image is displayed smaller than
 *  its natural size, the GPU may display the mip maps instead of the original texture. This
 *  reduces aliasing and accelerates rendering. It does, however, also need additional memory;
 *  for that reason, you can choose if you want to create them or not.</p>
 *
 *  <strong>Texture Frame</strong>
 *
 *  <p>The frame property of a texture allows you let a texture appear inside the bounds of an
 *  image, leaving a transparent space around the texture. The frame rectangle is specified in
 *  the coordinate system of the texture (not the image):</p>
 *
 *  <listing>
 *  var frame:Rectangle = new Rectangle(-10, -10, 30, 30);
 *  var texture:Texture = Texture.fromTexture(anotherTexture, null, frame);
 *  var image:Image = new Image(texture);</listing>
 *
 *  <p>This code would create an image with a size of 30x30, with the texture placed at
 *  <code>x=10, y=10</code> within that image (assuming that 'anotherTexture' has a width and
 *  height of 10 pixels, it would appear in the middle of the image).</p>
 *
 *  <p>The texture atlas makes use of this feature, as it allows to crop transparent edges
 *  of a texture and making up for the changed size by specifying the original texture frame.
 *  Tools like <a href="http://www.texturepacker.com/">TexturePacker</a> use this to
 *  optimize the atlas.</p>
 *
 *  <strong>Texture Coordinates</strong>
 *
 *  <p>If, on the other hand, you want to show only a part of the texture in an image
 *  (i.e. to crop the the texture), you can either create a subtexture (with the method
 *  'Texture.fromTexture()' and specifying a rectangle for the region), or you can manipulate
 *  the texture coordinates of the image object. The method 'image.setTexCoords' allows you
 *  to do that.</p>
 *
 *  @see starling.display.Image
 *  @see TextureAtlas
 */

using namespace flash::display;
using namespace flash::display3D;
using namespace flash::display3D::textures;
using namespace flash::events;
using namespace flash::geom;
using namespace flash::system;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::errors;
using namespace starling::utils;

namespace starling
{
    namespace textures
    {
        class Texture
        {
        private:
            Rectangle *mFrame;
        private:
            bool mRepeat;

            /** helper object */
        private:
            static Point *sOrigin;

            /** @private */
        public:
            Texture();

            /** Disposes the underlying texture data. Note that not all textures need to be disposed:
             *  SubTextures (created with 'Texture.fromTexture') just reference other textures and
             *  and do not take up resources themselves; this is also true for textures from an
             *  atlas. */
        public:
            void     dispose();

            /** Creates a texture object from a bitmap.
             *  Beware: you must not dispose 'data' if Starling should handle a lost device context. */
        public:
            static Texture  *fromBitmap(Bitmap *data, bool generateMipMaps=true,
                                        bool optimizeForRenderToTexture=false,
                                        float scale=1);

            /** Creates a texture from bitmap data.
             *  Beware: you must not dispose 'data' if Starling should handle a lost device context. */
        public:
            static Texture  *fromBitmapData(BitmapData *data, bool generateMipMaps=true,
                                            bool optimizeForRenderToTexture=false,
                                            float scale=1);

            /** Creates a texture from the compressed ATF format. If you don't want to use any embedded
             *  mipmaps, you can disable them by setting "useMipMaps" to <code>false</code>.
             *  Beware: you must not dispose 'data' if Starling should handle a lost device context.
             *
             *  <p>If you pass a function for the 'loadAsync' parameter, the method will return
             *  immediately, while the texture will be created asynchronously. It can be used as soon
             *  as the callback has been executed. This is the expected function definition:
             *  <code>function(texture:Texture):void;</code></p> */
        public:
            static Texture  *fromAtfData(ByteArray *data, float scale=1, bool useMipMaps=true,
                                         Function *loadAsync=NULL);

            /** Creates a texture with a certain size and color.
             *
             *  @param width:  in points; number of pixels depends on scale parameter
             *  @param height: in points; number of pixels depends on scale parameter
             *  @param color:  expected in ARGB format (inlude alpha!)
             *  @param optimizeForRenderToTexture: indicates if this texture will be used as render target
             *  @param scale:  if you omit this parameter, 'Starling.contentScaleFactor' will be used.
             */
        public:
            static Texture  *fromColor(int width, int height, unsigned int color=0xffffffff,
                                       bool optimizeForRenderToTexture=false,
                                       float scale=-1);

            /** Creates an empty texture of a certain size. Useful mainly for render textures.
             *  Beware that the texture can only be used after you either upload some color data or
             *  clear the texture while it is an active render target.
             *
             *  @param width:  in points; number of pixels depends on scale parameter
             *  @param height: in points; number of pixels depends on scale parameter
             *  @param premultipliedAlpha: the PMA format you will use the texture with
             *  @param optimizeForRenderToTexture: indicates if this texture will be used as render target
             *  @param scale:  if you omit this parameter, 'Starling.contentScaleFactor' will be used.
             */
        public:
            static Texture  *empty(int width=64, int height=64, bool premultipliedAlpha=false,
                                   bool optimizeForRenderToTexture=true,
                                   float scale=-1);

            /** Creates a texture that contains a region (in pixels) of another texture. The new
             *  texture will reference the base texture; no data is duplicated. */
        public:
            static Texture  *fromTexture(Texture *texture, Rectangle *region=NULL, Rectangle *frame=NULL);

            /** Converts texture coordinates and vertex positions of raw vertex data into the format
             *  required for rendering. */
        public:
            void     adjustVertexData(VertexData *vertexData, int vertexID, int count);

            /** @private Uploads the bitmap data to the native texture, optionally creating mipmaps. */
            static void     uploadBitmapData(flash::display3D::textures::Texture *nativeTexture,
                                             BitmapData *data, bool generateMipmaps);

            /** @private Uploads ATF data from a ByteArray to a native texture. */
            static void     uploadAtfData(flash::display3D::textures::Texture *nativeTexture,
                                          ByteArray *data, int offset=0,
                                          bool async=false);

            // properties

            /** The texture frame (see class description). */
        public:
            Rectangle    *frame();

            /** Indicates if the texture should repeat like a wallpaper or stretch the outermost pixels.
             *  Note: this only works in textures with sidelengths that are powers of two and
             *  that are not loaded from a texture atlas (i.e. no subtextures). @default false */
        public:
            bool      repeat();
        public:
            void         repeat(bool value);

            /** The width of the texture in points. */
        public:
            float       width();

            /** The height of the texture in points. */
        public:
            float       height();

            /** The width of the texture in pixels (without scale adjustment). */
        public:
            float       nativeWidth();

            /** The height of the texture in pixels (without scale adjustment). */
        public:
            float       nativeHeight();

            /** The scale factor, which influences width and height properties. */
        public:
            float       scale();

            /** The Stage3D texture object the texture is based on. */
        public:
            TextureBase  *base();

            /** The concrete (power-of-two) texture the texture is based on. */
        public:
            ConcreteTexture *root();

            /** The <code>Context3DTextureFormat</code> of the underlying texture data. */
        public:
            std::string       format();

            /** Indicates if the texture contains mip maps. */
        public:
            bool      mipMapping();

            /** Indicates if the alpha values are premultiplied into the RGB values. */
        public:
            bool      premultipliedAlpha();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXTURES_TEXTURE_AS
#endif // __cplusplus

