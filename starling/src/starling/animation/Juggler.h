#if !defined(__STARLING_SRC_STARLING_ANIMATION_JUGGLER_AS)
#define __STARLING_SRC_STARLING_ANIMATION_JUGGLER_AS
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
namespace starling
{
    namespace events
    {
        class EventDispatcher;
    }
}

/** The Juggler takes objects that implement IAnimatable (like Tweens) and executes them.
 *
 *  <p>A juggler is a simple object. It does no more than saving a list of objects implementing
 *  "IAnimatable" and advancing their time if it is told to do so (by calling its own
 *  "advanceTime"-method). When an animation is completed, it throws it away.</p>
 *
 *  <p>There is a default juggler available at the Starling class:</p>
 *
 *  <pre>
 *  var juggler:Juggler = Starling.juggler;
 *  </pre>
 *
 *  <p>You can create juggler objects yourself, just as well. That way, you can group
 *  your game into logical components that handle their animations independently. All you have
 *  to do is call the "advanceTime" method on your custom juggler once per frame.</p>
 *
 *  <p>Another handy feature of the juggler is the "delayCall"-method. Use it to
 *  execute a function at a later time. Different to conventional approaches, the method
 *  will only be called when the juggler is advanced, giving you perfect control over the
 *  call.</p>
 *
 *  <pre>
 *  juggler.delayCall(object.removeFromParent, 1.0);
 *  juggler.delayCall(object.addChild, 2.0, theChild);
 *  juggler.delayCall(function():void { doSomethingFunny(); }, 3.0);
 *  </pre>
 *
 *  @see Tween
 *  @see DelayedCall
 */
using namespace starling::core;
using namespace starling::events;
using namespace starling::events;

namespace starling
{
    namespace animation
    {
        class Juggler: public IAnimatable
        {
        private:
            std::vector<IAnimatable *> *mObjects;
        private:
            float mElapsedTime;

            /** Create an empty juggler. */
        public:
            Juggler();

            /** Adds an object to the juggler. */
        public:
            void     add(IAnimatable *object);

            /** Determines if an object has been added to the juggler. */
        public:
            bool     contains(IAnimatable *object);

            /** Removes an object from the juggler. */
        public:
            void     remove(IAnimatable *object);

            /** Removes all tweens with a certain target. */
        public:
            void     removeTweens(Object *target);

            /** Figures out if the juggler contains one or more tweens with a certain target. */
        public:
            bool     containsTweens(Object *target);

            /** Removes all objects at once. */
        public:
            void     purge();

            /** Delays the execution of a function until a certain time has passed. Creates an
             *  object of type 'DelayedCall' internally and returns it. Remove that object
             *  from the juggler to cancel the function call. */
        public:
            DelayedCall *delayCall(Function *call, float delay, ...);

            /** Utilizes a tween to animate the target object over a certain time. Internally, this
             *  method uses a tween instance (taken from an object pool) that is added to the
             *  juggler right away. This method provides a convenient alternative for creating
             *  and adding a tween manually.
             *
             *  <p>Fill 'properties' with key-value pairs that describe both the
             *  tween and the animation target. Here is an example:</p>
             *
             *  <pre>
             *  juggler.tween(object, 2.0, {
             *      transition: Transitions.EASE_IN_OUT,
             *      delay: 20, // -> tween.delay = 20
             *      x: 50      // -> tween.animate("x", 50)
             *  });
             *  </pre>
             */
        public:
            void     tween(Object *target, float time, Object *properties);

        private:
            void     onPooledTweenComplete(Event *event);

            /** Advances all objects by a certain time (in seconds). */
        public:
            void     advanceTime(float time);    // there is a high probability that the "advanceTime" function modifies the list

        private:
            void     onRemove(Event *event);

            /** The total life time of the juggler. */
        public:
            float        elapsedTime();
        };
    }
}

#endif // __STARLING_SRC_STARLING_ANIMATION_JUGGLER_AS
#endif // __cplusplus

