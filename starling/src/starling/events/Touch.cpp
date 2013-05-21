// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Touch.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"

#include "starling/core/starling_internal.h"
#include "starling/display/DisplayObject.h"
#include "starling/utils/MatrixUtil.h"
#include "starling/utils/formatString.h"

    //use namespace starling_internal;

    /** A Touch object contains information about the presence or movement of a finger 
     *  or the mouse on the screen.
     *  
     *  <p>You receive objects of this type from a TouchEvent. When such an event is triggered, you can 
     *  query it for all touches that are currently present on the screen. One Touch object contains
     *  information about a single touch. A touch object always moves through a series of
     *  TouchPhases. Have a look at the TouchPhase class for more information.</p>
     *  
     *  <strong>The position of a touch</strong>
     *  
     *  <p>You can get the current and previous position in stage coordinates with the corresponding 
     *  properties. However, you'll want to have the position in a different coordinate system 
     *  most of the time. For this reason, there are methods that convert the current and previous 
     *  touches into the local coordinate system of any object.</p>
     * 
     *  @see TouchEvent
     *  @see TouchPhase
     */

using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::utils;

namespace starling {
namespace events {


                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    

        /** Helper object. */
         Matrix* Touch::sHelperMatrix= new Matrix();

        /** Creates a new Touch object. */
        Touch::Touch(int id, float globalX, float globalY, std::string phase, DisplayObject* target)
        {
            mID = id;
            mGlobalX = mPreviousGlobalX = globalX;
            mGlobalY = mPreviousGlobalY = globalY;
            mTapCount = 0;
            mPhase = phase;
            mTarget = target;
            mPressure = mWidth = mHeight = 1.0;
            mBubbleChain.clear();
            updateBubbleChain();
        }

        /** Converts the current location of a touch to the local coordinate system of a display 
         *  object. If you pass a 'resultPoint', the result will be stored in this point instead 
         *  of creating a new object.*/
        Point* Touch::getLocation(DisplayObject* space, Point* resultPoint)
        {
            if (resultPoint == NULL) resultPoint = new Point();
            space->base()->getTransformationMatrix(space, sHelperMatrix);
            return MatrixUtil::transformCoords(sHelperMatrix, mGlobalX, mGlobalY, resultPoint);
        }

        /** Converts the previous location of a touch to the local coordinate system of a display 
         *  object. If you pass a 'resultPoint', the result will be stored in this point instead 
         *  of creating a new object.*/
        Point* Touch::getPreviousLocation(DisplayObject* space, Point* resultPoint)
        {
            if (resultPoint == NULL) resultPoint = new Point();
            space->base()->getTransformationMatrix(space, sHelperMatrix);
            return MatrixUtil::transformCoords(sHelperMatrix, mPreviousGlobalX, mPreviousGlobalY, resultPoint);
        }

        /** Returns the movement of the touch between the current and previous location. 
         *  If you pass a 'resultPoint', the result will be stored in this point instead 
         *  of creating a new object. */
        Point* Touch::getMovement(DisplayObject* space, Point* resultPoint)
        {
            if (resultPoint == NULL) resultPoint = new Point();
            getLocation(space, resultPoint);
             float x  = resultPoint->x;
             float y  = resultPoint->y;
            getPreviousLocation(space, resultPoint);
            resultPoint->setTo(x - resultPoint->x, y - resultPoint->y);
            return resultPoint;
        }

        /** Indicates if the target or one of its children is touched. */
        bool Touch::isTouching(DisplayObject* target)
        {
            return mBubbleChain.indexOf(target) != -1;
        }

        /** Returns a description of the object. */
        std::string Touch::toString()
        {
            return formatString("Touch {0}: globalX={1}, globalY={2}, phase={3}",
                                mID, mGlobalX, mGlobalY, mPhase);
        }

        /** Creates a clone of the Touch object. */
        Touch* Touch::clone()
        {
             Touch* clone= new Touch(mID, mGlobalX, mGlobalY, mPhase, mTarget);
            clone->mPreviousGlobalX ( mPreviousGlobalX);
            clone->mPreviousGlobalY ( mPreviousGlobalY);
            clone->mTapCount ( mTapCount);
            clone->mTimestamp ( mTimestamp);
            clone->mPressure ( mPressure);
            clone->mWidth ( mWidth);
            clone->mHeight ( mHeight);
            return clone;
        }

        // helper methods

        void Touch::updateBubbleChain()
        {
            if (mTarget)
            {
                 int length = 1;
                 DisplayObject* element= mTarget;

                mBubbleChain.size() = 1;
                mBubbleChain[0] = element;

                while ((element = element->parent()) != NULL)
                    mBubbleChain[int(length++)] = element;
            }
            else
            {
                mBubbleChain.clear()    ;
            }
        }

        // properties

        /** The identifier of a touch. '0' for mouse events, an increasing number for touches. */
        int Touch::id()     { return mID; }

        /** The x-position of the touch in stage coordinates. */
        float Touch::globalX()        { return mGlobalX; }

        /** The y-position of the touch in stage coordinates. */
        float Touch::globalY()        { return mGlobalY; }

        /** The previous x-position of the touch in stage coordinates. */
        float Touch::previousGlobalX()        { return mPreviousGlobalX; }

        /** The previous y-position of the touch in stage coordinates. */
        float Touch::previousGlobalY()        { return mPreviousGlobalY; }

        /** The number of taps the finger made in a short amount of time. Use this to detect 
         *  double-taps / double-clicks, etc. */
        int Touch::tapCount()     { return mTapCount; }

        /** The current phase the touch is in. @see TouchPhase */
        std::string Touch::phase()        { return mPhase; }

        /** The display object at which the touch occurred. */
        DisplayObject* Touch::target()               { return mTarget; }

        /** The moment the touch occurred (in seconds since application start). */
        float Touch::timestamp()        { return mTimestamp; }

        /** A value between 0.0 and 1.0 indicating force of the contact with the device. 
         *  If the device does not support detecting the pressure, the value is 1.0. */
        float Touch::pressure()        { return mPressure; }

        /** Width of the contact area. 
         *  If the device does not support detecting the pressure, the value is 1.0. */
        float Touch::width()        { return mWidth; }

        /** Height of the contact area. 
         *  If the device does not support detecting the pressure, the value is 1.0. */
        float Touch::height()        { return mHeight; }

        // internal methods

        /** @private 
         *  Dispatches a touch event along the current bubble chain (which is updated each time
         *  a target is set). */
        void Touch::dispatchEvent(TouchEvent* event)
        {
            if (mTarget) event->dispatch(mBubbleChain);
        }

        /** @private */
        std::vector<EventDispatcher*> Touch::bubbleChain()
        {
            return mBubbleChain.concat();
        }

        /** @private */
        void Touch::setTarget(DisplayObject* value)
        {
            mTarget = value;
            updateBubbleChain();
        }

        /** @private */
        void Touch::setPosition(float globalX, float globalY)
        {
            mPreviousGlobalX = mGlobalX;
            mPreviousGlobalY = mGlobalY;
            mGlobalX = globalX;
            mGlobalY = globalY;
        }

        /** @private */
        void Touch::setSize(float width, float height)
        {
            mWidth = width;
            mHeight = height;
        }

        /** @private */
        void Touch::setPhase(std::string value)      { mPhase = value; }

        /** @private */
        void Touch::setTapCount(int value)      { mTapCount = value; }

        /** @private */
        void Touch::setTimestamp(float value)      { mTimestamp = value; }

        /** @private */
        void Touch::setPressure(float value)      { mPressure = value; }
}
}

