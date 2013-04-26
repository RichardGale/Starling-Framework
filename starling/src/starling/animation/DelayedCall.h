#if !defined(__STARLING_SRC_STARLING_ANIMATION_DELAYEDCALL_AS)
#define __STARLING_SRC_STARLING_ANIMATION_DELAYEDCALL_AS
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



namespace starling
{
    namespace events
    {
        class Event;
    }
}
#include "starling/events/EventDispatcher.h"

/** A DelayedCall allows you to execute a method after a certain time has passed. Since it
 *  implements the IAnimatable interface, it can be added to a juggler. In most cases, you
 *  do not have to use this class directly; the juggler class contains a method to delay
 *  calls directly.
 *
 *  <p>DelayedCall dispatches an Event of type 'Event.REMOVE_FROM_JUGGLER' when it is finished,
 *  so that the juggler automatically removes it when its no longer needed.</p>
 *
 *  @see Juggler
 */
using namespace starling::events;
using namespace starling::events;

namespace starling
{
    namespace animation
    {
        class DelayedCall: public EventDispatcher, public IAnimatable
        {
        private:
            float mCurrentTime;
        private:
            float mTotalTime;
        private:
            Function *mCall;
        private:
            std::vector<void *> mArgs;
        private:
            int mRepeatCount;

            /** Creates a delayed call. */
        public:
            DelayedCall(Function *call, float delay, std::vector<void *> args);

            /** Resets the delayed call to its default values, which is useful for pooling. */
        public:
            DelayedCall *reset(Function *call, float delay, std::vector<void *> args);

            /** @inheritDoc */
        public:
            void     advanceTime(float time);

            /** Indicates if enough time has passed, and the call has already been executed. */
        public:
            bool         isComplete();

            /** The time for which calls will be delayed (in seconds). */
        public:
            float        totalTime();

            /** The time that has already passed (in seconds). */
        public:
            float        currentTime();

            /** The number of times the call will be repeated.
             *  Set to '0' to repeat indefinitely. @default 1 */
        public:
            int          repeatCount();
        public:
            void         repeatCount(int value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ANIMATION_DELAYEDCALL_AS
#endif // __cplusplus

