// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DelayedCall.h"
#include "starling/events/Event.h"
#include "starling/events/EventDispatcher.h"

#include "starling/animation/IAnimatable.h"

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
using namespace starling::animation;

namespace starling {
namespace animation {


        /** Creates a delayed call. */
        DelayedCall::DelayedCall(Function* call, float delay, std::vector<void*> args)
        {
            reset(call, delay, args);
        }

        /** Resets the delayed call to its default values, which is useful for pooling. */
        DelayedCall* DelayedCall::reset(Function* call, float delay, std::vector<void*> args)
        {
            mCurrentTime = 0;
            mTotalTime = Math::max(delay,0.0001);
            mCall = call;
            mArgs = args;
            mRepeatCount = 1;

            return this;
        }

        /** @inheritDoc */
        void DelayedCall::advanceTime(float time)
        {
             float previousTime = mCurrentTime;
            mCurrentTime = Math::min(mTotalTime,mCurrentTime + time);

            if (previousTime < mTotalTime && mCurrentTime >= mTotalTime)
            {
                mCall->apply(NULL,mArgs);

                if (mRepeatCount == 0 || mRepeatCount > 1)
                {
                    if (mRepeatCount > 0) mRepeatCount -= 1;
                    mCurrentTime = 0;
                    advanceTime((previousTime + time) - mTotalTime);
                }
                else
                {
                    dispatchEventWith(Event::REMOVE_FROM_JUGGLER);
                }
            }
        }

        /** Indicates if enough time has passed, and the call has already been executed. */
        bool DelayedCall::isComplete()
        {
            return mRepeatCount == 1 && mCurrentTime >= mTotalTime;
        }

        /** The time for which calls will be delayed (in seconds). */
        float DelayedCall::totalTime()        { return mTotalTime; }

        /** The time that has already passed (in seconds). */
        float DelayedCall::currentTime()        { return mCurrentTime; }

        /** The number of times the call will be repeated. 
         *  Set to '0' to repeat indefinitely. @default 1 */
        int DelayedCall::repeatCount()     { return mRepeatCount; }
        void DelayedCall::repeatCount(int value)      { mRepeatCount = value; }
}
}

