#if !defined(__STARLING_SRC_STARLING_ANIMATION_TWEEN_AS)
#define __STARLING_SRC_STARLING_ANIMATION_TWEEN_AS
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




#include "flex11.6.h"
namespace starling
{
    namespace core
    {
        class starling_internal;
    }
}
namespace starling
{
    namespace events
    {
        class Event;
    }
}
#include "starling/events/EventDispatcher.h"
namespace starling
{
    namespace animation
    {
        class Transitions;
    }
}
#include "starling/animation/IAnimatable.h"

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

using namespace starling::animation;
using namespace starling::core;
using namespace starling::events;

namespace starling
{
    namespace animation
    {
        class Tween : public starling::events::EventDispatcher, public starling::animation::IAnimatable
        {
        private:
            Object *mTarget;
        private:
            Function *mTransitionFunc;
        private:
            std::string mTransitionName;

        private:
            std::vector<std::string> mProperties;
        private:
            std::vector<float> mStartValues;
        private:
            std::vector<float> mEndValues;

        private:
            Function *mOnStart;
        private:
            Function *mOnUpdate;
        private:
            Function *mOnRepeat;
        private:
            Function *mOnComplete;

        private:
            std::vector<void *> mOnStartArgs;
        private:
            std::vector<void *> mOnUpdateArgs;
        private:
            std::vector<void *> mOnRepeatArgs;
        private:
            std::vector<void *> mOnCompleteArgs;

        private:
            float mTotalTime;
        private:
            float mCurrentTime;
        private:
            float mProgress;
        private:
            float mDelay;
        private:
            bool mRoundToInt;
        private:
            Tween *mNextTween;
        private:
            int mRepeatCount;
        private:
            float mRepeatDelay;
        private:
            bool mReverse;
        private:
            int mCurrentCycle;

            /** Creates a tween with a target, duration (in seconds) and a transition function.
             *  @param target the object that you want to animate
             *  @param time the duration of the Tween
             *  @param transition can be either a String (e.g. one of the constants defined in the
             *         Transitions class) or a function. Look up the 'Transitions' class for a
             *         documentation about the required function signature. */
        public:
            Tween(Object *target, float time, Object *transition=(Object *)"linear");

            /** Resets the tween to its default values. Useful for pooling tweens. */
        public:
            Tween    *reset(Object *target, float time, Object *transition=(Object *)"linear");

            /** Animates the property of the target to a certain value. You can call this method multiple
             *  times on one tween. */
        public:
            void     animate(std::string property, float endValue);

            /** Animates the 'scaleX' and 'scaleY' properties of an object simultaneously. */// tweening null just does nothing.
        public:
            void     scaleTo(float factor);

            /** Animates the 'x' and 'y' properties of an object simultaneously. */
        public:
            void     moveTo(float x, float y);

            /** Animates the 'alpha' property of an object to a certain target value. */
        public:
            void     fadeTo(float alpha);

            /** @inheritDoc */
        public:
            void     advanceTime(float time);

            /** The end value a certain property is animated to. Throws an ArgumentError if the
             *  property is not being animated. */// the delay is not over yet
        public:
            float   getEndValue(std::string property);

            /** Indicates if the tween is finished. */
        public:
            bool      isComplete();

            /** The target object that is animated. */
        public:
            Object       *target();

            /** The transition method used for the animation. @see Transitions */
        public:
            std::string       transition();
        public:
            void         transition(std::string value);

            /** The actual transition function used for the animation. */
        public:
            Function     *transitionFunc();
        public:
            void         transitionFunc(Function *value);

            /** The total time the tween will take per repetition (in seconds). */
        public:
            float       totalTime();

            /** The time that has passed since the tween was created. */
        public:
            float       currentTime();

            /** The current progress between 0 and 1, as calculated by the transition function. */
        public:
            float       progress();

            /** The delay before the tween is started. @default 0 */
        public:
            float       delay();
        public:
            void         delay(float value);

            /** The number of times the tween will be executed.
             *  Set to '0' to tween indefinitely. @default 1 */
        public:
            int          repeatCount();
        public:
            void         repeatCount(int value);

            /** The amount of time to wait between repeat cycles, in seconds. @default 0 */
        public:
            float       repeatDelay();
        public:
            void         repeatDelay(float value);

            /** Indicates if the tween should be reversed when it is repeating. If enabled,
             *  every second repetition will be reversed. @default false */
        public:
            bool      reverse();
        public:
            void         reverse(bool value);

            /** Indicates if the numeric values should be cast to Integers. @default false */
        public:
            bool      roundToInt();
        public:
            void         roundToInt(bool value);

            /** A function that will be called when the tween starts (after a possible delay). */
        public:
            Function     *onStart();
        public:
            void         onStart(Function *value);

            /** A function that will be called each time the tween is advanced. */
        public:
            Function     *onUpdate();
        public:
            void         onUpdate(Function *value);

            /** A function that will be called each time the tween finishes one repetition
             *  (except the last, which will trigger 'onComplete'). */
        public:
            Function     *onRepeat();
        public:
            void         onRepeat(Function *value);

            /** A function that will be called when the tween is complete. */
        public:
            Function     *onComplete();
        public:
            void         onComplete(Function *value);

            /** The arguments that will be passed to the 'onStart' function. */
        public:
            std::vector<void *>        onStartArgs();
        public:
            void         onStartArgs(std::vector<void *> value);

            /** The arguments that will be passed to the 'onUpdate' function. */
        public:
            std::vector<void *>        onUpdateArgs();
        public:
            void         onUpdateArgs(std::vector<void *> value);

            /** The arguments that will be passed to the 'onRepeat' function. */
        public:
            std::vector<void *>        onRepeatArgs();
        public:
            void         onRepeatArgs(std::vector<void *> value);

            /** The arguments that will be passed to the 'onComplete' function. */
        public:
            std::vector<void *>        onCompleteArgs();
        public:
            void         onCompleteArgs(std::vector<void *> value);

            /** Another tween that will be started (i.e. added to the same juggler) as soon as
             *  this tween is completed. */
        public:
            Tween        *nextTween();
        public:
            void         nextTween(Tween *value);

            // tween pooling

        private:
            static std::vector<Tween *> sTweenPool;

            /** @private */
            static Tween    *fromPool(Object *target, float time,
                                      Object *transition=(Object *)"linear");

            /** @private */
            static void     toPool(Tween *tween);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ANIMATION_TWEEN_AS
#endif // __cplusplus

