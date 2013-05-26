#if !defined(__STARLING_SRC_STARLING_DISPLAY_IMAGE_AS)
#define __STARLING_SRC_STARLING_DISPLAY_IMAGE_AS
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

namespace starling
{
    namespace core
    {
        class RenderSupport;
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
        class VertexData;
    }
}
#include "starling/display/Quad.h"

/** An Image is a quad with a texture mapped onto it.
 *
 *  <p>The Image class is the Starling equivalent of Flash's Bitmap class. Instead of
 *  BitmapData, Starling uses textures to represent the pixels of an image. To display a
 *  texture, you have to map it onto a quad - and that's what the Image class is for.</p>
 *
 *  <p>As "Image" inherits from "Quad", you can give it a color. For each pixel, the resulting
 *  color will be the result of the multiplication of the color of the texture with the color of
 *  the quad. That way, you can easily tint textures with a certain color. Furthermore, images
 *  allow the manipulation of texture coordinates. That way, you can move a texture inside an
 *  image without changing any vertex coordinates of the quad. You can also use this feature
 *  as a very efficient way to create a rectangular mask.</p>
 *
 *  @see starling.textures.Texture
 *  @see Quad
 */

using namespace flash::display;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace display
    {
        class Image : public starling::display::Quad
        {
        private:
            starling::textures::Texture *mTexture;
        private:
            std::string mSmoothing;

        private:
            VertexData *mVertexDataCache;
        private:
            bool mVertexDataCacheInvalid;

            /** Creates a quad with a texture mapped onto it. */
        public:
            Image(starling::textures::Texture *texture);

            /** Creates an Image with a texture that is created from a bitmap object. */
        public:
            static Image    *fromBitmap(Bitmap *bitmap, bool generateMipMaps=true,
                                        float scale=1);

            /** @inheritDoc */
        protected:
            virtual void     onVertexDataChanged();

            /** Readjusts the dimensions of the image according to its current texture. Call this method
             *  to synchronize image and texture size after assigning a texture with a different size.*/
        public:
            void     readjustSize();

            /** Sets the texture coordinates of a vertex. Coordinates are in the range [0, 1]. */
        public:
            void     setTexCoords(int vertexID, Point *coords);

            /** Gets the texture coordinates of a vertex. Coordinates are in the range [0, 1].
             *  If you pass a 'resultPoint', the result will be stored in this point instead of
             *  creating a new object.*/
        public:
            Point    *getTexCoords(int vertexID, Point *resultPoint=NULL);

            /** Copies the raw vertex data to a VertexData instance.
             *  The texture coordinates are already in the format required for rendering. */
        public:
            virtual void     copyVertexDataTo(VertexData *targetData, int targetVertexID=0);

            /** The texture that is displayed on the quad. */
        public:
            Texture      *texture();
        public:
            void         texture(Texture *value);

            /** The smoothing filter that is used for the texture.
            *   @default bilinear
            *   @see starling.textures.TextureSmoothing */
        public:
            std::string       smoothing();
        public:
            void         smoothing(std::string value);

            /** @inheritDoc */
        public:
            virtual void     render(RenderSupport *support, float parentAlpha);
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_IMAGE_AS
#endif // __cplusplus

