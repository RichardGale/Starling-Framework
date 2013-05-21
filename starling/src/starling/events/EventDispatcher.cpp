// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "EventDispatcher.h"
#include "flash/utils/Dictionary.h"

#include "starling/core/starling_internal.h"
#include "starling/display/DisplayObject.h"

#include "starling/events/Event.h"

    //use namespace starling_internal;

    /** The EventDispatcher class is the base class for all classes that dispatch events. 
     *  This is the Starling version of the Flash class with the same name. 
     *  
     *  <p>The event mechanism is a key feature of Starling's architecture. Objects can communicate 
     *  with each other through events. Compared the the Flash event system, Starling's event system
     *  was simplified. The main difference is that Starling events have no "Capture" phase.
     *  They are simply dispatched at the target and may optionally bubble up. They cannot move 
     *  in the opposite direction.</p>  
     *  
     *  <p>As in the conventional Flash classes, display objects inherit from EventDispatcher 
     *  and can thus dispatch events. Beware, though, that the Starling event classes are 
     *  <em>not compatible with Flash events:</em> Starling display objects dispatch 
     *  Starling events, which will bubble along Starling display objects - but they cannot 
     *  dispatch Flash events or bubble along Flash display objects.</p>
     *  
     *  @see Event
     *  @see starling.display.DisplayObject DisplayObject
     */

using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;

namespace starling {
namespace events {


                    

        /** Helper object. */
         std::vector<void*> EventDispatcher::sBubbleChains ;

        /** Creates an EventDispatcher. */
        EventDispatcher::EventDispatcher()
        {  }

        /** Registers an event listener at a certain object. */
        void EventDispatcher::addEventListener(std::string type, Function* listener)
        {
            if (mEventListeners.empty())
                mEventListeners.clear();

             std::vector<Function*> listeners=dynamic_cast<std::vector>(mEventListeners[type])           ;
            if (listeners.empty())
                mEventListeners[type] = new <Function*>[listener];
            else if (listeners.indexOf(listener) == -1)
                listeners.push_back(listener);
        }

        /** Removes an event listener from the object. */// check for duplicates
        void EventDispatcher::removeEventListener(std::string type, Function* listener)
        {
            if (mEventListeners)
            {
                 std::vector<Function*> listeners=dynamic_cast<std::vector>(mEventListeners[type])           ;
                if (listeners)
                {
                     int numListeners = listeners.size();
                     std::vector<Function*> remainingListeners=new<Function*>[];

                    for ( int i=0; i<numListeners; ++i)
                        if (listeners[i] != listener) remainingListeners.push_back(listeners[i]);

                    mEventListeners[type] = remainingListeners;
                }
            }
        }

        /** Removes all event listeners with a certain type, or all of them if type is null. 
         *  Be careful when removing all event listeners: you never know who else was listening. */
        void EventDispatcher::removeEventListeners(std::string type)
        {
            if (type && !mEventListeners.empty())
                delete mEventListeners[type];
            else
                mEventListeners.clear();
        }

        /** Dispatches an event to all objects that have registered listeners for its type. 
         *  If an event with enabled 'bubble' property is dispatched to a display object, it will 
         *  travel up along the line of parents, until it either hits the root object or someone
         *  stops its propagation manually. */
        void EventDispatcher::dispatchEvent(Event* event)
        {
             bool bubbles    = event->bubbles;

            if (!bubbles && (mEventListeners.empty() || !(event->type() in mEventListeners)))
                return;




             EventDispatcher* previousTarget= event->target();
            event->setTarget(this);

            if (bubbles && dynamic_cast<DisplayObject*>(this)) bubbleEvent(event);
            else                                  invokeEvent(event);

            if (previousTarget) event->setTarget(previousTarget);
        }

        /** @private
         *  Invokes an event on the current object. This method does not do any bubbling, nor
         *  does it back-up and restore the previous target on the event. The 'dispatchEvent' 
         *  method uses this method internally. */// no need to do anything// we save the current target and restore it later;// this allows users to re-dispatch events without creating a clone.
        bool EventDispatcher::invokeEvent(Event* event)
        {
             std::vector<Function*> listeners=mEventListeners?
                dynamic_cast<std::vector>(mEventListeners[event->type()])            : NULL;
             int numListeners = listeners.empty() ? 0 : listeners.size();

            if (numListeners)
            {
                event->setCurrentTarget(this);

                // we can enumerate directly over the vector, because:
                // when somebody modifies the list while we're looping, "addEventListener" is not
                // problematic, and "removeEventListener" will create a new Vector, anyway.

                for ( int i=0; i<numListeners; ++i)
                {
                     Function* listener= dynamic_cast<Function*>(listeners[i]);
                     int numArgs = listener->length();

                    if (numArgs == 0) listener();
                    else if (numArgs == 1) listener(event);
                    else listener(event, event->data());

                    if (event->stopsImmediatePropagation())
                        return true;
                }

                return event->stopsPropagation();
            }
            else
            {
                return false;
            }
        }

        /** @private */
        void EventDispatcher::bubbleEvent(Event* event)
        {
            // we determine the bubble chain before starting to invoke the listeners.
            // that way, changes done by the listeners won't affect the bubble chain.

             std::vector<EventDispatcher*> chain;
             DisplayObject* element= dynamic_cast<DisplayObject*>(this);
             int length = 1;

            if (sBubbleChains.size() > 0) { chain = sBubbleChains.pop(); chain[0] = element; }
            else chain.clear();

            while ((element = element->parent()) != NULL)
                chain[int(length++)] = element;

            for ( int i=0; i<length; ++i)
            {
                 bool stopPropagation    = chain[i]->invokeEvent(event);
                if (stopPropagation) break;
            }

            chain.clear()    ;
            sBubbleChains.push(chain);
        }

        /** Dispatches an event with the given parameters to all objects that have registered 
         *  listeners for the given type. The method uses an internal pool of event objects to 
         *  avoid allocations. */
        void EventDispatcher::dispatchEventWith(std::string type, bool bubbles, Object* data)
        {
            if (bubbles || hasEventListener(type))
            {
                 Event* event= Event::fromPool(type, bubbles, data);
                dispatchEvent(event);
                Event::toPool(event);
            }
        }

        /** Returns if there are listeners registered for a certain event type. */
        bool EventDispatcher::hasEventListener(std::string type)
        {
             std::vector<Function*> listeners=mEventListeners?
                dynamic_cast<std::vector>(mEventListeners[type])            : NULL;
            return listeners ? listeners.size() != 0 : false;
        }
}
}

