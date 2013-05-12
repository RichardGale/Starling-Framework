// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Image.h"
#include "flash/display/Bitmap.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/core/RenderSupport.h"
#include "starling/textures/Texture.h"
#include "starling/textures/TextureSmoothing.h"
#include "starling/utils/VertexData.h"

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
using namespace flash::geom;
using namespace starling::core;
using namespace starling::textures;
using namespace starling::textures;
using namespace starling::utils;

namespace starling {
namespace display {


        /** Creates a quad with a texture mapped onto it. */
        Image::Image(Texture* texture)
        {
            if (texture)
            {
                 Rectangle* frame=texture->frame;
                 float width  = frame ? frame->width : texture->width;
                 float height = frame ? frame->height: texture->height;
                 bool pma   = texture->premultipliedAlpha;

                super(width, height, 0xffffff, pma);

                mVertexData->setTexCoords(0,0.0, 0.0);
                mVertexData->setTexCoords(1,1.0, 0.0);
                mVertexData->setTexCoords(2,0.0, 1.0);
                mVertexData->setTexCoords(3,1.0, 1.0);

                mTexture = texture;
                mSmoothing = TextureSmoothing::BILINEAR;
                mVertexDataCache = new VertexData(4, pma);
                mVertexDataCacheInvalid = true;
            }
            else
            {
                throw new ArgumentError("Texture cannot be null");
            }
        }

        /** Creates an Image with a texture that is created from a bitmap object. */
        Image* Image::fromBitmap(Bitmap* bitmap, bool generateMipMaps,
                                          float scale)
        {
            return new Image(Texture::fromBitmap(bitmap,generateMipMaps, false, scale));
        }

        /** @inheritDoc */
        void Image::onVertexDataChanged()
        {
            mVertexDataCacheInvalid = true;
        }

        /** Readjusts the dimensions of the image according to its current texture. Call this method 
         *  to synchronize image and texture size after assigning a texture with a different size.*/
        void Image::readjustSize()
        {
             Rectangle* frame=texture->frame;
             float width  = frame ? frame->width : texture->width;
             float height = frame ? frame->height: texture->height;

            mVertexData->setPosition(0,0.0, 0.0);
            mVertexData->setPosition(1,width, 0.0);
            mVertexData->setPosition(2,0.0, height);
            mVertexData->setPosition(3,width, height);

            onVertexDataChanged();
        }

        /** Sets the texture coordinates of a vertex. Coordinates are in the range [0, 1]. */
        void Image::setTexCoords(int vertexID, Point* coords)
        {
            mVertexData->setTexCoords(vertexID,coords->x,coords->y);
            onVertexDataChanged();
        }

        /** Gets the texture coordinates of a vertex. Coordinates are in the range [0, 1]. 
         *  If you pass a 'resultPoint', the result will be stored in this point instead of 
         *  creating a new object.*/
        Point* Image::getTexCoords(int vertexID, Point* resultPoint)
        {
            if (resultPoint == NULL) resultPoint = new Point();
            mVertexData->getTexCoords(vertexID,resultPoint);
            return resultPoint;
        }

        /** Copies the raw vertex data to a VertexData instance.
         *  The texture coordinates are already in the format required for rendering. */
        void Image::copyVertexDataTo(VertexData* targetData, int targetVertexID)
        {
            if (mVertexDataCacheInvalid)
            {
                mVertexDataCacheInvalid = false;
                mVertexData->copyTo(mVertexDataCache);
                mTexture->adjustVertexData(mVertexDataCache,0, 4);
            }

            mVertexDataCache->copyTo(targetData,targetVertexID);
        }

        /** The texture that is displayed on the quad. */
        Texture* Image::texture()         { return mTexture; }
        void Image::texture(Texture* value)
        {
            if (value == NULL)
            {
                throw new ArgumentError("Texture cannot be null");
            }
            else if (value != mTexture)
            {
                mTexture = value;
                mVertexData->setPremultipliedAlpha(mTexture->premultipliedAlpha);
                mVertexDataCache->setPremultipliedAlpha(mTexture->premultipliedAlpha,false);
                onVertexDataChanged();
            }
        }

        /** The smoothing filter that is used for the texture. 
        *   @default bilinear
        *   @see starling.textures.TextureSmoothing */
        std::string Image::smoothing()        { return mSmoothing; }
        void Image::smoothing(std::string value)
        {
            if (TextureSmoothing::isValid(value))
                mSmoothing = value;
            else
                throw new ArgumentError("Invalid smoothing mode: " + value);
        }

        /** @inheritDoc */
        void Image::render(RenderSupport* support, float parentAlpha)
        {
            support->batchQuad(this,parentAlpha, mTexture, mSmoothing);
        }
}
}

