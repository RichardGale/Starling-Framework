// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "EnterFrameEvent.h"
#include "starling/events/Event.h"

    /** An EnterFrameEvent is triggered once per frame and is dispatched to all objects in the
     *  display tree.
     *
     *  It contains information about the time that has passed since the last frame. That way, you 
     *  can easily make animations that are independent of the frame rate, taking the passed time
     *  into account.
     */

using namespace starling::events;

namespace starling {
namespace events {


        /** Event type for a display object that is entering a new frame. */
        const std::string EnterFrameEvent::ENTER_FRAME = "enterFrame";

        /** Creates an enter frame event with the passed time. */
        EnterFrameEvent::EnterFrameEvent(std::string type, float passedTime, bool bubbles)
        {
            Event(type, bubbles, passedTime);
        }

        /** The time that has passed since the last frame (in seconds). */
        float EnterFrameEvent::passedTime()        { return dynamic_cast<float>(data); }
}
}

