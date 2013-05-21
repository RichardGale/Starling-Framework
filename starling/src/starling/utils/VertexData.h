#if !defined(__STARLING_SRC_STARLING_UTILS_VERTEXDATA_AS)
#define __STARLING_SRC_STARLING_UTILS_VERTEXDATA_AS
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

namespace starling
{
    namespace utils
    {
        class VertexData
        {
            /** The total number of elements (Numbers) stored per vertex. */
        public:
            static const int ELEMENTS_PER_VERTEX;

            /** The offset of position data (x, y) within a vertex. */
        public:
            static const int POSITION_OFFSET;

            /** The offset of color data (r, g, b, a) within a vertex. */
        public:
            static const int COLOR_OFFSET;

            /** The offset of texture coordinates (u, v) within a vertex. */
        public:
            static const int TEXCOORD_OFFSET;

        private:
            std::vector<float> mRawData;
        private:
            bool mPremultipliedAlpha;
        private:
            int mNumVertices;

            /** Helper object. */
        private:
            static Point *sHelperPoint;

            /** Create a new VertexData object with a specified number of vertices. */
        public:
            VertexData(int numVertices, bool premultipliedAlpha   =false);

            /** Creates a duplicate of either the complete vertex data object, or of a subset.
             *  To clone all vertices, set 'numVertices' to '-1'. */
        public:
            VertexData *clone(int vertexID=0, int numVertices=-1);

            /** Copies the vertex data (or a range of it, defined by 'vertexID' and 'numVertices')
             *  of this instance to another vertex data object, starting at a certain index. */
        public:
            void     copyTo(VertexData *targetData, int targetVertexID=0,
                            int vertexID=0, int numVertices=-1);

            /** Appends the vertices from another VertexData object. */
        public:
            void     append(VertexData *data);

            // functions

            /** Updates the position values of a vertex. */
        public:
            void     setPosition(int vertexID, float x, float y);

            /** Returns the position of a vertex. */
        public:
            void     getPosition(int vertexID, Point *position);

            /** Updates the RGB color values of a vertex. */
        public:
            void     setColor(int vertexID, unsigned int color);

            /** Returns the RGB color of a vertex (no alpha). */
        public:
            unsigned int getColor(int vertexID);

            /** Updates the alpha value of a vertex (range 0-1). */
        public:
            void     setAlpha(int vertexID, float alpha);

            /** Returns the alpha value of a vertex in the range 0-1. */
        public:
            float    getAlpha(int vertexID);

            /** Updates the texture coordinates of a vertex (range 0-1). */
        public:
            void     setTexCoords(int vertexID, float u, float v);

            /** Returns the texture coordinates of a vertex in the range 0-1. */
        public:
            void     getTexCoords(int vertexID, Point *texCoords);

            // utility functions

            /** Translate the position of a vertex by a certain offset. */
        public:
            void     translateVertex(int vertexID, float deltaX, float deltaY);

            /** Transforms the position of subsequent vertices by multiplication with a
             *  transformation matrix. */
        public:
            void     transformVertex(int vertexID, Matrix *matrix, int numVertices=1);

            /** Sets all vertices of the object to the same color values. */
        public:
            void     setUniformColor(unsigned int color);

            /** Sets all vertices of the object to the same alpha values. */
        public:
            void     setUniformAlpha(float alpha);

            /** Multiplies the alpha value of subsequent vertices with a certain delta. */
        public:
            void     scaleAlpha(int vertexID, float alpha, int numVertices=1);

        private:
            int      getOffset(int vertexID);

            /** Calculates the bounds of the vertices, which are optionally transformed by a matrix.
             *  If you pass a 'resultRect', the result will be stored in this rectangle
             *  instead of creating a new object. To use all vertices for the calculation, set
             *  'numVertices' to '-1'. */
        public:
            Rectangle *getBounds(Matrix *transformationMatrix=NULL,
                                 int vertexID=0, int numVertices=-1,
                                 Rectangle *resultRect=NULL);

            // properties

            /** Indicates if any vertices have a non-white color or are not fully opaque. */
        public:
            bool         tinted();

            /** Changes the way alpha and color values are stored. Updates all exisiting vertices. */
        public:
            void     setPremultipliedAlpha(bool value, bool updateData   =true);

            /** Indicates if the rgb values are stored premultiplied with the alpha value. */
        public:
            bool         premultipliedAlpha();

            /** The total number of vertices. */
        public:
            int          numVertices();
        public:
            void         numVertices(int value);

            /** The raw vertex data; not a copy! */
        public:
            std::vector<float> rawData();
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_VERTEXDATA_AS
#endif // __cplusplus

