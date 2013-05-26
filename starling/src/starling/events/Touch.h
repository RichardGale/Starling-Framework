#if !defined(__STARLING_SRC_STARLING_EVENTS_TOUCH_AS)
#define __STARLING_SRC_STARLING_EVENTS_TOUCH_AS
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
    namespace geom
    {
        class Matrix;
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
    namespace core
    {
        class starling_internal;
    }
}
namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
namespace starling
{
    namespace utils
    {
        class MatrixUtil;
    }
}
namespace starling
{
    namespace utils
    {
        class formatString;
    }
}
namespace starling
{
    namespace events
    {
        class EventDispatcher;
    }
}

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
using namespace starling::events;
using namespace starling::utils;

namespace starling
{
    namespace events
    {
        class Touch
        {
        private:
            int mID;
        private:
            float mGlobalX;
        private:
            float mGlobalY;
        private:
            float mPreviousGlobalX;
        private:
            float mPreviousGlobalY;
        private:
            int mTapCount;
        private:
            std::string mPhase;
        private:
            DisplayObject *mTarget;
        private:
            float mTimestamp;
        private:
            float mPressure;
        private:
            float mWidth;
        private:
            float mHeight;
        private:
            std::vector<EventDispatcher *> mBubbleChain;

            /** Helper object. */
        private:
            static Matrix *sHelperMatrix;

            /** Creates a new Touch object. */
        public:
            Touch(int id, float globalX, float globalY, std::string phase, DisplayObject *target);

            /** Converts the current location of a touch to the local coordinate system of a display
             *  object. If you pass a 'resultPoint', the result will be stored in this point instead
             *  of creating a new object.*/
        public:
            Point    *getLocation(DisplayObject *space, Point *resultPoint=NULL);

            /** Converts the previous location of a touch to the local coordinate system of a display
             *  object. If you pass a 'resultPoint', the result will be stored in this point instead
             *  of creating a new object.*/
        public:
            Point    *getPreviousLocation(DisplayObject *space, Point *resultPoint=NULL);

            /** Returns the movement of the touch between the current and previous location.
             *  If you pass a 'resultPoint', the result will be stored in this point instead
             *  of creating a new object. */
        public:
            Point    *getMovement(DisplayObject *space, Point *resultPoint=NULL);

            /** Indicates if the target or one of its children is touched. */
        public:
            bool  isTouching(DisplayObject *target);

            /** Returns a description of the object. */
        public:
            std::string   toString();

            /** Creates a clone of the Touch object. */
        public:
            Touch    *clone();

            // helper methods

        private:
            void     updateBubbleChain();

            // properties

            /** The identifier of a touch. '0' for mouse events, an increasing number for touches. */
        public:
            int          id();

            /** The x-position of the touch in stage coordinates. */
        public:
            float       globalX();

            /** The y-position of the touch in stage coordinates. */
        public:
            float       globalY();

            /** The previous x-position of the touch in stage coordinates. */
        public:
            float       previousGlobalX();

            /** The previous y-position of the touch in stage coordinates. */
        public:
            float       previousGlobalY();

            /** The number of taps the finger made in a short amount of time. Use this to detect
             *  double-taps / double-clicks, etc. */
        public:
            int          tapCount();

            /** The current phase the touch is in. @see TouchPhase */
        public:
            std::string       phase();

            /** The display object at which the touch occurred. */
        public:
            DisplayObject *target();

            /** The moment the touch occurred (in seconds since application start). */
        public:
            float       timestamp();

            /** A value between 0.0 and 1.0 indicating force of the contact with the device.
             *  If the device does not support detecting the pressure, the value is 1.0. */
        public:
            float       pressure();

            /** Width of the contact area.
             *  If the device does not support detecting the pressure, the value is 1.0. */
        public:
            float       width();

            /** Height of the contact area.
             *  If the device does not support detecting the pressure, the value is 1.0. */
        public:
            float       height();

            // internal methods

            /** @private
             *  Dispatches a touch event along the current bubble chain (which is updated each time
             *  a target is set). */
            void     dispatchEvent(TouchEvent *event);

            /** @private */
            std::vector<EventDispatcher *> bubbleChain();

            /** @private */
            void     setTarget(DisplayObject *value);

            /** @private */
            void     setPosition(float globalX, float globalY);

            /** @private */
            void     setSize(float width, float height);

            /** @private */
            void     setPhase(std::string value);

            /** @private */
            void     setTapCount(int value);

            /** @private */
            void     setTimestamp(float value);

            /** @private */
            void     setPressure(float value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_TOUCH_AS
#endif // __cplusplus

