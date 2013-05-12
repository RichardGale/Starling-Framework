#if !defined(__STARLING_SRC_STARLING_DISPLAY_QUAD_AS)
#define __STARLING_SRC_STARLING_DISPLAY_QUAD_AS
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
namespace flash { namespace geom { class Matrix; } }
namespace flash { namespace geom { class Point; } }
namespace flash { namespace geom { class Rectangle; } }

namespace starling { namespace core { class RenderSupport; } }
namespace starling { namespace utils { class VertexData; } }

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
using namespace flash::geom;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::utils;

namespace starling {
namespace display {
    class Quad: public DisplayObject
    {
        private:  bool mTinted;

        /** The raw vertex data of the quad. */
        protected:  VertexData* mVertexData;

        /** Helper objects. */
        private: static  Point* sHelperPoint;
        private: static  Matrix* sHelperMatrix;

        /** Creates a quad with a certain size and color. The last parameter controls if the 
         *  alpha value should be premultiplied into the color values on rendering, which can
         *  influence blending output. You can use the default value in most cases.  */
        public:          Quad(float width, float height, unsigned int color=0xffffff,
                             bool premultipliedAlpha   =true);

        /** Call this method after manually changing the contents of 'mVertexData'. */
        protected: void     onVertexDataChanged();

        /** @inheritDoc */
        public: virtual Rectangle* getBounds(DisplayObject* targetSpace, Rectangle* resultRect=NULL);

        /** Returns the color of a vertex at a certain index. */// optimization
        public: unsigned int getVertexColor(int vertexID);

        /** Sets the color of a vertex at a certain index. */
        public: void     setVertexColor(int vertexID, unsigned int color);

        /** Returns the alpha value of a vertex at a certain index. */
        public: float    getVertexAlpha(int vertexID);

        /** Sets the alpha value of a vertex at a certain index. */
        public: void     setVertexAlpha(int vertexID, float alpha);

        /** Returns the color of the quad, or of vertex 0 if vertices have different colors. */
        public: unsigned int color();

        /** Sets the colors of all vertices to a certain value. */
        public: void         color(unsigned int value);

        /** @inheritDoc **/
        public: virtual void         alpha(float value);

        /** Copies the raw vertex data to a VertexData instance. */
        public: void     copyVertexDataTo(VertexData* targetData, int targetVertexID=0);

        /** @inheritDoc */
        public: virtual void     render(RenderSupport* support, float parentAlpha);

        /** Returns true if the quad (or any of its vertices) is non-white or non-opaque. */
        public: bool         tinted();

        /** Indicates if the rgb values are stored premultiplied with the alpha value; this can
         *  affect the rendering. (Most developers don't have to care, though.) */
        public: bool         premultipliedAlpha();
    };
}
}

#endif // __STARLING_SRC_STARLING_DISPLAY_QUAD_AS
#endif // __cplusplus

