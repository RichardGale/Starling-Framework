// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Event.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/starling_internal.h"
#include "starling/utils/formatString.h"

#include "starling/events/EventDispatcher.h"

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
using namespace starling::events;

namespace starling
{
    namespace events
    {


        /** Event type for a display object that is added to a parent. */
        const std::string Event::ADDED="added";
        /** Event type for a display object that is added to the stage */
        const std::string Event::ADDED_TO_STAGE="addedToStage";
        /** Event type for a display object that is entering a new frame. */
        const std::string Event::ENTER_FRAME="enterFrame";
        /** Event type for a display object that is removed from its parent. */
        const std::string Event::REMOVED="removed";
        /** Event type for a display object that is removed from the stage. */
        const std::string Event::REMOVED_FROM_STAGE="removedFromStage";
        /** Event type for a triggered button. */
        const std::string Event::TRIGGERED="triggered";
        /** Event type for a display object that is being flattened. */
        const std::string Event::FLATTEN="flatten";
        /** Event type for a resized Flash Player. */
        const std::string Event::RESIZE="resize";
        /** Event type that may be used whenever something finishes. */
        const std::string Event::COMPLETE="complete";
        /** Event type for a (re)created stage3D rendering context. */
        const std::string Event::CONTEXT3D_CREATE="context3DCreate";
        /** Event type that indicates that the root DisplayObject has been created. */
        const std::string Event::ROOT_CREATED="rootCreated";
        /** Event type for an animated object that requests to be removed from the juggler. */
        const std::string Event::REMOVE_FROM_JUGGLER="removeFromJuggler";

        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::CHANGE="change";
        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::CANCEL="cancel";
        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::SCROLL="scroll";
        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::OPEN="open";
        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::CLOSE="close";
        /** An event type to be utilized in custom events. Not used by Starling right now. */
        const std::string Event::SELECT="select";

        std::vector<Event *> *Event::sEventPool=new<Event *>[];

        /** Creates an event object that can be passed to listeners. */
        Event::Event(std::string type, bool bubbles, Object *data)
        {
            mType = type;
            mBubbles = bubbles;
            mData = data;
        }

        /** Prevents listeners at the next bubble stage from receiving the event. */
        void Event::stopPropagation()
        {
            mStopsPropagation = true;
        }

        /** Prevents any other listeners from receiving the event. */
        void Event::stopImmediatePropagation()
        {
            mStopsPropagation = mStopsImmediatePropagation = true;
        }

        /** Returns a description of the event, containing type and bubble information. */
        std::string Event::toString()
        {
            return formatString("[{0} type=\"{1}\" bubbles={2}]",
                                getQualifiedClassName(this)->split("::")->pop(),mType,mBubbles);
        }

        /** Indicates if event will bubble. */
        bool Event::bubbles()
        {
            return mBubbles;
        }

        /** The object that dispatched the event. */
        EventDispatcher *Event::target()
        {
            return mTarget;
        }

        /** The object the event is currently bubbling at. */
        EventDispatcher *Event::currentTarget()
        {
            return mCurrentTarget;
        }

        /** A string that identifies the event. */
        std::string Event::type()
        {
            return mType;
        }

        /** Arbitrary data that is attached to the event. */
        Object *Event::data()
        {
            return mData;
        }

        // properties for internal use

        /** @private */
        void Event::setTarget(EventDispatcher *value)
        {
            mTarget = value;
        }

        /** @private */
        void Event::setCurrentTarget(EventDispatcher *value)
        {
            mCurrentTarget = value;
        }

        /** @private */
        void Event::setData(Object *value)
        {
            mData = value;
        }

        /** @private */
        bool Event::stopsPropagation()
        {
            return mStopsPropagation;
        }

        /** @private */
        bool Event::stopsImmediatePropagation()
        {
            return mStopsImmediatePropagation;
        }

        // event pooling

        /** @private */
        Event *Event::fromPool(std::string type, bool bubbles, Object *data)
        {
            if (sEventPool.length) return sEventPool.pop()->reset(type,bubbles, data);
            else return new Event(type, bubbles, data);
        }

        /** @private */
        void Event::toPool(Event *event)
        {
            event->mData= event->mTarget= event->mCurrentTarget= NULL;
            sEventPool.push_back(event);
        }

        /** @private */
        Event *Event::reset(std::string type, bool bubbles, Object *data)
        {
            mType = type;
            mBubbles = bubbles;
            mData = data;
            mTarget = mCurrentTarget = NULL;
            mStopsPropagation = mStopsImmediatePropagation = false;
            return this;
        }
    }
}

