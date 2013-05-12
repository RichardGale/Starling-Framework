// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TouchProcessor.h"
#include "flash/geom/Point.h"
#include "flash/utils/getDefinitionByName.h"

#include "starling/display/Stage.h"
#include "starling/events/KeyboardEvent.h"
#include "starling/events/Touch.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"

//use starling_internal        ;

/** @private
 *  The TouchProcessor is used internally to convert mouse and touch events of the conventional
 *  Flash stage to Starling's TouchEvents. */
using namespace flash::geom;
using namespace flash::utils;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;

namespace starling
{
    namespace core
    {


        const float TouchProcessor::MULTITAP_TIME=0.3;
        const float TouchProcessor::MULTITAP_DISTANCE=25;

        /** Helper objects. */
        std::vector<int> *TouchProcessor::sProcessedTouchIDs=new<int>[];
        std::vector<Object *> *TouchProcessor::sHoveringTouchData=new<Object *>[];

        TouchProcessor::TouchProcessor(Stage *stage)
        {
            mStage = stage;
            mElapsedTime = 0.0;
            mCurrentTouches.clear();
            mQueue.clear();
            mLastTaps.clear();

            mStage->addEventListener(KeyboardEvent::KEY_DOWN,onKey);
            mStage->addEventListener(KeyboardEvent::KEY_UP, onKey);
            monitorInterruptions(true);
        }

        void TouchProcessor::dispose()
        {
            monitorInterruptions(false);
            mStage->removeEventListener(KeyboardEvent::KEY_DOWN,onKey);
            mStage->removeEventListener(KeyboardEvent::KEY_UP, onKey);
            if (mTouchMarker) mTouchMarker->dispose();
        }

        void TouchProcessor::advanceTime(float passedTime)
        {
            int i;
            int touchID;
            Touch *touch;

            mElapsedTime += passedTime;

            // remove old taps
            if (mLastTaps.length > 0)
            {
                for (i=mLastTaps.length-1; i>=0; --i)
                    if (mElapsedTime - mLastTaps[i]->timestamp> MULTITAP_TIME)
                        mLastTaps.splice(i, 1);
            }

            while (mQueue.length > 0)
            {
                sProcessedTouchIDs.length = sHoveringTouchData.length = 0;

                // set touches that were new or moving to phase 'stationary'
                for each (touch in mCurrentTouches)
                    if (touch->phase== TouchPhase::BEGAN|| touch->phase== TouchPhase::MOVED)
                        touch->setPhase(TouchPhase::STATIONARY);

                // process new touches, but each ID only once
                while (mQueue.length > 0 &&
                        sProcessedTouchIDs.indexOf(mQueue[mQueue.length-1][0]) == -1)
                {
                    std::vector<void *> touchArgs=mQueue.pop();
                    touchID = static_cast<int>(touchArgs[0]);
                    touch = getCurrentTouch(touchID);

                    // hovering touches need special handling (see below)
                    if (touch && touch->phase== TouchPhase::HOVER&& touch->target)
                        sHoveringTouchData.push_back(
                    {
touch: touch,
target: touch->target,
bubbleChain: touch->bubbleChain
                    });

                    processTouch->apply(this,touchArgs);
                    sProcessedTouchIDs.push_back(touchID);
                }

                // the same touch event will be dispatched to all targets;
                // the 'dispatch' method will make sure each bubble target is visited only once.
                TouchEvent *touchEvent=
                    new TouchEvent(TouchEvent::TOUCH,mCurrentTouches, mShiftDown, mCtrlDown);

                // if the target of a hovering touch changed, we dispatch the event to the previous
                // target to notify it that it's no longer being hovered over.
                for each (var Object* touchDatain sHoveringTouchData)
                    if (touchData->touch->target!=touchData->target)
                        touchEvent->dispatch(touchData->bubbleChain);

                // dispatch events
                for each (touchID in sProcessedTouchIDs)
                    getCurrentTouch(touchID)->dispatchEvent(touchEvent);

                // remove ended touches
                for (i=mCurrentTouches.length-1; i>=0; --i)
                    if (mCurrentTouches[i]->phase== TouchPhase::ENDED)
                        mCurrentTouches.splice(i, 1);
            }
        }

        void TouchProcessor::enqueue(int touchID, std::string phase, float globalX, float globalY,
                                     float pressure, float width, float height)
        {
            mQueue.unshift(arguments);

            // multitouch simulation (only with mouse)
            if (mCtrlDown && simulateMultitouch && touchID == 0)
            {
                mTouchMarker->moveMarker(globalX,globalY, mShiftDown);
                mQueue.unshift([1, phase, mTouchMarker->mockX,mTouchMarker->mockY]);
            }
        }

        void TouchProcessor::enqueueMouseLeftStage()
        {
            Touch *mouse=getCurrentTouch(0);
            if (mouse == NULL || mouse->phase!= TouchPhase::HOVER)return;





            int offset= 1;
            float exitX = mouse->globalX;
            float exitY = mouse->globalY;
            float distLeft = mouse->globalX;
            float distRight = mStage->stageWidth- distLeft;
            float distTop = mouse->globalY;
            float distBottom = mStage->stageHeight- distTop;
            float minDist = Math::min(distLeft,distRight, distTop, distBottom);

            // the new hover point should be just outside the stage, near the point where
            // the mouse point was last to be seen.

            if (minDist == distLeft)       exitX = -offset;
            else if (minDist == distRight) exitX = mStage->stageWidth+ offset;
            else if (minDist == distTop)   exitY = -offset;
            else                           exitY = mStage->stageHeight+ offset;

            enqueue(0, TouchPhase::HOVER,exitX, exitY);
        }   // On OS X, we get mouse events from outside the stage; on Windows, we do not.

        void TouchProcessor::processTouch(int touchID, std::string phase, float globalX, float globalY,
                                          float pressure, float width, float height)
        {
            Point *position=new Point(globalX, globalY);
            Touch *touch=getCurrentTouch(touchID);

            if (touch == NULL)
            {
                touch = new Touch(touchID, globalX, globalY, phase, NULL);
                addCurrentTouch(touch);
            }

            touch->setPosition(globalX,globalY);
            touch->setPhase(phase);
            touch->setTimestamp(mElapsedTime);
            touch->setPressure(pressure);
            touch->setSize(width,height);

            if (phase == TouchPhase::HOVER|| phase == TouchPhase::BEGAN)
                touch->setTarget(mStage->hitTest(position,true));

            if (phase == TouchPhase::BEGAN)
                processTap(touch);
        }

        void TouchProcessor::onKey(KeyboardEvent *event)
        {
            if (event->keyCode== 17 || event->keyCode== 15) // ctrl or cmd key
            {
                bool wasCtrlDown   = mCtrlDown;
                mCtrlDown = event->type== KeyboardEvent::KEY_DOWN;

                if (simulateMultitouch && wasCtrlDown != mCtrlDown)
                {
                    mTouchMarker->visible= mCtrlDown;
                    mTouchMarker->moveCenter(mStage->stageWidth/2,mStage->stageHeight/2);

                    Touch *mouseTouch=getCurrentTouch(0);
                    Touch *mockedTouch=getCurrentTouch(1);

                    if (mouseTouch)
                        mTouchMarker->moveMarker(mouseTouch->globalX,mouseTouch->globalY);

                    // end active touch ...
                    if (wasCtrlDown && mockedTouch && mockedTouch->phase!= TouchPhase::ENDED)
                        mQueue.unshift([1, TouchPhase::ENDED,mockedTouch->globalX,mockedTouch->globalY]);
                    // ... or start new one
                    else if (mCtrlDown && mouseTouch)
                    {
                        if (mouseTouch->phase== TouchPhase::HOVER|| mouseTouch->phase== TouchPhase::ENDED)
                            mQueue.unshift([1, TouchPhase::HOVER,mTouchMarker->mockX,mTouchMarker->mockY]);
                        else
                            mQueue.unshift([1, TouchPhase::BEGAN,mTouchMarker->mockX,mTouchMarker->mockY]);
                    }
                }
            }
            else if (event->keyCode== 16)
            {
                mShiftDown = event->type== KeyboardEvent::KEY_DOWN;
            }
        }                                 // shift key

        void TouchProcessor::processTap(Touch *touch)
        {
            Touch *nearbyTap=NULL;
            float minSqDist = MULTITAP_DISTANCE * MULTITAP_DISTANCE;

            for each (var Touch* tapin mLastTaps)
            {
                float sqDist = Math::pow(tap->globalX-touch->globalX,2) +
                               Math::pow(tap->globalY-touch->globalY,2);
                if (sqDist <= minSqDist)
                {
                    nearbyTap = tap;
                    break;
                }
            }

            if (nearbyTap)
            {
                touch->setTapCount(nearbyTap->tapCount+1);
                mLastTaps.splice(mLastTaps.indexOf(nearbyTap), 1);
            }
            else
            {
                touch->setTapCount(1);
            }

            mLastTaps.push_back(touch->clone());
        }

        void TouchProcessor::addCurrentTouch(Touch *touch)
        {
            for ( int i=mCurrentTouches.length-1; i>=0; --i)
                if (mCurrentTouches[i]->id== touch->id)
                    mCurrentTouches.splice(i, 1);

            mCurrentTouches.push_back(touch);
        }

        Touch *TouchProcessor::getCurrentTouch(int touchID)
        {
            for each (var Touch* touchin mCurrentTouches)
                if (touch->id== touchID) return touch;
            return NULL;
        }

        bool TouchProcessor::simulateMultitouch()
        {
            return mTouchMarker != NULL;
        }
        void TouchProcessor::simulateMultitouch(bool value)
        {
            if (simulateMultitouch == value) return;
            if (value)
            {
                mTouchMarker = new TouchMarker();
                mTouchMarker->visible= false;
                mStage->addChild(mTouchMarker);
            }
            else
            {
                mTouchMarker->removeFromParent(true);
                mTouchMarker = NULL;
            }
        }

        // interruption handling                     // no change

        void TouchProcessor::monitorInterruptions(bool enable)
        {
            // if the application moves into the background or is interrupted (e.g. through
            // an incoming phone call), we need to abort all touches.

            try
            {
                Object *nativeAppClass=getDefinitionByName("flash.desktop::NativeApplication");
                Object *nativeApp=nativeAppClass["nativeApplication"];

                if (enable)
                    nativeApp->addEventListener("deactivate",onInterruption, false, 0, true);
                else
                    nativeApp->removeEventListener("activate",onInterruption);
            }
catch (e:Error *) {} // we're not running in AIR
        }

        void TouchProcessor::onInterruption(Object *event)
        {
            Touch *touch;

            // abort touches
            for each (touch in mCurrentTouches)
            {
                if (touch->phase== TouchPhase::BEGAN|| touch->phase== TouchPhase::MOVED||
                        touch->phase== TouchPhase::STATIONARY)
                {
                    touch->setPhase(TouchPhase::ENDED);
                }
            }

            // dispatch events
            TouchEvent *touchEvent=
                new TouchEvent(TouchEvent::TOUCH,mCurrentTouches, mShiftDown, mCtrlDown);

            for each (touch in mCurrentTouches)
                touch->dispatchEvent(touchEvent);

            // purge touches
            mCurrentTouches.clear()
        }
    }
}

