// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Sprite.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/core/RenderSupport.h"
#include "starling/events/Event.h"
#include "starling/utils/MatrixUtil.h"
#include "starling/utils/RectangleUtil.h"

#include "starling/display/DisplayObjectContainer.h"
#include "starling/display/QuadBatch.h"

    /** Dispatched on all children when the object is flattened. */

using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::utils;

namespace starling {
namespace display {


                    
                    
                    

        /** Helper objects. */
        Matrix* Sprite::sHelperMatrix = new Matrix();
        Point* Sprite::sHelperPoint = new Point();
        Rectangle* Sprite::sHelperRect = new Rectangle();

        /** Creates an empty sprite. */
        Sprite::Sprite()
        {
            DisplayObjectContainer();
        }

        /** @inheritDoc */
        void Sprite::dispose()
        {
            disposeFlattenedContents();
            DisplayObjectContainer::dispose();
        }

        void Sprite::disposeFlattenedContents()
        {
            if (mFlattenedContents)
            {
                for (int i=0, int max=mFlattenedContents.size(); i<max; ++i)
                    mFlattenedContents[i]->dispose();

                mFlattenedContents.clear();
            }
        }

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
        void Sprite::flatten()
        {
            mFlattenRequested = true;
            broadcastEventWith(Event::FLATTEN);
        }

        /** Removes the rendering optimizations that were created when flattening the sprite.
         *  Changes to the sprite's children will immediately become visible again. */
        void Sprite::unflatten()
        {
            mFlattenRequested = false;
            disposeFlattenedContents();
        }

        /** Indicates if the sprite was flattened. */
        bool Sprite::isFlattened()
        {
            return (!mFlattenedContents.empty()) || mFlattenRequested;
        }

        /** The object's clipping rectangle in its local coordinate system.
         *  Only pixels within that rectangle will be drawn. 
         *  <strong>Note:</strong> clip rects are axis aligned with the screen, so they
         *  will not be rotated or skewed if the Sprite is. */
        Rectangle* Sprite::clipRect()           { return mClipRect; }
        void Sprite::clipRect(Rectangle* value)
        {
            if (mClipRect && value) mClipRect->copyFrom(value);
            else mClipRect = (value ? value->clone() : NULL);
        }

        /** Returns the bounds of the container's clipRect in the given coordinate space, or
         *  null if the sprite doens't have a clipRect. */
        Rectangle* Sprite::getClipRect(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            if (mClipRect == NULL) return NULL;
            if (resultRect == NULL) resultRect = new Rectangle();

            float minX =  Number::MAX_VALUE;
            float maxX = -Number::MAX_VALUE;
            float minY =  Number::MAX_VALUE;
            float maxY = -Number::MAX_VALUE;

            Matrix* transMatrix = getTransformationMatrix(targetSpace, sHelperMatrix);
            float x = 0;
            float y = 0;
            for (int i=0; i<4; ++i)
            {
                switch(i)
                {
                    case 0: x = mClipRect->left();  y = mClipRect->top();    break;
                    case 1: x = mClipRect->left();  y = mClipRect->bottom(); break;
                    case 2: x = mClipRect->right(); y = mClipRect->top();    break;
                    case 3: x = mClipRect->right(); y = mClipRect->bottom(); break;
                }
                Point* transformedPoint = MatrixUtil::transformCoords(transMatrix, x, y, sHelperPoint);
                minX = Math::min(minX, transformedPoint->x);
                maxX = Math::max(maxX, transformedPoint->x);
                minY = Math::min(minY, transformedPoint->y);
                maxY = Math::max(maxY, transformedPoint->y);
            }

            resultRect->setTo(minX, minY, maxX-minX, maxY-minY);
            return resultRect;
        }

        /** @inheritDoc */
        Rectangle* Sprite::getBounds(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            Rectangle* bounds = DisplayObjectContainer::getBounds(targetSpace, resultRect);

            // if we have a scissor rect, intersect it with our bounds
            if (mClipRect)
                RectangleUtil::intersect(bounds, getClipRect(targetSpace, sHelperRect),
                                        bounds);

            return bounds;
        }

        /** @inheritDoc */
        DisplayObject* Sprite::hitTest(Point* localPoint, bool forTouch)
        {
            if (mClipRect != NULL && !mClipRect->containsPoint(localPoint))
                return NULL;
            else
                return DisplayObjectContainer::hitTest(localPoint, forTouch);
        }

        /** @inheritDoc */
        void Sprite::render(RenderSupport* support, float parentAlpha)
        {
            if (mClipRect)
            {
                Rectangle* clipRect = support->pushClipRect(getClipRect(stage, sHelperRect));
                if (clipRect->isEmpty())
                {
                    // empty clipping bounds - no need to render children.
                    support->popClipRect();
                    return;
                }
            }

            if (mFlattenedContents || mFlattenRequested)
            {
                if (mFlattenedContents.empty())
                    mFlattenedContents.clear();

                if (mFlattenRequested)
                {
                    QuadBatch::compile(this, mFlattenedContents);
                    support->applyClipRect(); // compiling filters might change scissor rect. :-\
                    mFlattenRequested = false;
                }

                float alpha = parentAlpha * this->alpha;
                int numBatches = mFlattenedContents.size();
                Matrix* mvpMatrix = support->mvpMatrix;

                support->finishQuadBatch();
                support->raiseDrawCount(numBatches);

                for (int i=0; i<numBatches; ++i)
                {
                    QuadBatch* quadBatch = mFlattenedContents[i];
                    std::string blendMode = quadBatch->blendMode() == BlendMode()->AUTO ?
                        support->blendMode : quadBatch->blendMode;
                    quadBatch->renderCustom(mvpMatrix, alpha, blendMode);
                }
            }
            else DisplayObjectContainer::render(support, parentAlpha);

            if (mClipRect)
                support->popClipRect();
        }
}
}

