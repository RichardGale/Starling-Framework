#if !defined(__STARLING_SRC_STARLING_EVENTS_EVENT_AS)
#define __STARLING_SRC_STARLING_EVENTS_EVENT_AS
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



namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
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
    namespace utils
    {
        class formatString;
    }
}

//use starling_internal        ;

/** Event objects are passed as parameters to event listeners when an event occurs.
 *  This is Starling's version of the Flash Event class.
 *
 *  <p>EventDispatchers create instances of this class and send them to registered listeners.
 *  An event object contains information that characterizes an event, most importantly the
 *  event type and if the event bubbles. The target of an event is the object that
 *  dispatched it.</p>
 *
 *  <p>For some event types, this information is sufficient; other events may need additional
 *  information to be carried to the listener. In that case, you can subclass "Event" and add
 *  properties with all the information you require. The "EnterFrameEvent" is an example for
 *  this practice; it adds a property about the time that has passed since the last frame.</p>
 *
 *  <p>Furthermore, the event class contains methods that can stop the event from being
 *  processed by other listeners - either completely or at the next bubble stage.</p>
 *
 *  @see EventDispatcher
 */
using namespace flash::utils;
using namespace starling::core;
using namespace starling::utils;

namespace starling
{
    namespace events
    {
        class Event
        {
            /** Event type for a display object that is added to a parent. */
        public:
            static const std::string ADDED;
            /** Event type for a display object that is added to the stage */
        public:
            static const std::string ADDED_TO_STAGE;
            /** Event type for a display object that is entering a new frame. */
        public:
            static const std::string ENTER_FRAME;
            /** Event type for a display object that is removed from its parent. */
        public:
            static const std::string REMOVED;
            /** Event type for a display object that is removed from the stage. */
        public:
            static const std::string REMOVED_FROM_STAGE;
            /** Event type for a triggered button. */
        public:
            static const std::string TRIGGERED;
            /** Event type for a display object that is being flattened. */
        public:
            static const std::string FLATTEN;
            /** Event type for a resized Flash Player. */
        public:
            static const std::string RESIZE;
            /** Event type that may be used whenever something finishes. */
        public:
            static const std::string COMPLETE;
            /** Event type for a (re)created stage3D rendering context. */
        public:
            static const std::string CONTEXT3D_CREATE;
            /** Event type that indicates that the root DisplayObject has been created. */
        public:
            static const std::string ROOT_CREATED;
            /** Event type for an animated object that requests to be removed from the juggler. */
        public:
            static const std::string REMOVE_FROM_JUGGLER;

            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string CHANGE;
            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string CANCEL;
            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string SCROLL;
            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string OPEN;
            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string CLOSE;
            /** An event type to be utilized in custom events. Not used by Starling right now. */
        public:
            static const std::string SELECT;

        private:
            static std::vector<Event *> *sEventPool;

        private:
            EventDispatcher *mTarget;
        private:
            EventDispatcher *mCurrentTarget;
        private:
            std::string mType;
        private:
            bool mBubbles;
        private:
            bool mStopsPropagation;
        private:
            bool mStopsImmediatePropagation;
        private:
            Object *mData;

            /** Creates an event object that can be passed to listeners. */
        public:
            Event(std::string type, bool bubbles, Object *data);

            /** Prevents listeners at the next bubble stage from receiving the event. */
        public:
            void     stopPropagation();

            /** Prevents any other listeners from receiving the event. */
        public:
            void     stopImmediatePropagation();

            /** Returns a description of the event, containing type and bubble information. */
        public:
            std::string toString();

            /** Indicates if event will bubble. */
        public:
            bool         bubbles();

            /** The object that dispatched the event. */
        public:
            EventDispatcher *target();

            /** The object the event is currently bubbling at. */
        public:
            EventDispatcher *currentTarget();

            /** A string that identifies the event. */
        public:
            std::string  type();

            /** Arbitrary data that is attached to the event. */
        public:
            Object      *data();

            // properties for internal use

            /** @private */
            friend void     setTarget(EventDispatcher *value);

            /** @private */
            friend void     setCurrentTarget(EventDispatcher *value);

            /** @private */
            friend void     setData(Object *value);

            /** @private */
            friend bool         stopsPropagation();

            /** @private */
            friend bool         stopsImmediatePropagation();

            // event pooling

            /** @private */
            starling_internal static Event   *fromPool(std::string type, bool bubbles, Object *data);

            /** @private */
            starling_internal static void     toPool(Event *event);

            /** @private */
            starling_internal Event   *reset(std::string type, bool bubbles, Object *data);
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_EVENT_AS
#endif // __cplusplus

