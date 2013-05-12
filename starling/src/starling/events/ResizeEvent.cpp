// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "ResizeEvent.h"
#include "flash/geom/Point.h"

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

namespace starling
{
    namespace events
    {


        /** Event type for a resized Flash player. */
        const std::string ResizeEvent::RESIZE="resize";

        /** Creates a new ResizeEvent. */
        ResizeEvent::ResizeEvent(std::string type, int width, int height, bool bubbles)
        {
            super(type, bubbles, new Point(width, height));
        }

        /** The updated width of the player. */
        int ResizeEvent::width()
        {
            return (static_cast<Point>(data))->x;
        }

        /** The updated height of the player. */
        int ResizeEvent::height()
        {
            return (static_cast<Point>(data))->y;
        }
    }
}

