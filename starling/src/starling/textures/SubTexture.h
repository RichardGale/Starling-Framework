#if !defined(__STARLING_SRC_STARLING_TEXTURES_SUBTEXTURE_AS)
#define __STARLING_SRC_STARLING_TEXTURES_SUBTEXTURE_AS
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
namespace flash
{
    namespace display3D
    {
        namespace textures
        {
            class TextureBase;
        }
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
    namespace utils
    {
        class VertexData;
    }
}

/** A SubTexture represents a section of another texture. This is achieved solely by
 *  manipulation of texture coordinates, making the class very efficient.
 *
 *  <p><em>Note that it is OK to create subtextures of subtextures.</em></p>
 */
using namespace flash::display3D::textures;
using namespace flash::geom;
using namespace flash::geom;
using namespace starling::utils;

namespace starling
{
    namespace textures
    {
        class SubTexture: public Texture
        {
        private:
            Texture *mParent;
        private:
            Rectangle *mClipping;
        private:
            Rectangle *mRootClipping;
        private:
            bool mOwnsParent;

            /** Helper object. */
        private:
            static  Point *sTexCoords;

            /** Creates a new subtexture containing the specified region (in points) of a parent
             *  texture. If 'ownsParent' is true, the parent texture will be disposed automatically
             *  when the subtexture is disposed. */
        public:
            SubTexture(Texture *parentTexture, Rectangle *region,
                       bool ownsParent   =false);

            /** Disposes the parent texture if this texture owns it. */
        public:
            virtual void     dispose();

        private:
            void     setClipping(Rectangle *value);

            /** @inheritDoc */
        public:
            virtual void     adjustVertexData(VertexData *vertexData, int vertexID, int count);

            /** The texture which the subtexture is based on. */
        public:
            Texture     *parent();

            /** Indicates if the parent texture is disposed when this object is disposed. */
        public:
            bool         ownsParent();

            /** The clipping rectangle, which is the region provided on initialization
             *  scaled into [0.0, 1.0]. */
        public:
            Rectangle   *clipping();

            /** @inheritDoc */
        public:
            virtual TextureBase *base();

            /** @inheritDoc */
        public:
            virtual ConcreteTexture *root();

            /** @inheritDoc */
        public:
            virtual std::string  format();

            /** @inheritDoc */
        public:
            virtual float        width();

            /** @inheritDoc */
        public:
            virtual float        height();

            /** @inheritDoc */
        public:
            virtual float        nativeWidth();

            /** @inheritDoc */
        public:
            virtual float        nativeHeight();

            /** @inheritDoc */
        public:
            virtual bool         mipMapping();

            /** @inheritDoc */
        public:
            virtual bool         premultipliedAlpha();

            /** @inheritDoc */
        public:
            virtual float        scale();

        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXTURES_SUBTEXTURE_AS
#endif // __cplusplus

