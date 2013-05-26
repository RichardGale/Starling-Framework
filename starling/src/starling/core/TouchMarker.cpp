// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TouchMarker.h"
#include "flash/display/BitmapData.h"
#include "flash/display/Shape.h"
#include "flash/display/Graphics.h"
#include "flash/geom/Point.h"

#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/textures/Texture.h"
#include "starling/core/starling.h"

    /** The TouchMarker is used internally to mark touches created through "simulateMultitouch". */

using namespace flash::display;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::textures;

namespace starling {
namespace core {


                    
                    

        TouchMarker::TouchMarker()
        {
            mCenter = new Point();
            mTexture = createTexture();

            for (int i=0; i<2; ++i)
            {
                Image* marker = new Image(mTexture);
                marker->pivotX ( mTexture->width() / 2);
                marker->pivotY ( mTexture->height() / 2);
                marker->touchable ( false);
                addChild(marker);
            }
        }

        void TouchMarker::dispose()
        {
            mTexture->dispose();
            Sprite::dispose();
        }

        void TouchMarker::moveMarker(float x, float y, bool withCenter)
        {
            if (withCenter)
            {
                mCenter->x() += x - realMarker()->x;
                mCenter->y() += y - realMarker()->y;
            }

            realMarker()->x ( x);
            realMarker()->y ( y);
            mockMarker()->x ( 2*mCenter->x() - x);
            mockMarker()->y ( 2*mCenter->y() - y);
        }

        void TouchMarker::moveCenter(float x, float y)
        {
            mCenter->x ( x);
            mCenter->y ( y);
            moveMarker(realX, realY); // reset mock position
        }

        starling::textures::Texture* TouchMarker::createTexture()
        {
            float scale = Starling()->contentScaleFactor();
            float radius = 12 * scale;
            int width = 32 * scale;
            int height = 32 * scale;
            float thickness = 1.5 * scale;
            Shape* shape = new Shape();

            // draw dark outline
            shape->graphics()->lineStyle(thickness, 0x0, 0.3);
            shape->graphics()->drawCircle(width/2, height/2, radius + thickness);

            // draw white inner circle
            shape->graphics()->beginFill(0xffffff, 0.4);
            shape->graphics()->lineStyle(thickness, 0xffffff);
            shape->graphics()->drawCircle(width/2, height/2, radius);
            shape->graphics()->endFill();

            BitmapData* bmpData = new BitmapData(width, height, true, 0x0);
            bmpData->draw(shape);

            return Texture::fromBitmapData(bmpData, false, false, scale);
        }

        Image* TouchMarker::realMarker()       { return getChildAt(0) as Image; }
        Image* TouchMarker::mockMarker()       { return getChildAt(1) as Image; }

        float TouchMarker::realX()        { return realMarker()->x(); }
        float TouchMarker::realY()        { return realMarker()->y(); }

        float TouchMarker::mockX()        { return mockMarker()->x(); }
        float TouchMarker::mockY()        { return mockMarker()->y(); }
}
}

