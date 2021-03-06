// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TouchEvent.h"
#include "starling/core/starling_internal.h"
#include "starling/display/DisplayObject.h"
#include "starling/events/Event.h"
#include "starling/events/Touch.h"

    //use namespace starling_internal;

    /** A TouchEvent is triggered either by touch or mouse input.  
     *  
     *  <p>In Starling, both touch events and mouse events are handled through the same class: 
     *  TouchEvent. To process user input from a touch screen or the mouse, you have to register
     *  an event listener for events of the type <code>TouchEvent.TOUCH</code>. This is the only
     *  event type you need to handle; the long list of mouse event types as they are used in
     *  conventional Flash are mapped to so-called "TouchPhases" instead.</p> 
     * 
     *  <p>The difference between mouse input and touch input is that</p>
     *  
     *  <ul>
     *    <li>only one mouse cursor can be present at a given moment and</li>
     *    <li>only the mouse can "hover" over an object without a pressed button.</li>
     *  </ul> 
     *  
     *  <strong>Which objects receive touch events?</strong>
     * 
     *  <p>In Starling, any display object receives touch events, as long as the  
     *  <code>touchable</code> property of the object and its parents is enabled. There 
     *  is no "InteractiveObject" class in Starling.</p>
     *  
     *  <strong>How to work with individual touches</strong>
     *  
     *  <p>The event contains a list of all touches that are currently present. Each individual
     *  touch is stored in an object of type "Touch". Since you are normally only interested in 
     *  the touches that occurred on top of certain objects, you can query the event for touches
     *  with a specific target:</p>
     * 
     *  <code>var touches:Vector.&lt;Touch&gt; = touchEvent.getTouches(this);</code>
     *  
     *  <p>This will return all touches of "this" or one of its children. When you are not using 
     *  multitouch, you can also access the touch object directly, like this:</p>
     * 
     *  <code>var touch:Touch = touchEvent.getTouch(this);</code>
     *  
     *  @see Touch
     *  @see TouchPhase
     */

using namespace starling::core;
using namespace starling::display;
using namespace starling::events;

namespace starling {
namespace events {


        /** Event type for touch or mouse input. */
        const std::string TouchEvent::TOUCH = "touch";

                    
                    
                    
                    

        /** Helper object. */
        std::vector<Touch*> TouchEvent::sTouches=std::vector<void*>()           ;

        /** Creates a new TouchEvent instance. */
        TouchEvent::TouchEvent(std::string type, std::vector<Touch*> touches, bool shiftKey,
                                   bool ctrlKey, bool bubbles)
        {
            Event(type, bubbles, touches);

            mShiftKey = shiftKey;
            mCtrlKey = ctrlKey;
            mTimestamp = -1.0;
            mVisitedObjects.clear();

            int numTouches=touches.size();
            for (int i=0; i<numTouches; ++i)
                if (touches[i]->timestamp() > mTimestamp)
                    mTimestamp = touches[i]->timestamp();
        }

        /** Returns a list of touches that originated over a certain target. If you pass a
         *  'result' vector, the touches will be added to this vector instead of creating a new 
         *  object. */
        std::vector<Touch*> TouchEvent::getTouches(DisplayObject* target, std::string phase,
                                   std::vector<Touch*> result)
        {
            if (result.empty()) result.clear();
            std::vector<Touch*> allTouches=dynamic_cast<std::vector>(data)        ;
            int numTouches = allTouches.size();

            for (int i=0; i<numTouches; ++i)
            {
                Touch* touch = allTouches[i];
                bool correctTarget = touch->isTouching(target);
                bool correctPhase = (phase == NULL || phase == touch->phase);

                if (correctTarget && correctPhase)
                    result.push_back(touch);
            }
            return result;
        }

        /** Returns a touch that originated over a certain target. */
        Touch* TouchEvent::getTouch(DisplayObject* target, std::string phase)
        {
            getTouches(target, phase, sTouches);
            if (sTouches.size())
            {
                Touch* touch = sTouches[0];
                sTouches.clear()    ;
                return touch;
            }
            else return NULL;
        }

        /** Indicates if a target is currently being touched or hovered over. */
        bool TouchEvent::interactsWith(DisplayObject* target)
        {
            if (getTouch(target) == NULL)
                return false;
            else
            {
                std::vector<Touch*> touches=getTouches(target);

                for (int i=touches.size()-1; i>=0; --i)
                    if (touches[i]->phase() != TouchPhase()->ENDED())
                        return true;

                return false;
            }
        }

        // custom dispatching

        /** @private
         *  Dispatches the event along a custom bubble chain. During the lifetime of the event,
         *  each object is visited only once. */
        void TouchEvent::dispatch(std::vector<EventDispatcher*> chain)
        {
            if (chain && chain.size())
            {
                int chainLength = bubbles ? chain.size() : 1;
                EventDispatcher* previousTarget = target;
                setTarget(dynamic_cast<EventDispatcher*>(chain[0]));

                for (int i=0; i<chainLength; ++i)
                {
                    EventDispatcher* chainElement = dynamic_cast<EventDispatcher*>(chain[i]);
                    if (mVisitedObjects.indexOf(chainElement) == -1)
                    {
                        bool stopPropagation = chainElement->invokeEvent(this);
                        mVisitedObjects.push_back(chainElement);
                        if (stopPropagation) break;
                    }
                }

                setTarget(previousTarget);
            }
        }

        // properties

        /** The time the event occurred (in seconds since application launch). */
        float TouchEvent::timestamp()        { return mTimestamp; }

        /** All touches that are currently available. */
        std::vector<Touch*> TouchEvent::touches()                { return (dynamic_cast<std::vector>(data)        )->concat(); }

        /** Indicates if the shift key was pressed when the event occurred. */
        bool TouchEvent::shiftKey()         { return mShiftKey; }

        /** Indicates if the ctrl key was pressed when the event occurred. (Mac OS: Cmd or Ctrl) */
        bool TouchEvent::ctrlKey()         { return mCtrlKey; }
}
}

