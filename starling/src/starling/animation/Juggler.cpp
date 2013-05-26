// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Juggler.h"
#include "starling/core/starling_internal.h"
#include "starling/events/Event.h"
#include "starling/events/EventDispatcher.h"
#include "starling/animation/IAnimatable.h"
#include "starling/animation/DelayedCall.h"
#include "starling/animation/Tween.h"

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

using namespace starling::animation;
using namespace starling::core;
using namespace starling::events;

namespace starling {
namespace animation {


                    
                    

        /** Create an empty juggler. */
        Juggler::Juggler()
        {
            mElapsedTime = 0;
            mObjects.clear();
        }

        /** Adds an object to the juggler. */
        void Juggler::add(IAnimatable* object)
        {
            if (object && mObjects.indexOf(object) == -1)
            {
                mObjects.push_back(object);

                EventDispatcher* dispatcher = dynamic_cast<EventDispatcher*>(object);
                if (dispatcher) dispatcher->addEventListener(Event::REMOVE_FROM_JUGGLER, onRemove);
            }
        }

        /** Determines if an object has been added to the juggler. */
        bool Juggler::contains(IAnimatable* object)
        {
            return mObjects.indexOf(object) != -1;
        }

        /** Removes an object from the juggler. */
        void Juggler::remove(IAnimatable* object)
        {
            if (object == NULL) return;

            EventDispatcher* dispatcher = dynamic_cast<EventDispatcher*>(object);
            if (dispatcher) dispatcher->removeEventListener(Event::REMOVE_FROM_JUGGLER, onRemove);

            int index = mObjects.indexOf(object);
            if (index != -1) mObjects[index] = NULL;
        }

        /** Removes all tweens with a certain target. */
        void Juggler::removeTweens(Object* target)
        {
            if (target == NULL) return;

            for (int i=mObjects.size()-1; i>=0; --i)
            {
                Tween* tween = dynamic_cast<Tween*>(mObjects[i]);
                if (tween && tween->target() == target)
                {
                    tween->removeEventListener(Event::REMOVE_FROM_JUGGLER, onRemove);
                    mObjects[i] = NULL;
                }
            }
        }

        /** Figures out if the juggler contains one or more tweens with a certain target. */
        bool Juggler::containsTweens(Object* target)
        {
            if (target == NULL) return false;

            for (int i=mObjects.size()-1; i>=0; --i)
            {
                Tween* tween = dynamic_cast<Tween*>(mObjects[i]);
                if (tween && tween->target() == target) return true;
            }

            return false;
        }

        /** Removes all objects at once. */
        void Juggler::purge()
        {
            // the object vector is not purged right away, because if this method is called 
            // from an 'advanceTime' call, this would make the loop crash. Instead, the
            // vector is filled with 'null' values. They will be cleaned up on the next call
            // to 'advanceTime'.

            for (int i=mObjects.size()-1; i>=0; --i)
            {
                EventDispatcher* dispatcher = dynamic_cast<EventDispatcher*>(mObjects[i]);
                if (dispatcher) dispatcher->removeEventListener(Event::REMOVE_FROM_JUGGLER, onRemove);
                mObjects[i] = NULL;
            }
        }

        /** Delays the execution of a function until a certain time has passed. Creates an
         *  object of type 'DelayedCall' internally and returns it. Remove that object
         *  from the juggler to cancel the function call. */
        DelayedCall* Juggler::delayCall(Function* call, float delay, ...)
        {
            if (call == NULL) return NULL;

            DelayedCall* delayedCall = new DelayedCall(call, delay, args);
            add(delayedCall);
            return delayedCall;
        }

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
        void Juggler::tween(Object* target, float time, Object* properties)
        {
            Tween* tween = Tween::starling_internal::fromPool(target, time);

            for(std::vector<std::string>::iterator property = properties.begin(); property != properties.end(); ++property)
            {
                Object* value = properties[property];

                if (tween->hasOwnProperty(property))
                    tween[property] = value;
                else if (target->hasOwnProperty(property))
                    tween->animate(property, dynamic_cast<float>(value));
                else
                    throw new ArgumentError("Invalid property: " + property);
            }

            tween->addEventListener(Event::REMOVE_FROM_JUGGLER, onPooledTweenComplete);
            add(tween);
        }

        void Juggler::onPooledTweenComplete(flash::events::Event* event)
        {
            Tween::starling_internal::toPool(event->dynamic_cast<Tween*>(target));
        }

        /** Advances all objects by a certain time (in seconds). */
        void Juggler::advanceTime(float time)
        {
            int numObjects = mObjects.size();
            int currentIndex = 0;
            int i;

            mElapsedTime += time;
            if (numObjects == 0) return;





            for (i=0; i<numObjects; ++i)
            {
                IAnimatable* object = mObjects[i];
                if (object)
                {
                    // shift objects into empty slots along the way
                    if (currentIndex != i)
                    {
                        mObjects[currentIndex] = object;
                        mObjects[i] = NULL;
                    }

                    object->advanceTime(time);
                    ++currentIndex;
                }
            }

            if (currentIndex != i)
            {
                numObjects = mObjects.size(); // count might have changed!

                while (i < numObjects)
                    mObjects[int(currentIndex++)] = mObjects[int(i++)];

                mObjects.size() = currentIndex;
            }
        }   // there is a high probability that the "advanceTime" function modifies the list // of animatables. we must not process new objects right now (they will be processed// in the next frame), and we need to clean up any empty slots in the list.

        void Juggler::onRemove(flash::events::Event* event)
        {
            remove(event->dynamic_cast<IAnimatable*>(target));

            Tween* tween = event->dynamic_cast<Tween*>(target);
            if (tween && tween->isComplete())
                add(tween->nextTween());
        }

        /** The total life time of the juggler. */
        float Juggler::elapsedTime()        { return mElapsedTime; }
}
}

