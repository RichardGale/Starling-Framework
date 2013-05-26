#if !defined(__STARLING_SRC_STARLING_CORE_TOUCHMARKER_AS)
#define __STARLING_SRC_STARLING_CORE_TOUCHMARKER_AS
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
        class BitmapData;
    }
}
namespace flash
{
    namespace display
    {
        class Shape;
    }
}
namespace flash
{
    namespace display
    {
        class Graphics;
    }
}
namespace flash
{
    namespace geom
    {
        class Point;
    }
}

namespace starling
{
    namespace display
    {
        class Image;
    }
}
#include "starling/display/Sprite.h"
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace core
    {
        class starling;
    }
}

/** The TouchMarker is used internally to mark touches created through "simulateMultitouch". */

using namespace flash::display;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::textures;

namespace starling
{
    namespace core
    {
        class TouchMarker : public starling::display::Sprite
        {
        private:
            Point *mCenter;
        private:
            starling::textures::Texture *mTexture;

        public:
            TouchMarker();

        public:
            virtual void     dispose();

        public:
            void     moveMarker(float x, float y, bool withCenter=false);

        public:
            void     moveCenter(float x, float y);

        private:
            starling::textures::Texture *createTexture();

        private:
            Image        *realMarker();
        private:
            Image        *mockMarker();

        public:
            float       realX();
        public:
            float       realY();

        public:
            float       mockX();
        public:
            float       mockY();
        };
    }
}

#endif // __STARLING_SRC_STARLING_CORE_TOUCHMARKER_AS
#endif // __cplusplus

