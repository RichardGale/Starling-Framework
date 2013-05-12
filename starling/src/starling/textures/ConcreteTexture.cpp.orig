// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "ConcreteTexture.h"
#include "flash/display/BitmapData.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DTextureFormat.h"
#include "flash/display3D/textures/TextureBase.h"

#include "starling/core/Starling.h"
#include "starling/events/Event.h"

    /** A ConcreteTexture wraps a Stage3D texture object, storing the properties of the texture. */
using namespace flash::display;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D::textures;
using namespace starling::core;
using namespace starling::events;

namespace starling {
namespace textures {


        /** Creates a ConcreteTexture object from a TextureBase, storing information about size,
         *  mip-mapping, and if the channels contain premultiplied alpha values. */
        ConcreteTexture::ConcreteTexture(TextureBase* base, std::string format, int width, int height,
                                        bool mipMapping, bool premultipliedAlpha,
                                        bool optimizedForRenderTexture,
                                        float scale)
        {
            mScale = scale <= 0 ? 1.0 : scale;
            mBase = base;
            mFormat = format;
            mWidth = width;
            mHeight = height;
            mMipMapping = mipMapping;
            mPremultipliedAlpha = premultipliedAlpha;
            mOptimizedForRenderTexture = optimizedForRenderTexture;
        }

        /** Disposes the TextureBase object. */
        void ConcreteTexture::dispose()
        {
            if (mBase) mBase->dispose();
            restoreOnLostContext(NULL); // removes event listener & data reference 
            super->dispose();
        }

        // texture backup (context lost)

        /** Instructs this instance to restore its base texture when the context is lost. 'data' 
         *  can be either BitmapData or a ByteArray with ATF data. */
        void ConcreteTexture::restoreOnLostContext(Object* data)
        {
            if (mData == NULL && data != NULL)
                Starling::current->addEventListener(Event::CONTEXT3D_CREATE,onContextCreated);
            else if (data == NULL)
                Starling::current->removeEventListener(Event::CONTEXT3D_CREATE,onContextCreated);

            mData = data;
        }

        void ConcreteTexture::onContextCreated(Event* event)
        {
             Context3D* context=Starling::context;
             BitmapData* bitmapData=static_cast<BitmapData>(mData);
             AtfData* atfData=static_cast<AtfData>(mData);
             flash::display3D::textures::Texture* nativeTexture;

            if (bitmapData)
            {
                nativeTexture = context->createTexture(mWidth,mHeight,
                    Context3DTextureFormat::BGRA,mOptimizedForRenderTexture);
                Texture->uploadBitmapData(nativeTexture,bitmapData, mMipMapping);
            }
            else if (atfData)
            {
                nativeTexture = context->createTexture(atfData->width,atfData->height,atfData->format,
                                                      mOptimizedForRenderTexture);
                Texture->uploadAtfData(nativeTexture,atfData->data);
            }

            mBase = nativeTexture;
        }

        // properties

        /** Indicates if the base texture was optimized for being used in a render texture. */
        bool ConcreteTexture::optimizedForRenderTexture()         { return mOptimizedForRenderTexture; }

        /** @inheritDoc */
        TextureBase* ConcreteTexture::base()             { return mBase; }

        /** @inheritDoc */
        ConcreteTexture* ConcreteTexture::root()                 { return this; }

        /** @inheritDoc */
        std::string ConcreteTexture::format()        { return mFormat; }

        /** @inheritDoc */
        float ConcreteTexture::width()         { return mWidth / mScale;  }

        /** @inheritDoc */
        float ConcreteTexture::height()        { return mHeight / mScale; }

        /** @inheritDoc */
        float ConcreteTexture::nativeWidth()        { return mWidth; }

        /** @inheritDoc */
        float ConcreteTexture::nativeHeight()        { return mHeight; }

        /** The scale factor, which influences width and height properties. */
        float ConcreteTexture::scale()        { return mScale; }

        /** @inheritDoc */
        bool ConcreteTexture::mipMapping()         { return mMipMapping; }

        /** @inheritDoc */
        bool ConcreteTexture::premultipliedAlpha()         { return mPremultipliedAlpha; }
}
}

