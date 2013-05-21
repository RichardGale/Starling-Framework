// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Texture.h"
#include "flash/display/Bitmap.h"
#include "flash/display/BitmapData.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DTextureFormat.h"
#include "flash/display3D/textures/Texture.h"
#include "flash/display3D/textures/TextureBase.h"
#include "flash/display3D/textures/ConcreteTexture.h"
#include "flash/events/Event.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"
#include "flash/utils/ByteArray.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/Starling.h"
#include "starling/errors/AbstractClassError.h"
#include "starling/errors/MissingContextError.h"
#include "starling/utils/VertexData.h"
#include "starling/utils/getNextPowerOfTwo.h"

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

namespace starling {
namespace textures {


                    
                    

        /** helper object */
         Point* Texture::sOrigin= new Point();

        /** @private */
        Texture::Texture()
        {
            if (Capabilities::isDebugger() &&
                getQualifiedClassName(this) == "starling.textures::Texture")
            {
                throw new AbstractClassError();
            }

            mRepeat = false;
        }

        /** Disposes the underlying texture data. Note that not all textures need to be disposed: 
         *  SubTextures (created with 'Texture.fromTexture') just reference other textures and
         *  and do not take up resources themselves; this is also true for textures from an 
         *  atlas. */
        void Texture::dispose()
        {
            // override in subclasses
        }

        /** Creates a texture object from a bitmap.
         *  Beware: you must not dispose 'data' if Starling should handle a lost device context. */
        Texture* Texture::fromBitmap(Bitmap* data, bool generateMipMaps,
                                          bool optimizeForRenderToTexture,
                                          float scale)
        {
            return fromBitmapData(data->bitmapData(), generateMipMaps, optimizeForRenderToTexture, scale);
        }

        /** Creates a texture from bitmap data. 
         *  Beware: you must not dispose 'data' if Starling should handle a lost device context. */
        Texture* Texture::fromBitmapData(BitmapData* data, bool generateMipMaps,
                                              bool optimizeForRenderToTexture,
                                              float scale)
        {
             int origWidth   = data->width();
             int origHeight  = data->height();
             int legalWidth  = getNextPowerOfTwo(origWidth);
             int legalHeight = getNextPowerOfTwo(origHeight);
             Context3D* context= Starling::context;
             BitmapData* potData;

            if (context == NULL) throw new MissingContextError();

             flash::display3D::textures::Texture* nativeTexture=context->createTexture(
                legalWidth, legalHeight, Context3DTextureFormat::BGRA, optimizeForRenderToTexture);

            if (legalWidth > origWidth || legalHeight > origHeight)
            {
                potData = new BitmapData(legalWidth, legalHeight, true, 0);
                potData->copyPixels(data, data->rect(), sOrigin);
                data = potData;
            }

            uploadBitmapData(nativeTexture, data, generateMipMaps);

             ConcreteTexture* concreteTexture= new ConcreteTexture(
                nativeTexture, Context3DTextureFormat::BGRA, legalWidth, legalHeight,
                generateMipMaps, true, optimizeForRenderToTexture, scale);

            if (Starling::handleLostContext)
                concreteTexture->restoreOnLostContext(data);
            else if (potData)
                potData->dispose();

            if (origWidth == legalWidth && origHeight == legalHeight)
                return concreteTexture;
            else
                return new SubTexture(concreteTexture,
                                      new Rectangle(0, 0, origWidth/scale, origHeight/scale),
                                      true);
        }

        /** Creates a texture from the compressed ATF format. If you don't want to use any embedded
         *  mipmaps, you can disable them by setting "useMipMaps" to <code>false</code>.
         *  Beware: you must not dispose 'data' if Starling should handle a lost device context.
         *  
         *  <p>If you pass a function for the 'loadAsync' parameter, the method will return
         *  immediately, while the texture will be created asynchronously. It can be used as soon
         *  as the callback has been executed. This is the expected function definition:
         *  <code>function(texture:Texture):void;</code></p> */
        Texture* Texture::fromAtfData(ByteArray* data, float scale, bool useMipMaps,
                                           Function* loadAsync)
        {
            const std::string eventType="textureReady";// defined here for backwards compatibility

             Context3D* context= Starling::context;
            if (context == NULL) throw new MissingContextError();

             bool async    = loadAsync != NULL;
             AtfData* atfData= new AtfData(data);
             flash::display3D::textures::Texture* nativeTexture=context->createTexture(
                    atfData->width(), atfData->height(), atfData->format(), false);

            uploadAtfData(nativeTexture, data, 0, async);

             ConcreteTexture* concreteTexture= new ConcreteTexture(nativeTexture, atfData->format(),
                atfData->width(), atfData->height(), useMipMaps && atfData->numTextures() > 1,
                false, false, scale);

            if (Starling::handleLostContext)
                concreteTexture->restoreOnLostContext(atfData);

            if (async)
                nativeTexture->addEventListener(eventType, onTextureReady);

            return concreteTexture;void Texture::(Event* event)
            {
                nativeTexture->removeEventListener(eventType, onTextureReady);
                if (loadAsync->length() == 1) loadAsync(concreteTexture);
                else loadAsync();
            }
        }

        /** Creates a texture with a certain size and color.
         *  
         *  @param width:  in points; number of pixels depends on scale parameter
         *  @param height: in points; number of pixels depends on scale parameter
         *  @param color:  expected in ARGB format (inlude alpha!)
         *  @param optimizeForRenderToTexture: indicates if this texture will be used as render target
         *  @param scale:  if you omit this parameter, 'Starling.contentScaleFactor' will be used.
         */
        Texture* Texture::fromColor(int width, int height, unsigned int color,
                                         bool optimizeForRenderToTexture,
                                         float scale)
        {
            if (scale <= 0) scale = Starling::contentScaleFactor;

             BitmapData* bitmapData= new BitmapData(width*scale, height*scale, true, color);
             Texture* texture= fromBitmapData(bitmapData, false, optimizeForRenderToTexture, scale);

            if (!Starling::handleLostContext)
                bitmapData->dispose();

            return texture;
        }

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
        Texture* Texture::empty(int width, int height, bool premultipliedAlpha,
                                     bool optimizeForRenderToTexture,
                                     float scale)
        {
            if (scale <= 0) scale = Starling::contentScaleFactor;

             int origWidth  = width * scale;
             int origHeight = height * scale;
             int legalWidth  = getNextPowerOfTwo(origWidth);
             int legalHeight = getNextPowerOfTwo(origHeight);
             std::string format=Context3DTextureFormat::BGRA;
             Context3D* context= Starling::context;

            if (context == NULL) throw new MissingContextError();

             flash::display3D::textures::Texture* nativeTexture=context->createTexture(
                legalWidth, legalHeight, Context3DTextureFormat::BGRA, optimizeForRenderToTexture);

             ConcreteTexture* concreteTexture= new ConcreteTexture(nativeTexture, format,
                legalWidth, legalHeight, false, premultipliedAlpha, optimizeForRenderToTexture, scale);

            if (origWidth == legalWidth && origHeight == legalHeight)
                return concreteTexture;
            else
                return new SubTexture(concreteTexture, new Rectangle(0, 0, width, height), true);
        }

        /** Creates a texture that contains a region (in pixels) of another texture. The new
         *  texture will reference the base texture; no data is duplicated. */
        Texture* Texture::fromTexture(Texture* texture, Rectangle* region, Rectangle* frame)
        {
             Texture* subTexture= new SubTexture(texture, region);
            subTexture->mFrame ( frame);
            return subTexture;
        }

        /** Converts texture coordinates and vertex positions of raw vertex data into the format 
         *  required for rendering. */
        void Texture::adjustVertexData(VertexData* vertexData, int vertexID, int count)
        {
            if (mFrame)
            {
                if (count != 4)
                    throw new ArgumentError("Textures with a frame can only be used on quads");

                 float deltaRight   = mFrame->width()  + mFrame->x() - width;
                 float deltaBottom  = mFrame->height() + mFrame->y() - height;

                vertexData->translateVertex(vertexID,     -mFrame->x(), -mFrame->y());
                vertexData->translateVertex(vertexID + 1, -deltaRight, -mFrame->y());
                vertexData->translateVertex(vertexID + 2, -mFrame->x(), -deltaBottom);
                vertexData->translateVertex(vertexID + 3, -deltaRight, -deltaBottom);
            }
        }

        /** @private Uploads the bitmap data to the native texture, optionally creating mipmaps. */
        void Texture::uploadBitmapData(flash::display3D::textures::Texture* nativeTexture,
                                                  BitmapData* data, bool generateMipmaps)
        {
            nativeTexture->uploadFromBitmapData(data);

            if (generateMipmaps && data->width > 1 && data->height() > 1)
            {
                 int currentWidth  = data->width()  >> 1;
                 int currentHeight = data->height() >> 1;
                 int level = 1;
                 BitmapData* canvas= new BitmapData(currentWidth, currentHeight, true, 0);
                 Matrix* transform= new Matrix(.5, 0, 0, .5);
                 Rectangle* bounds= new Rectangle();

                while (currentWidth >= 1 || currentHeight >= 1)
                {
                    bounds->width ( currentWidth);bounds->height ( currentHeight);
                    canvas->fillRect(bounds, 0);
                    canvas->draw(data, transform, NULL, NULL, NULL, true);
                    nativeTexture->uploadFromBitmapData(canvas, level++);
                    transform->scale(0.5, 0.5);
                    currentWidth  = currentWidth  >> 1;
                    currentHeight = currentHeight >> 1;
                }

                canvas->dispose();
            }
        }

        /** @private Uploads ATF data from a ByteArray to a native texture. */
        void Texture::uploadAtfData(flash::display3D::textures::Texture* nativeTexture,
                                               ByteArray* data, int offset,
                                               bool async)
        {
            nativeTexture->uploadCompressedTextureFromByteArray(data, offset, async);
        }

        // properties

        /** The texture frame (see class description). */
        Rectangle* Texture::frame()
        {
            return mFrame ? mFrame->clone() : new Rectangle(0, 0, width, height);

            // the frame property is readonly - set the frame in the 'fromTexture' method.
            // why is it readonly? To be able to efficiently cache the texture coordinates on
            // rendering, textures need to be immutable (except 'repeat', which is not cached,
            // anyway).
        }

        /** Indicates if the texture should repeat like a wallpaper or stretch the outermost pixels.
         *  Note: this only works in textures with sidelengths that are powers of two and 
         *  that are not loaded from a texture atlas (i.e. no subtextures). @default false */
        bool Texture::repeat()         { return mRepeat; }
        void Texture::repeat(bool value)      { mRepeat = value; }

        /** The width of the texture in points. */
        float Texture::width()        { return 0; }

        /** The height of the texture in points. */
        float Texture::height()        { return 0; }

        /** The width of the texture in pixels (without scale adjustment). */
        float Texture::nativeWidth()        { return 0; }

        /** The height of the texture in pixels (without scale adjustment). */
        float Texture::nativeHeight()        { return 0; }

        /** The scale factor, which influences width and height properties. */
        float Texture::scale()        { return 1.0; }

        /** The Stage3D texture object the texture is based on. */
        TextureBase* Texture::base()             { return NULL; }

        /** The concrete (power-of-two) texture the texture is based on. */
        ConcreteTexture* Texture::root()                 { return NULL; }

        /** The <code>Context3DTextureFormat</code> of the underlying texture data. */
        std::string Texture::format()        { return Context3DTextureFormat::BGRA; }

        /** Indicates if the texture contains mip maps. */
        bool Texture::mipMapping()         { return false; }

        /** Indicates if the alpha values are premultiplied into the RGB values. */
        bool Texture::premultipliedAlpha()         { return false; }
}
}

