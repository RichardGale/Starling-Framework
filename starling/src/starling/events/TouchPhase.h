#if !defined(__STARLING_SRC_STARLING_EVENTS_TOUCHPHASE_AS)
#define __STARLING_SRC_STARLING_EVENTS_TOUCHPHASE_AS
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
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A class that provides constant values for the phases of a touch object.
 *
 *  <p>A touch moves through at least the following phases in its life:</p>
 *
 *  <code>BEGAN -> MOVED -> ENDED</code>
 *
 *  <p>Furthermore, a touch can enter a <code>STATIONARY</code> phase. That phase does not
 *  trigger a touch event itself, and it can only occur in multitouch environments. Picture a
 *  situation where one finger is moving and the other is stationary. A touch event will
 *  be dispatched only to the object under the <em>moving</em> finger. In the list of touches
 *  of that event, you will find the second touch in the stationary phase.</p>
 *
 *  <p>Finally, there's the <code>HOVER</code> phase, which is exclusive to mouse input. It is
 *  the equivalent of a <code>MouseOver</code> event in Flash when the mouse button is
 *  <em>not</em> pressed.</p>
 */
using namespace starling::errors;

namespace starling
{
    namespace events
    {
        class TouchPhase
        {
            /** @private */
        public:
            TouchPhase();

            /** Only available for mouse input: the cursor hovers over an object <em>without</em> a
             *  pressed button. */
        public:
            static const std::string HOVER;

            /** The finger touched the screen just now, or the mouse button was pressed. */
        public:
            static const std::string BEGAN;

            /** The finger moves around on the screen, or the mouse is moved while the button is
             *  pressed. */
        public:
            static const std::string MOVED;

            /** The finger or mouse (with pressed button) has not moved since the last frame. */
        public:
            static const std::string STATIONARY;

            /** The finger was lifted from the screen or from the mouse button. */
        public:
            static const std::string ENDED;
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_TOUCHPHASE_AS
#endif // __cplusplus

