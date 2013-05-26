#if !defined(__STARLING_SRC_STARLING_EVENTS_RESIZEEVENT_AS)
#define __STARLING_SRC_STARLING_EVENTS_RESIZEEVENT_AS
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
namespace flash
{
    namespace geom
    {
        class Point;
    }
}
#include "starling/events/Event.h"

/** A ResizeEvent is dispatched by the stage when the size of the Flash container changes.
 *  Use it to update the Starling viewport and the stage size.
 *
 *  <p>The event contains properties containing the updated width and height of the Flash
 *  player. If you want to scale the contents of your stage to fill the screen, update the
 *  <code>Starling.current.viewPort</code> rectangle accordingly. If you want to make use of
 *  the additional screen estate, update the values of <code>stage.stageWidth</code> and
 *  <code>stage.stageHeight</code> as well.</p>
 *
 *  @see starling.display.Stage
 *  @see starling.core.Starling
 */

using namespace flash::geom;
using namespace starling::events;

namespace starling
{
    namespace events
    {
        class ResizeEvent : public starling::events::Event
        {
            /** Event type for a resized Flash player. */
        public:
            static const std::string RESIZE;

            /** Creates a new ResizeEvent. */
        public:
            ResizeEvent(std::string type, int width, int height, bool bubbles=false);

            /** The updated width of the player. */
        public:
            int          width();

            /** The updated height of the player. */
        public:
            int          height();
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_RESIZEEVENT_AS
#endif // __cplusplus

