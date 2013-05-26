#if !defined(__STARLING_SRC_STARLING_EVENTS_ENTERFRAMEEVENT_AS)
#define __STARLING_SRC_STARLING_EVENTS_ENTERFRAMEEVENT_AS
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
#include "starling/events/Event.h"

/** An EnterFrameEvent is triggered once per frame and is dispatched to all objects in the
 *  display tree.
 *
 *  It contains information about the time that has passed since the last frame. That way, you
 *  can easily make animations that are independent of the frame rate, taking the passed time
 *  into account.
 */

using namespace starling::events;

namespace starling
{
    namespace events
    {
        class EnterFrameEvent : public starling::events::Event
        {
            /** Event type for a display object that is entering a new frame. */
        public:
            static const std::string ENTER_FRAME;

            /** Creates an enter frame event with the passed time. */
        public:
            EnterFrameEvent(std::string type, float passedTime, bool bubbles=false);

            /** The time that has passed since the last frame (in seconds). */
        public:
            float       passedTime();
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_ENTERFRAMEEVENT_AS
#endif // __cplusplus

