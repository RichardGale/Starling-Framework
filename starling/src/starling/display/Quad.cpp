// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Quad.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/core/RenderSupport.h"
#include "starling/utils/VertexData.h"
#include "starling/display/DisplayObject.h"

    /** A Quad represents a rectangle with a uniform color or a color gradient.
     *  
     *  <p>You can set one color per vertex. The colors will smoothly fade into each other over the area
     *  of the quad. To display a simple linear color gradient, assign one color to vertices 0 and 1 and 
     *  another color to vertices 2 and 3. </p> 
     *
     *  <p>The indices of the vertices are arranged like this:</p>
     *  
     *  <pre>
     *  0 - 1
     *  | / |
     *  2 - 3
     *  </pre>
     * 
     *  @see Image
     */

using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::utils;

namespace starling {
namespace display {


                    

        /** The raw vertex data of the quad. */
                      

        /** Helper objects. */
         Point* Quad::sHelperPoint= new Point();
         Matrix* Quad::sHelperMatrix= new Matrix();

        /** Creates a quad with a certain size and color. The last parameter controls if the 
         *  alpha value should be premultiplied into the color values on rendering, which can
         *  influence blending output. You can use the default value in most cases.  */
        Quad::Quad(float width, float height, unsigned int color,
                             bool premultipliedAlpha)
        {
            mTinted = color != 0xffffff;

            mVertexData = new VertexData(4, premultipliedAlpha);
            mVertexData->setPosition(0, 0.0, 0.0);
            mVertexData->setPosition(1, width, 0.0);
            mVertexData->setPosition(2, 0.0, height);
            mVertexData->setPosition(3, width, height);
            mVertexData->setUniformColor(color);

            onVertexDataChanged();
        }

        /** Call this method after manually changing the contents of 'mVertexData'. */
        void Quad::onVertexDataChanged()
        {
            // override in subclasses, if necessary
        }

        /** @inheritDoc */
        Rectangle* Quad::getBounds(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            if (resultRect == NULL) resultRect = new Rectangle();

            if (targetSpace == this) // optimization
            {
                mVertexData->getPosition(3, sHelperPoint);
                resultRect->setTo(0.0, 0.0, sHelperPoint->x(), sHelperPoint->y());
            }
            else if (targetSpace == parent && rotation == 0.0)
            {
                 float scaleX  = this()->scaleX;
                 float scaleY  = this()->scaleY;
                mVertexData->getPosition(3, sHelperPoint);
                resultRect->setTo(x - pivotX * scaleX,      y - pivotY * scaleY,
                                 sHelperPoint->x() * scaleX, sHelperPoint->y() * scaleY);
                if (scaleX < 0) { resultRect->width  *= -1; resultRect->x() -= resultRect->width();  }
                if (scaleY < 0) { resultRect->height *= -1; resultRect->y() -= resultRect->height(); }
            }
            else
            {
                getTransformationMatrix(targetSpace, sHelperMatrix);
                mVertexData->getBounds(sHelperMatrix, 0, 4, resultRect);
            }

            return resultRect;
        }

        /** Returns the color of a vertex at a certain index. */// optimization
        unsigned int Quad::getVertexColor(int vertexID)
        {
            return mVertexData->getColor(vertexID);
        }

        /** Sets the color of a vertex at a certain index. */
        void Quad::setVertexColor(int vertexID, unsigned int color)
        {
            mVertexData->setColor(vertexID, color);
            onVertexDataChanged();

            if (color != 0xffffff) mTinted = true;
            else mTinted = mVertexData->tinted();
        }

        /** Returns the alpha value of a vertex at a certain index. */
        float Quad::getVertexAlpha(int vertexID)
        {
            return mVertexData->getAlpha(vertexID);
        }

        /** Sets the alpha value of a vertex at a certain index. */
        void Quad::setVertexAlpha(int vertexID, float alpha)
        {
            mVertexData->setAlpha(vertexID, alpha);
            onVertexDataChanged();

            if (alpha != 1.0) mTinted = true;
            else mTinted = mVertexData->tinted();
        }

        /** Returns the color of the quad, or of vertex 0 if vertices have different colors. */
        unsigned int Quad::color()
        {
            return mVertexData->getColor(0);
        }

        /** Sets the colors of all vertices to a certain value. */
        void Quad::color(unsigned int value)
        {
            for ( int i=0; i<4; ++i)
                setVertexColor(i, value);

            if (value != 0xffffff || alpha != 1.0) mTinted = true;
            else mTinted = mVertexData->tinted();
        }

        /** @inheritDoc **/
        void Quad::alpha(float value)
        {
            super()->alpha ( value);

            if (value < 1.0) mTinted = true;
            else mTinted = mVertexData->tinted();
        }

        /** Copies the raw vertex data to a VertexData instance. */
        void Quad::copyVertexDataTo(VertexData* targetData, int targetVertexID)
        {
            mVertexData->copyTo(targetData, targetVertexID);
        }

        /** @inheritDoc */
        void Quad::render(RenderSupport* support, float parentAlpha)
        {
            support->batchQuad(this, parentAlpha);
        }

        /** Returns true if the quad (or any of its vertices) is non-white or non-opaque. */
        bool Quad::tinted()         { return mTinted; }

        /** Indicates if the rgb values are stored premultiplied with the alpha value; this can
         *  affect the rendering. (Most developers don't have to care, though.) */
        bool Quad::premultipliedAlpha()         { return mVertexData->premultipliedAlpha(); }
}
}

