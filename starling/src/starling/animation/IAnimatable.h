#if !defined(__STARLING_SRC_STARLING_ANIMATION_IANIMATABLE_AS)
#define __STARLING_SRC_STARLING_ANIMATION_IANIMATABLE_AS
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
















namespace starling {
namespace animation {
    class/**  The IAnimatable interface describes objects that are animated depending on the passed time. 
     *   Any object that implements this interface can be added to a juggler.
     *   
     *   <p>When an object should no longer be animated, it has to be removed from the juggler. 
     *   To do this, you can manually remove it via the method <code>juggler.remove(object)</code>,
     *   or the object can request to be removed by dispatching a Starling event with the type
     *   <code>Event.REMOVE_FROM_JUGGLER</code>. The "Tween" class is an example of a class that
     *   dispatches such an event; you don't have to remove tweens manually from the juggler.</p> 
     *   
     *   @see Juggler
     *   @see Tween
     */              IAnimatable{/** Advance the time by a number of seconds. @param time in seconds. */virtual void advanceTime(float time) = 0;
    };
}
}

#endif // __STARLING_SRC_STARLING_ANIMATION_IANIMATABLE_AS
#endif // __cplusplus

