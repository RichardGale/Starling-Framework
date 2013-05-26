#if !defined(__STARLING_SRC_STARLING_DISPLAY_SPRITE_AS)
#define __STARLING_SRC_STARLING_DISPLAY_SPRITE_AS
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
        class Matrix;
    }
}
namespace flash
{
    namespace geom
    {
        class Point;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}

namespace starling
{
    namespace core
    {
        class RenderSupport;
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
    namespace utils
    {
        class MatrixUtil;
    }
}
namespace starling
{
    namespace utils
    {
        class RectangleUtil;
    }
}

#include "starling/display/DisplayObjectContainer.h"
namespace starling
{
    namespace display
    {
        class QuadBatch;
    }
}

/** Dispatched on all children when the object is flattened. */
//[Event(name="flatten",type="starling.events.Event")]

using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::utils;

namespace starling
{
    namespace display
    {
        /** A Sprite is the most lightweight, non-abstract container class.
         *  <p>Use it as a simple means of grouping objects together in one coordinate system, or
         *  as the base class for custom display objects.</p>
         *
         *  <strong>Flattened Sprites</strong>
         *
         *  <p>The <code>flatten</code>-method allows you to optimize the rendering of static parts of
         *  your display list.</p>
         *
         *  <p>It analyzes the tree of children attached to the sprite and optimizes the rendering calls
         *  in a way that makes rendering extremely fast. The speed-up comes at a price, though: you
         *  will no longer see any changes in the properties of the children (position, rotation,
         *  alpha, etc.). To update the object after changes have happened, simply call
         *  <code>flatten</code> again, or <code>unflatten</code> the object.</p>
         *
         *  <strong>Clipping Rectangle</strong>
         *
         *  <p>The <code>clipRect</code> property allows you to clip the visible area of the sprite
         *  to a rectangular region. Only pixels inside the rectangle will be displayed. This is a very
         *  fast way to mask objects. However, there is one limitation: the <code>clipRect</code>
         *  only works with stage-aligned rectangles, i.e. you cannot rotate or skew the rectangle.
         *  This limitation is inherited from the underlying "scissoring" technique that is used
         *  internally.</p>
         *
         *  @see DisplayObject
         *  @see DisplayObjectContainer
         */
        class Sprite : public starling::display::DisplayObjectContainer
        {
        private:
            std::vector<QuadBatch *> mFlattenedContents;
        private:
            bool mFlattenRequested;
        private:
            Rectangle *mClipRect;

            /** Helper objects. */
        private:
            static Matrix *sHelperMatrix;
        private:
            static Point *sHelperPoint;
        private:
            static Rectangle *sHelperRect;

            /** Creates an empty sprite. */
        public:
            Sprite();

            /** @inheritDoc */
        public:
            virtual void     dispose();

        private:
            void     disposeFlattenedContents();

            /** Optimizes the sprite for optimal rendering performance. Changes in the
             *  children of a flattened sprite will not be displayed any longer. For this to happen,
             *  either call <code>flatten</code> again, or <code>unflatten</code> the sprite.
             *  Beware that the actual flattening will not happen right away, but right before the
             *  next rendering.
             *
             *  <p>When you flatten a sprite, the result of all matrix operations that are otherwise
             *  executed during rendering are cached. For this reason, a flattened sprite can be
             *  rendered with much less strain on the CPU. However, a flattened sprite will always
             *  produce at least one draw call; if it were merged together with other objects, this
             *  would cause additional matrix operations, and the optimization would have been in vain.
             *  Thus, don't just blindly flatten all your sprites, but reserve flattening for sprites
             *  with a big number of children.</p>
             */
        public:
            void     flatten();

            /** Removes the rendering optimizations that were created when flattening the sprite.
             *  Changes to the sprite's children will immediately become visible again. */
        public:
            void     unflatten();

            /** Indicates if the sprite was flattened. */
        public:
            bool      isFlattened();

            /** The object's clipping rectangle in its local coordinate system.
             *  Only pixels within that rectangle will be drawn.
             *  <strong>Note:</strong> clip rects are axis aligned with the screen, so they
             *  will not be rotated or skewed if the Sprite is. */
        public:
            Rectangle    *clipRect();
        public:
            void         clipRect(Rectangle *value);

            /** Returns the bounds of the container's clipRect in the given coordinate space, or
             *  null if the sprite doens't have a clipRect. */
        public:
            Rectangle *getClipRect(DisplayObject *targetSpace, Rectangle *resultRect=NULL);

            /** @inheritDoc */
        public:
            virtual Rectangle *getBounds(DisplayObject *targetSpace, Rectangle *resultRect=NULL);

            /** @inheritDoc */
        public:
            virtual DisplayObject *hitTest(Point *localPoint, bool forTouch=false);

            /** @inheritDoc */
        public:
            virtual void     render(RenderSupport *support, float parentAlpha);
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_SPRITE_AS
#endif // __cplusplus

