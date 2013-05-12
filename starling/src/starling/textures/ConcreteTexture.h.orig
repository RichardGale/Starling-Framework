#if !defined(__STARLING_SRC_STARLING_TEXTURES_CONCRETETEXTURE_AS)
#define __STARLING_SRC_STARLING_TEXTURES_CONCRETETEXTURE_AS
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
namespace flash { namespace display { class BitmapData; } }
namespace flash { namespace display3D { class Context3D; } }
namespace flash { namespace display3D { class Context3DTextureFormat; } }
namespace flash { namespace display3D { namespace textures { class TextureBase; } } }

namespace starling { namespace core { class Starling; } }
namespace starling { namespace events { class Event; } }

    /** A ConcreteTexture wraps a Stage3D texture object, storing the properties of the texture. */
using namespace flash::display;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D::textures;
using namespace starling::core;
using namespace starling::events;

namespace starling {
namespace textures {
    class ConcreteTexture: public Texture
    {
        private:  TextureBase* mBase;
        private:  std::string mFormat;
        private:  int mWidth;
        private:  int mHeight;
        private:  bool mMipMapping;
        private:  bool mPremultipliedAlpha;
        private:  bool mOptimizedForRenderTexture;
        private:  Object* mData;
        private:  float mScale;

        /** Creates a ConcreteTexture object from a TextureBase, storing information about size,
         *  mip-mapping, and if the channels contain premultiplied alpha values. */
        public:          ConcreteTexture(TextureBase* base, std::string format, int width, int height,
                                        bool mipMapping, bool premultipliedAlpha,
                                        bool optimizedForRenderTexture   =false,
                                        float scale =1);

        /** Disposes the TextureBase object. */
        public: virtual void     dispose();

        // texture backup (context lost)

        /** Instructs this instance to restore its base texture when the context is lost. 'data' 
         *  can be either BitmapData or a ByteArray with ATF data. */
        public: void     restoreOnLostContext(Object* data);

        private: void     onContextCreated(Event* event);

        // properties

        /** Indicates if the base texture was optimized for being used in a render texture. */
        public: bool         optimizedForRenderTexture();

        /** @inheritDoc */
        public: virtual TextureBase* base();

        /** @inheritDoc */
        public: virtual ConcreteTexture* root();

        /** @inheritDoc */
        public: virtual std::string  format();

        /** @inheritDoc */
        public: virtual float        width();

        /** @inheritDoc */
        public: virtual float        height();

        /** @inheritDoc */
        public: virtual float        nativeWidth();

        /** @inheritDoc */
        public: virtual float        nativeHeight();

        /** The scale factor, which influences width and height properties. */
        public: virtual float        scale();

        /** @inheritDoc */
        public: virtual bool         mipMapping();

        /** @inheritDoc */
        public: virtual bool         premultipliedAlpha();
    };
}
}

#endif // __STARLING_SRC_STARLING_TEXTURES_CONCRETETEXTURE_AS
#endif // __cplusplus

