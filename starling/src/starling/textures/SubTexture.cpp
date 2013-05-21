// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "SubTexture.h"
#include "flash/display3D/textures/TextureBase.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/utils/VertexData.h"

    /** A SubTexture represents a section of another texture. This is achieved solely by 
     *  manipulation of texture coordinates, making the class very efficient. 
     *
     *  <p><em>Note that it is OK to create subtextures of subtextures.</em></p>
     */

using namespace flash::display3D::textures;
using namespace flash::geom;
using namespace starling::utils;

namespace starling {
namespace textures {


                    
                    
                    
                    

        /** Helper object. */
         Point* SubTexture::sTexCoords= new Point();

        /** Creates a new subtexture containing the specified region (in points) of a parent 
         *  texture. If 'ownsParent' is true, the parent texture will be disposed automatically
         *  when the subtexture is disposed. */
        SubTexture::SubTexture(Texture* parentTexture, Rectangle* region,
                                   bool ownsParent)
        {
            mParent = parentTexture;
            mOwnsParent = ownsParent;

            if (region == NULL) setClipping(new Rectangle(0, 0, 1, 1));
            else setClipping(new Rectangle(region->x() / parentTexture->width(),
                                           region->y() / parentTexture->height(),
                                           region->width() / parentTexture->width(),
                                           region->height() / parentTexture->height()));
        }

        /** Disposes the parent texture if this texture owns it. */
        void SubTexture::dispose()
        {
            if (mOwnsParent) mParent->dispose();
            super()->dispose();
        }

        void SubTexture::setClipping(Rectangle* value)
        {
            mClipping = value;
            mRootClipping = value->clone();

             SubTexture* parentTexture= dynamic_cast<SubTexture*>(mParent);
            while (parentTexture)
            {
                 Rectangle* parentClipping= parentTexture->mClipping;
                mRootClipping->x ( parentClipping->x() + mRootClipping->x() * parentClipping->width());
                mRootClipping->y ( parentClipping->y() + mRootClipping->y() * parentClipping->height());
                mRootClipping->width()  *= parentClipping->width();
                mRootClipping->height() *= parentClipping->height();
                parentTexture = parentTexture->dynamic_cast<SubTexture*>(mParent);
            }
        }

        /** @inheritDoc */
        void SubTexture::adjustVertexData(VertexData* vertexData, int vertexID, int count)
        {
            super()->adjustVertexData(vertexData, vertexID, count);

             float clipX  = mRootClipping->x();
             float clipY  = mRootClipping->y();
             float clipWidth   = mRootClipping->width();
             float clipHeight  = mRootClipping->height();
             int endIndex = vertexID + count;

            for ( int i=vertexID; i<endIndex; ++i)
            {
                vertexData->getTexCoords(i, sTexCoords);
                vertexData->setTexCoords(i, clipX + sTexCoords->x() * clipWidth,
                                           clipY + sTexCoords->y() * clipHeight);
            }
        }

        /** The texture which the subtexture is based on. */
        Texture* SubTexture::parent()         { return mParent; }

        /** Indicates if the parent texture is disposed when this object is disposed. */
        bool SubTexture::ownsParent()         { return mOwnsParent; }

        /** The clipping rectangle, which is the region provided on initialization 
         *  scaled into [0.0, 1.0]. */
        Rectangle* SubTexture::clipping()           { return mClipping->clone(); }

        /** @inheritDoc */
        TextureBase* SubTexture::base()             { return mParent->base(); }

        /** @inheritDoc */
        ConcreteTexture* SubTexture::root()                 { return mParent->root(); }

        /** @inheritDoc */
        std::string SubTexture::format()        { return mParent->format(); }

        /** @inheritDoc */
        float SubTexture::width()        { return mParent->width() * mClipping->width(); }

        /** @inheritDoc */
        float SubTexture::height()        { return mParent->height() * mClipping->height(); }

        /** @inheritDoc */
        float SubTexture::nativeWidth()        { return mParent->nativeWidth() * mClipping->width(); }

        /** @inheritDoc */
        float SubTexture::nativeHeight()        { return mParent->nativeHeight() * mClipping->height(); }

        /** @inheritDoc */
        bool SubTexture::mipMapping()         { return mParent->mipMapping(); }

        /** @inheritDoc */
        bool SubTexture::premultipliedAlpha()         { return mParent->premultipliedAlpha(); }

        /** @inheritDoc */
        float SubTexture::scale()        { return mParent->scale(); }
}
}

