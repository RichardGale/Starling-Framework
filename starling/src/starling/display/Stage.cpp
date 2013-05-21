// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Stage.h"
#include "flash/errors/IllegalOperationError.h"
#include "flash/geom/Point.h"

#include "starling/core/starling_internal.h"
#include "starling/events/EnterFrameEvent.h"
#include "starling/events/Event.h"

#include "starling/display/DisplayObjectContainer.h"

    //use namespace starling_internal;

    /** Dispatched when the Flash container is resized. */

using namespace flash::errors;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;

namespace starling {
namespace display {


                    
                    
                    
                    

        /** @private */
        Stage::Stage(int width, int height, unsigned int color)
        {
            mWidth = width;
            mHeight = height;
            mColor = color;
        }

        /** @inheritDoc */
        void Stage::advanceTime(float passedTime)
        {
            mEnterFrameEvent->reset(Event::ENTER_FRAME, false, passedTime);
            broadcastEvent(mEnterFrameEvent);
        }

        /** Returns the object that is found topmost beneath a point in stage coordinates, or  
         *  the stage itself if nothing else is found. */
        DisplayObject* Stage::hitTest(Point* localPoint, bool forTouch)
        {
            if (forTouch && (!visible || !touchable))
                return NULL;

            // locations outside of the stage area shouldn't be accepted
            if (localPoint->x < 0 || localPoint->x > mWidth ||
                localPoint->y < 0 || localPoint->y() > mHeight)
                return NULL;

            // if nothing else is hit, the stage returns itself as target
             DisplayObject* target= super()->hitTest(localPoint, forTouch);
            if (target == NULL) target = this;
            return target;
        }

        /** @private */
        void Stage::width(float value)
        {
            throw new IllegalOperationError("Cannot set width of stage");
        }

        /** @private */
        void Stage::height(float value)
        {
            throw new IllegalOperationError("Cannot set height of stage");
        }

        /** @private */
        void Stage::x(float value)
        {
            throw new IllegalOperationError("Cannot set x-coordinate of stage");
        }

        /** @private */
        void Stage::y(float value)
        {
            throw new IllegalOperationError("Cannot set y-coordinate of stage");
        }

        /** @private */
        void Stage::scaleX(float value)
        {
            throw new IllegalOperationError("Cannot scale stage");
        }

        /** @private */
        void Stage::scaleY(float value)
        {
            throw new IllegalOperationError("Cannot scale stage");
        }

        /** @private */
        void Stage::rotation(float value)
        {
            throw new IllegalOperationError("Cannot rotate stage");
        }

        /** The background color of the stage. */
        unsigned int Stage::color()      { return mColor; }
        void Stage::color(unsigned int value)      { mColor = value; }

        /** The width of the stage coordinate system. Change it to scale its contents relative
         *  to the <code>viewPort</code> property of the Starling object. */
        int Stage::stageWidth()     { return mWidth; }
        void Stage::stageWidth(int value)      { mWidth = value; }

        /** The height of the stage coordinate system. Change it to scale its contents relative
         *  to the <code>viewPort</code> property of the Starling object. */
        int Stage::stageHeight()     { return mHeight; }
        void Stage::stageHeight(int value)      { mHeight = value; }
}
}

