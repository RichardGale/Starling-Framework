#if !defined(__STARLING_SRC_STARLING_EVENTS_TOUCHEVENT_AS)
#define __STARLING_SRC_STARLING_EVENTS_TOUCHEVENT_AS
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
namespace starling { namespace core { class starling_internal; } }
namespace starling { namespace display { class DisplayObject; } }

    //use starling_internal        ;

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

namespace starling {
namespace events {
    class TouchEvent: public Event
    {
        /** Event type for touch or mouse input. */
        public: static const std::string TOUCH;

        private:  bool mShiftKey;
        private:  bool mCtrlKey;
        private:  float mTimestamp;
        private:  std::vector<EventDispatcher*>* mVisitedObjects;

        /** Helper object. */
        private: static  std::vector<Touch*>* sTouches;

        /** Creates a new TouchEvent instance. */
        public:          TouchEvent(std::string type, std::vector<Touch*>* touches, bool shiftKey   =false,
                                   bool ctrlKey   =false, bool bubbles   =true);

        /** Returns a list of touches that originated over a certain target. If you pass a
         *  'result' vector, the touches will be added to this vector instead of creating a new 
         *  object. */
        public: std::vector<Touch*>* getTouches(DisplayObject* target, std::string phase=NULL,
                                   std::vector<Touch*>* result=NULL);

        /** Returns a touch that originated over a certain target. */
        public: Touch*   getTouch(DisplayObject* target, std::string phase=NULL);

        /** Indicates if a target is currently being touched or hovered over. */
        public: bool     interactsWith(DisplayObject* target);

        // custom dispatching

        /** @private
         *  Dispatches the event along a custom bubble chain. During the lifetime of the event,
         *  each object is visited only once. */
        void dispatch(std::vector<EventDispatcher*>* chain);

        // properties

        /** The time the event occurred (in seconds since application launch). */
        public: float        timestamp();

        /** All touches that are currently available. */
        public: std::vector<Touch*>* touches();

        /** Indicates if the shift key was pressed when the event occurred. */
        public: bool         shiftKey();

        /** Indicates if the ctrl key was pressed when the event occurred. (Mac OS: Cmd or Ctrl) */
        public: bool         ctrlKey();
    };
}
}

#endif // __STARLING_SRC_STARLING_EVENTS_TOUCHEVENT_AS
#endif // __cplusplus

