#if !defined(__STARLING_SRC_STARLING_EVENTS_EVENTDISPATCHER_AS)
#define __STARLING_SRC_STARLING_EVENTS_EVENTDISPATCHER_AS
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



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash
{
    namespace utils
    {
        class Dictionary;
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

//use starling_internal        ;

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

namespace starling
{
    namespace events
    {
        class EventDispatcher
        {
        private:
            std::map<std::string, void *> mEventListeners;

            /** Helper object. */
        private:
            static  std::vector<void *> sBubbleChains;

            /** Creates an EventDispatcher. */
        public:
            EventDispatcher();

            /** Registers an event listener at a certain object. */
        public:
            void     addEventListener(std::string type, Function *listener);

            /** Removes an event listener from the object. */// check for duplicates
        public:
            void     removeEventListener(std::string type, Function *listener);

            /** Removes all event listeners with a certain type, or all of them if type is null.
             *  Be careful when removing all event listeners: you never know who else was listening. */
        public:
            void     removeEventListeners(std::string type=NULL);

            /** Dispatches an event to all objects that have registered listeners for its type.
             *  If an event with enabled 'bubble' property is dispatched to a display object, it will
             *  travel up along the line of parents, until it either hits the root object or someone
             *  stops its propagation manually. */
        public:
            void     dispatchEvent(Event *event);

            /** @private
             *  Invokes an event on the current object. This method does not do any bubbling, nor
             *  does it back-up and restore the previous target on the event. The 'dispatchEvent'
             *  method uses this method internally. */// no need to do anything
            bool invokeEvent(Event *event);

            /** @private */
            void bubbleEvent(Event *event);

            /** Dispatches an event with the given parameters to all objects that have registered
             *  listeners for the given type. The method uses an internal pool of event objects to
             *  avoid allocations. */
        public:
            void     dispatchEventWith(std::string type, bool bubbles   =false, Object *data=NULL);

            /** Returns if there are listeners registered for a certain event type. */
        public:
            bool     hasEventListener(std::string type);
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_EVENTDISPATCHER_AS
#endif // __cplusplus

