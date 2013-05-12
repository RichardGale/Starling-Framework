// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "VertexData.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

/** The VertexData class manages a raw list of vertex information, allowing direct upload
 *  to Stage3D vertex buffers. <em>You only have to work with this class if you create display
 *  objects with a custom render function. If you don't plan to do that, you can safely
 *  ignore it.</em>
 *
 *  <p>To render objects with Stage3D, you have to organize vertex data in so-called
 *  vertex buffers. Those buffers reside in graphics memory and can be accessed very
 *  efficiently by the GPU. Before you can move data into vertex buffers, you have to
 *  set it up in conventional memory - that is, in a Vector object. The vector contains
 *  all vertex information (the coordinates, color, and texture coordinates) - one
 *  vertex after the other.</p>
 *
 *  <p>To simplify creating and working with such a bulky list, the VertexData class was
 *  created. It contains methods to specify and modify vertex data. The raw Vector managed
 *  by the class can then easily be uploaded to a vertex buffer.</p>
 *
 *  <strong>Premultiplied Alpha</strong>
 *
 *  <p>The color values of the "BitmapData" object contain premultiplied alpha values, which
 *  means that the <code>rgb</code> values were multiplied with the <code>alpha</code> value
 *  before saving them. Since textures are created from bitmap data, they contain the values in
 *  the same style. On rendering, it makes a difference in which way the alpha value is saved;
 *  for that reason, the VertexData class mimics this behavior. You can choose how the alpha
 *  values should be handled via the <code>premultipliedAlpha</code> property.</p>
 *
 */
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;

namespace starling
{
    namespace utils
    {


        /** The total number of elements (Numbers) stored per vertex. */
        const int VertexData::ELEMENTS_PER_VERTEX=8;

        /** The offset of position data (x, y) within a vertex. */
        const int VertexData::POSITION_OFFSET=0;

        /** The offset of color data (r, g, b, a) within a vertex. */
        const int VertexData::COLOR_OFFSET=2;

        /** The offset of texture coordinates (u, v) within a vertex. */
        const int VertexData::TEXCOORD_OFFSET=6;

        /** Helper object. */
        Point *VertexData::sHelperPoint=newPoint();

        /** Create a new VertexData object with a specified number of vertices. */
        VertexData::VertexData(int numVertices, bool premultipliedAlpha)
        {
            mRawData.clear();
            mPremultipliedAlpha = premultipliedAlpha;
            this->numVertices= numVertices;
        }

        /** Creates a duplicate of either the complete vertex data object, or of a subset.
         *  To clone all vertices, set 'numVertices' to '-1'. */
        VertexData *VertexData::clone(int vertexID, int numVertices)
        {
            if (numVertices < 0 || vertexID + numVertices > mNumVertices)
                numVertices = mNumVertices - vertexID;

            VertexData *clone=new VertexData(0, mPremultipliedAlpha);
            clone->mNumVertices= numVertices;
            clone->mRawData= mRawData.slice(vertexID * ELEMENTS_PER_VERTEX,
                                            numVertices * ELEMENTS_PER_VERTEX);
            clone->mRawData.fixed= true;
            return clone;
        }

        /** Copies the vertex data (or a range of it, defined by 'vertexID' and 'numVertices')
         *  of this instance to another vertex data object, starting at a certain index. */
        void VertexData::copyTo(VertexData *targetData, int targetVertexID,
                                int vertexID, int numVertices)
        {
            if (numVertices < 0 || vertexID + numVertices > mNumVertices)
                numVertices = mNumVertices - vertexID;

            // todo: check/convert pma

            std::vector<float> *targetRawData=targetData->mRawData;
            int targetIndex= targetVertexID * ELEMENTS_PER_VERTEX;
            int sourceIndex= vertexID * ELEMENTS_PER_VERTEX;
            int dataLength= numVertices * ELEMENTS_PER_VERTEX;

            for ( int i=sourceIndex; i<dataLength; ++i)
                targetRawData[int(targetIndex++)] = mRawData[i];
        }

        /** Appends the vertices from another VertexData object. */
        void VertexData::append(VertexData *data)
        {
            mRawData.fixed = false;

            int targetIndex= mRawData.length;
            std::vector<float> *rawData=data->mRawData;
            int rawDataLength= rawData.length;

            for ( int i=0; i<rawDataLength; ++i)
                mRawData[int(targetIndex++)] = rawData[i];

            mNumVertices += data->numVertices;
            mRawData.fixed = true;
        }

        // functions

        /** Updates the position values of a vertex. */
        void VertexData::setPosition(int vertexID, float x, float y)
        {
            int offset= getOffset(vertexID) + POSITION_OFFSET;
            mRawData[offset] = x;
            mRawData[int(offset+1)] = y;
        }

        /** Returns the position of a vertex. */
        void VertexData::getPosition(int vertexID, Point *position)
        {
            int offset= getOffset(vertexID) + POSITION_OFFSET;
            position->x= mRawData[offset];
            position->y= mRawData[int(offset+1)];
        }

        /** Updates the RGB color values of a vertex. */
        void VertexData::setColor(int vertexID, unsigned int color)
        {
            int offset= getOffset(vertexID) + COLOR_OFFSET;
            float multiplier = mPremultipliedAlpha ? mRawData[int(offset+3)] : 1.0;
            mRawData[offset]        = ((color >> 16) & 0xff) / 255.0 * multiplier;
            mRawData[int(offset+1)] = ((color >>  8) & 0xff) / 255.0 * multiplier;
            mRawData[int(offset+2)] = ( color        & 0xff) / 255.0 * multiplier;
        }

        /** Returns the RGB color of a vertex (no alpha). */
        unsigned int VertexData::getColor(int vertexID)
        {
            int offset= getOffset(vertexID) + COLOR_OFFSET;
            float divisor = mPremultipliedAlpha ? mRawData[int(offset+3)] : 1.0;

            if (divisor == 0) return 0;
            else
            {
                float red   = mRawData[offset]        / divisor;
                float green = mRawData[int(offset+1)] / divisor;
                float blue  = mRawData[int(offset+2)] / divisor;

                return (int(red*255) << 16) | (int(green*255) << 8) | int(blue*255);
            }
        }

        /** Updates the alpha value of a vertex (range 0-1). */
        void VertexData::setAlpha(int vertexID, float alpha)
        {
            int offset= getOffset(vertexID) + COLOR_OFFSET + 3;

            if (mPremultipliedAlpha)
            {
                if (alpha < 0.001) alpha = 0.001; // zero alpha would wipe out all color data
                unsigned int color=getColor(vertexID);
                mRawData[offset] = alpha;
                setColor(vertexID, color);
            }
            else
            {
                mRawData[offset] = alpha;
            }
        }

        /** Returns the alpha value of a vertex in the range 0-1. */
        float VertexData::getAlpha(int vertexID)
        {
            int offset= getOffset(vertexID) + COLOR_OFFSET + 3;
            return mRawData[offset];
        }

        /** Updates the texture coordinates of a vertex (range 0-1). */
        void VertexData::setTexCoords(int vertexID, float u, float v)
        {
            int offset= getOffset(vertexID) + TEXCOORD_OFFSET;
            mRawData[offset]        = u;
            mRawData[int(offset+1)] = v;
        }

        /** Returns the texture coordinates of a vertex in the range 0-1. */
        void VertexData::getTexCoords(int vertexID, Point *texCoords)
        {
            int offset= getOffset(vertexID) + TEXCOORD_OFFSET;
            texCoords->x= mRawData[offset];
            texCoords->y= mRawData[int(offset+1)];
        }

        // utility functions

        /** Translate the position of a vertex by a certain offset. */
        void VertexData::translateVertex(int vertexID, float deltaX, float deltaY)
        {
            int offset= getOffset(vertexID) + POSITION_OFFSET;
            mRawData[offset]        += deltaX;
            mRawData[int(offset+1)] += deltaY;
        }

        /** Transforms the position of subsequent vertices by multiplication with a
         *  transformation matrix. */
        void VertexData::transformVertex(int vertexID, Matrix *matrix, int numVertices)
        {
            int offset= getOffset(vertexID) + POSITION_OFFSET;

            for ( int i=0; i<numVertices; ++i)
            {
                float x = mRawData[offset];
                float y = mRawData[int(offset+1)];

                mRawData[offset]        = matrix->a* x + matrix->c* y + matrix->tx;
                mRawData[int(offset+1)] = matrix->d* y + matrix->b* x + matrix->ty;

                offset += ELEMENTS_PER_VERTEX;
            }
        }

        /** Sets all vertices of the object to the same color values. */
        void VertexData::setUniformColor(unsigned int color)
        {
            for ( int i=0; i<mNumVertices; ++i)
                setColor(i, color);
        }

        /** Sets all vertices of the object to the same alpha values. */
        void VertexData::setUniformAlpha(float alpha)
        {
            for ( int i=0; i<mNumVertices; ++i)
                setAlpha(i, alpha);
        }

        /** Multiplies the alpha value of subsequent vertices with a certain delta. */
        void VertexData::scaleAlpha(int vertexID, float alpha, int numVertices)
        {
            if (alpha == 1.0) return;
            if (numVertices < 0 || vertexID + numVertices > mNumVertices)
                numVertices = mNumVertices - vertexID;

            int i;

            if (mPremultipliedAlpha)
            {
                for (i=0; i<numVertices; ++i)
                    setAlpha(vertexID+i, getAlpha(vertexID+i) * alpha);
            }
            else
            {
                int offset= getOffset(vertexID) + COLOR_OFFSET + 3;
                for (i=0; i<numVertices; ++i)
                    mRawData[int(offset + i*ELEMENTS_PER_VERTEX)] *= alpha;
            }
        }

        int VertexData::getOffset(int vertexID)
        {
            return vertexID * ELEMENTS_PER_VERTEX;
        }

        /** Calculates the bounds of the vertices, which are optionally transformed by a matrix.
         *  If you pass a 'resultRect', the result will be stored in this rectangle
         *  instead of creating a new object. To use all vertices for the calculation, set
         *  'numVertices' to '-1'. */
        Rectangle *VertexData::getBounds(Matrix *transformationMatrix,
                                         int vertexID, int numVertices,
                                         Rectangle *resultRect)
        {
            if (resultRect == NULL) resultRect = new Rectangle();
            if (numVertices < 0 || vertexID + numVertices > mNumVertices)
                numVertices = mNumVertices - vertexID;

            float minX = Number::MAX_VALUE, float maxX  = -Number::MAX_VALUE;
            float minY = Number::MAX_VALUE, float maxY  = -Number::MAX_VALUE;
            int offset= getOffset(vertexID) + POSITION_OFFSET;
            float x, float y, int i;

            if (transformationMatrix == NULL)
            {
                for (i=0; i<numVertices; ++i)
                {
                    x = mRawData[offset];
                    y = mRawData[int(offset+1)];
                    offset += ELEMENTS_PER_VERTEX;

                    minX = minX < x ? minX : x;
                    maxX = maxX > x ? maxX : x;
                    minY = minY < y ? minY : y;
                    maxY = maxY > y ? maxY : y;
                }
            }
            else
            {
                for (i=0; i<numVertices; ++i)
                {
                    x = mRawData[offset];
                    y = mRawData[int(offset+1)];
                    offset += ELEMENTS_PER_VERTEX;

                    MatrixUtil->transformCoords(transformationMatrix,x, y, sHelperPoint);
                    minX = minX < sHelperPoint->x? minX : sHelperPoint->x;
                    maxX = maxX > sHelperPoint->x? maxX : sHelperPoint->x;
                    minY = minY < sHelperPoint->y? minY : sHelperPoint->y;
                    maxY = maxY > sHelperPoint->y? maxY : sHelperPoint->y;
                }
            }

            resultRect->setTo(minX,minY, maxX - minX, maxY - minY);
            return resultRect;
        }

        // properties

        /** Indicates if any vertices have a non-white color or are not fully opaque. */
        bool VertexData::tinted()
        {
            int offset= COLOR_OFFSET;

            for ( int i=0; i<mNumVertices; ++i)
            {
                for ( int j=0; j<4; ++j)
                    if (mRawData[int(offset+j)] != 1.0) return true;

                offset += ELEMENTS_PER_VERTEX;
            }

            return false;
        }

        /** Changes the way alpha and color values are stored. Updates all exisiting vertices. */
        void VertexData::setPremultipliedAlpha(bool value, bool updateData)
        {
            if (value == mPremultipliedAlpha) return;

            if (updateData)
            {
                int dataLength= mNumVertices * ELEMENTS_PER_VERTEX;

                for ( int i=COLOR_OFFSET; i<dataLength; i += ELEMENTS_PER_VERTEX)
                {
                    float alpha = mRawData[int(i+3)];
                    float divisor = mPremultipliedAlpha ? alpha : 1.0;
                    float multiplier = value ? alpha : 1.0;

                    if (divisor != 0)
                    {
                        mRawData[i]        = mRawData[i]        / divisor * multiplier;
                        mRawData[int(i+1)] = mRawData[int(i+1)] / divisor * multiplier;
                        mRawData[int(i+2)] = mRawData[int(i+2)] / divisor * multiplier;
                    }
                }
            }

            mPremultipliedAlpha = value;
        }

        /** Indicates if the rgb values are stored premultiplied with the alpha value. */
        bool VertexData::premultipliedAlpha()
        {
            return mPremultipliedAlpha;
        }

        /** The total number of vertices. */
        int VertexData::numVertices()
        {
            return mNumVertices;
        }
        void VertexData::numVertices(int value)
        {
            mRawData.fixed = false;

            int i;
            int delta= value - mNumVertices;

            for (i=0; i<delta; ++i)
                mRawData.push_back(0,0,0,0,0, 1,  0, 0); // alpha should be '1' per default

            for (i=0; i<-(delta*ELEMENTS_PER_VERTEX); ++i)
                mRawData.pop();

            mNumVertices = value;
            mRawData.fixed = true;
        }

        /** The raw vertex data; not a copy! */
        std::vector<float> *VertexData::rawData()
        {
            return mRawData;
        }
    }
}

