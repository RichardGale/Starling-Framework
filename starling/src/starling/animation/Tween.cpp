// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================




#include "Tween.h"
#include "starling/core/starling_internal.h"
#include "starling/events/Event.h"
#include "starling/events/EventDispatcher.h"
#include "Transitions.h"

/** A Tween animates numeric properties of objects. It uses different transition functions
 *  to give the animations various styles.
 *
 *  <p>The primary use of this class is to do standard animations like movement, fading,
 *  rotation, etc. But there are no limits on what to animate; as long as the property you want
 *  to animate is numeric (<code>int, uint, Number</code>), the tween can handle it. For a list
 *  of available Transition types, look at the "Transitions" class.</p>
 *
 *  <p>Here is an example of a tween that moves an object to the right, rotates it, and
 *  fades it out:</p>
 *
 *  <listing>
 *  var tween:Tween = new Tween(object, 2.0, Transitions.EASE_IN_OUT);
 *  tween.animate("x", object.x + 50);
 *  tween.animate("rotation", deg2rad(45));
 *  tween.fadeTo(0);    // equivalent to 'animate("alpha", 0)'
 *  Starling.juggler.add(tween);</listing>
 *
 *  <p>Note that the object is added to a juggler at the end of this sample. That's because a
 *  tween will only be executed if its "advanceTime" method is executed regularly - the
 *  juggler will do that for you, and will remove the tween when it is finished.</p>
 *
 *  @see Juggler
 *  @see Transitions
 */
using namespace starling::core;
using namespace starling::events;
using namespace starling::events;

namespace starling
{
    namespace animation
    {


        /** Creates a tween with a target, duration (in seconds) and a transition function.
         *  @param target the object that you want to animate
         *  @param time the duration of the Tween
         *  @param transition can be either a String (e.g. one of the constants defined in the
         *         Transitions class) or a function. Look up the 'Transitions' class for a
         *         documentation about the required function signature. */
        Tween::Tween(Object *target, float time, Object *transition)
        {
            reset(target, time, transition);
        }

        /** Resets the tween to its default values. Useful for pooling tweens. */
        Tween *Tween::reset(Object *target, float time, Object *transition)
        {
            mTarget = target;
            mCurrentTime = 0.0;
            mTotalTime = Math::max(0.0001,time);
            mProgress = 0.0;
            mDelay = mRepeatDelay = 0.0;
            mOnStart = NULL;
            mOnUpdate = NULL;
            mOnComplete = NULL;
            mOnStartArgs.clear();
            mOnUpdateArgs.clear();
            mOnCompleteArgs.clear();
            mRoundToInt = mReverse = false;
            mRepeatCount = 1;
            mCurrentCycle = -1;

            if (dynamic_cast<std::string>(transition))
                this->transition= static_cast<std::string>(transition);
            else if (dynamic_cast<Function>(transition))
                this->transitionFunc= static_cast<Function>(transition);
            else
                throw new ArgumentError("Transition must be either a string or a function");

            if (mProperties)  mProperties.clear()      else mProperties.clear();
            if (mStartValues) mStartValues.clear()     else mStartValues.clear();
            if (mEndValues)   mEndValues.clear()       else mEndValues.clear();

            return this;
        }

        /** Animates the property of the target to a certain value. You can call this method multiple
         *  times on one tween. */
        void Tween::animate(std::string property, float endValue)
        {
            if (mTarget == NULL) return;

            mProperties.push_back(property);
            mStartValues.push_back(Number::NaN);
            mEndValues.push_back(endValue);
        }

        /** Animates the 'scaleX' and 'scaleY' properties of an object simultaneously. */// tweening null just does nothing.
        void Tween::scaleTo(float factor)
        {
            animate("scaleX", factor);
            animate("scaleY", factor);
        }

        /** Animates the 'x' and 'y' properties of an object simultaneously. */
        void Tween::moveTo(float x, float y)
        {
            animate("x", x);
            animate("y", y);
        }

        /** Animates the 'alpha' property of an object to a certain target value. */
        void Tween::fadeTo(float alpha)
        {
            animate("alpha", alpha);
        }

        /** @inheritDoc */
        void Tween::advanceTime(float time)
        {
            if (time == 0 || (mRepeatCount == 1 && mCurrentTime == mTotalTime)) return;

            int i;
            float previousTime = mCurrentTime;
            float restTime = mTotalTime - mCurrentTime;
            float carryOverTime = time > restTime ? time - restTime : 0.0;

            mCurrentTime = Math::min(mTotalTime,mCurrentTime + time);

            if (mCurrentTime <= 0) return;

            if (mCurrentCycle < 0 && previousTime <= 0 && mCurrentTime > 0)
            {
                mCurrentCycle++;
                if (mOnStart != NULL) mOnStart->apply(NULL,mOnStartArgs);
            }

            float ratio = mCurrentTime / mTotalTime;
            bool reversed   = mReverse && (mCurrentCycle % 2 == 1);
            int numProperties= mStartValues.length;
            mProgress = reversed ? mTransitionFunc(1.0 - ratio) : mTransitionFunc(ratio);

            for (i=0; i<numProperties; ++i)
            {
                if (isNaN(mStartValues[i]))
                    mStartValues[i] = static_cast<float>(mTarget[mProperties[i]]);

                float startValue = mStartValues[i];
                float endValue = mEndValues[i];
                float delta = endValue - startValue;
                float currentValue = startValue + mProgress * delta;

                if (mRoundToInt) currentValue = Math::round(currentValue);
                mTarget[mProperties[i]] = currentValue;
            }

            if (mOnUpdate != NULL)
                mOnUpdate->apply(NULL,mOnUpdateArgs);

            if (previousTime < mTotalTime && mCurrentTime >= mTotalTime)
            {
                if (mRepeatCount == 0 || mRepeatCount > 1)
                {
                    mCurrentTime = -mRepeatDelay;
                    mCurrentCycle++;
                    if (mRepeatCount > 1) mRepeatCount--;
                    if (mOnRepeat != NULL) mOnRepeat->apply(NULL,mOnRepeatArgs);
                }
                else
                {
                    // save callback & args: they might be changed through an event listener
                    Function *onComplete=mOnComplete;
                    std::vector<void *> onCompleteArgs=mOnCompleteArgs;

                    // in the 'onComplete' callback, people might want to call "tween.reset" and
                    // add it to another juggler; so this event has to be dispatched *before*
                    // executing 'onComplete'.
                    dispatchEventWith(Event::REMOVE_FROM_JUGGLER);
                    if (onComplete != NULL) onComplete->apply(NULL,onCompleteArgs);
                }
            }

            if (carryOverTime)
                advanceTime(carryOverTime);
        }

        /** The end value a certain property is animated to. Throws an ArgumentError if the
         *  property is not being animated. */// the delay is not over yet
        float Tween::getEndValue(std::string property)
        {
            int index= mProperties.indexOf(property);
            if (index == -1) throw new ArgumentError("The property '" + property + "' is not animated");
            else return static_cast<float>(mEndValues[index]);
        }

        /** Indicates if the tween is finished. */
        bool Tween::isComplete()
        {
            return mCurrentTime >= mTotalTime && mRepeatCount == 1;
        }

        /** The target object that is animated. */
        Object *Tween::target()
        {
            return mTarget;
        }

        /** The transition method used for the animation. @see Transitions */
        std::string Tween::transition()
        {
            return mTransitionName;
        }
        void Tween::transition(std::string value)
        {
            mTransitionName = value;
            mTransitionFunc = Transitions::getTransition(value);

            if (mTransitionFunc == NULL)
                throw new ArgumentError("Invalid transiton: " + value);
        }

        /** The actual transition function used for the animation. */
        Function *Tween::transitionFunc()
        {
            return mTransitionFunc;
        }
        void Tween::transitionFunc(Function *value)
        {
            mTransitionName = "custom";
            mTransitionFunc = value;
        }

        /** The total time the tween will take per repetition (in seconds). */
        float Tween::totalTime()
        {
            return mTotalTime;
        }

        /** The time that has passed since the tween was created. */
        float Tween::currentTime()
        {
            return mCurrentTime;
        }

        /** The current progress between 0 and 1, as calculated by the transition function. */
        float Tween::progress()
        {
            return mProgress;
        }

        /** The delay before the tween is started. @default 0 */
        float Tween::delay()
        {
            return mDelay;
        }
        void Tween::delay(float value)
        {
            mCurrentTime = mCurrentTime + mDelay - value;
            mDelay = value;
        }

        /** The number of times the tween will be executed.
         *  Set to '0' to tween indefinitely. @default 1 */
        int Tween::repeatCount()
        {
            return mRepeatCount;
        }
        void Tween::repeatCount(int value)
        {
            mRepeatCount = value;
        }

        /** The amount of time to wait between repeat cycles, in seconds. @default 0 */
        float Tween::repeatDelay()
        {
            return mRepeatDelay;
        }
        void Tween::repeatDelay(float value)
        {
            mRepeatDelay = value;
        }

        /** Indicates if the tween should be reversed when it is repeating. If enabled,
         *  every second repetition will be reversed. @default false */
        bool Tween::reverse()
        {
            return mReverse;
        }
        void Tween::reverse(bool value)
        {
            mReverse = value;
        }

        /** Indicates if the numeric values should be cast to Integers. @default false */
        bool Tween::roundToInt()
        {
            return mRoundToInt;
        }
        void Tween::roundToInt(bool value)
        {
            mRoundToInt = value;
        }

        /** A function that will be called when the tween starts (after a possible delay). */
        Function *Tween::onStart()
        {
            return mOnStart;
        }
        void Tween::onStart(Function *value)
        {
            mOnStart = value;
        }

        /** A function that will be called each time the tween is advanced. */
        Function *Tween::onUpdate()
        {
            return mOnUpdate;
        }
        void Tween::onUpdate(Function *value)
        {
            mOnUpdate = value;
        }

        /** A function that will be called each time the tween finishes one repetition
         *  (except the last, which will trigger 'onComplete'). */
        Function *Tween::onRepeat()
        {
            return mOnRepeat;
        }
        void Tween::onRepeat(Function *value)
        {
            mOnRepeat = value;
        }

        /** A function that will be called when the tween is complete. */
        Function *Tween::onComplete()
        {
            return mOnComplete;
        }
        void Tween::onComplete(Function *value)
        {
            mOnComplete = value;
        }

        /** The arguments that will be passed to the 'onStart' function. */
        std::vector<void *> Tween::onStartArgs()
        {
            return mOnStartArgs;
        }
        void Tween::onStartArgs(std::vector<void *> value)
        {
            mOnStartArgs = value;
        }

        /** The arguments that will be passed to the 'onUpdate' function. */
        std::vector<void *> Tween::onUpdateArgs()
        {
            return mOnUpdateArgs;
        }
        void Tween::onUpdateArgs(std::vector<void *> value)
        {
            mOnUpdateArgs = value;
        }

        /** The arguments that will be passed to the 'onRepeat' function. */
        std::vector<void *> Tween::onRepeatArgs()
        {
            return mOnRepeatArgs;
        }
        void Tween::onRepeatArgs(std::vector<void *> value)
        {
            mOnRepeatArgs = value;
        }

        /** The arguments that will be passed to the 'onComplete' function. */
        std::vector<void *> Tween::onCompleteArgs()
        {
            return mOnCompleteArgs;
        }
        void Tween::onCompleteArgs(std::vector<void *> value)
        {
            mOnCompleteArgs = value;
        }

        /** Another tween that will be started (i.e. added to the same juggler) as soon as
         *  this tween is completed. */
        Tween *Tween::nextTween()
        {
            return mNextTween;
        }
        void Tween::nextTween(Tween *value)
        {
            mNextTween = value;
        }

        // tween pooling

        std::vector<Tween *> *Tween::sTweenPool=new<Tween *>[];

        /** @private */
        Tween *Tween::fromPool(Object *target, float time,
                               Object *transition)
        {
            if (sTweenPool.length) return sTweenPool.pop()->reset(target,time, transition);
            else return new Tween(target, time, transition);
        }

        /** @private */
        void Tween::toPool(Tween *tween)
        {
            // reset any object-references, to make sure we don't prevent any garbage collection
            tween->mOnStart= NULL;
            tween->mOnUpdate= NULL;
            tween->mOnRepeat= NULL;
            tween->mOnComplete= NULL;
            tween->mOnStartArgs= NULL;
            tween->mOnUpdateArgs= NULL;
            tween->mOnRepeatArgs= NULL;
            tween->mOnCompleteArgs= NULL;
            tween->mTarget= NULL;
            tween->mTransitionFunc= NULL;
            tween->removeEventListeners();
            sTweenPool.push_back(tween);
        }
    }
}

