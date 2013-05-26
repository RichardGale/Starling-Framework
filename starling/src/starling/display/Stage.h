#if !defined(__STARLING_SRC_STARLING_DISPLAY_STAGE_AS)
#define __STARLING_SRC_STARLING_DISPLAY_STAGE_AS
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
    namespace errors
    {
        class IllegalOperationError;
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
    namespace events
    {
        class EnterFrameEvent;
    }
}
namespace starling
{
    namespace events
    {
        class Event;
    }
}

#include "starling/display/DisplayObjectContainer.h"

//use namespace starling_internal;

/** Dispatched when the Flash container is resized. */
//[Event(name="resize",type="starling.events.ResizeEvent")]

/** Dispatched when a key on the keyboard is released. */
//[Event(name="keyUp",type="starling.events.KeyboardEvent")]

/** Dispatched when a key on the keyboard is pressed. */
//[Event(name="keyDown",type="starling.events.KeyboardEvent")]

using namespace flash::errors;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;

namespace starling
{
    namespace display
    {
        /** A Stage represents the root of the display tree.
         *  Only objects that are direct or indirect children of the stage will be rendered.
         *
         *  <p>This class represents the Starling version of the stage. Don't confuse it with its
         *  Flash equivalent: while the latter contains objects of the type
         *  <code>flash.display.DisplayObject</code>, the Starling stage contains only objects of the
         *  type <code>starling.display.DisplayObject</code>. Those classes are not compatible, and
         *  you cannot exchange one type with the other.</p>
         *
         *  <p>A stage object is created automatically by the <code>Starling</code> class. Don't
         *  create a Stage instance manually.</p>
         *
         *  <strong>Keyboard Events</strong>
         *
         *  <p>In Starling, keyboard events are only dispatched at the stage. Add an event listener
         *  directly to the stage to be notified of keyboard events.</p>
         *
         *  <strong>Resize Events</strong>
         *
         *  <p>When the Flash player is resized, the stage dispatches a <code>ResizeEvent</code>. The
         *  event contains properties containing the updated width and height of the Flash player.</p>
         *
         *  @see starling.events.KeyboardEvent
         *  @see starling.events.ResizeEvent
         *
         * */
        class Stage : public starling::display::DisplayObjectContainer
        {
        private:
            int mWidth;
        private:
            int mHeight;
        private:
            unsigned int mColor;
        private:
            EnterFrameEvent *mEnterFrameEvent;

            /** @private */
        public:
            Stage(int width, int height, unsigned int color=0);

            /** @inheritDoc */
        public:
            void     advanceTime(float passedTime);

            /** Returns the object that is found topmost beneath a point in stage coordinates, or
             *  the stage itself if nothing else is found. */
        public:
            virtual DisplayObject *hitTest(Point *localPoint, bool forTouch=false);

            /** @private */
        public:
            virtual void         width(float value);

            /** @private */
        public:
            virtual void         height(float value);

            /** @private */
        public:
            virtual void         x(float value);

            /** @private */
        public:
            virtual void         y(float value);

            /** @private */
        public:
            virtual void         scaleX(float value);

            /** @private */
        public:
            virtual void         scaleY(float value);

            /** @private */
        public:
            virtual void         rotation(float value);

            /** The background color of the stage. */
        public:
            unsigned int         color();
        public:
            void         color(unsigned int value);

            /** The width of the stage coordinate system. Change it to scale its contents relative
             *  to the <code>viewPort</code> property of the Starling object. */
        public:
            int          stageWidth();
        public:
            void         stageWidth(int value);

            /** The height of the stage coordinate system. Change it to scale its contents relative
             *  to the <code>viewPort</code> property of the Starling object. */
        public:
            int          stageHeight();
        public:
            void         stageHeight(int value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_STAGE_AS
#endif // __cplusplus

