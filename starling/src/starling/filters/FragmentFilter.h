#if !defined(__STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTER_AS)
#define __STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTER_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "flex11.6.h"
namespace flash
{
    namespace display3D
    {
        class Context3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3DProgramType;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3DVertexBufferFormat;
    }
}
namespace flash
{
    namespace display3D
    {
        class IndexBuffer3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class Program3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class VertexBuffer3D;
    }
}
namespace flash
{
    namespace errors
    {
        class IllegalOperationError;
    }
}
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
        class Rectangle;
    }
}
namespace flash
{
    namespace system
    {
        class Capabilities;
    }
}
namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
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
    namespace core
    {
        class Starling;
    }
}
namespace starling
{
    namespace core
    {
        class starling_internal;
    }
}
namespace starling
{
    namespace display
    {
        class BlendMode;
    }
}
namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
namespace starling
{
    namespace display
    {
        class Image;
    }
}
namespace starling
{
    namespace display
    {
        class QuadBatch;
    }
}
namespace starling
{
    namespace display
    {
        class Stage;
    }
}
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}
namespace starling
{
    namespace errors
    {
        class MissingContextError;
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
    namespace textures
    {
        class Texture;
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
namespace starling
{
    namespace utils
    {
        class VertexData;
    }
}
namespace starling
{
    namespace utils
    {
        class getNextPowerOfTwo;
    }
}

/** The FragmentFilter class is the base class for all filter effects in Starling.
 *  All other filters of this package extend this class. You can attach them to any display
 *  object through the 'filter' property.
 *
 *  <p>A fragment filter works in the following way:</p>
 *  <ol>
 *    <li>The object that is filtered is rendered into a texture (in stage coordinates).</li>
 *    <li>That texture is passed to the first filter pass.</li>
 *    <li>Each pass processes the texture using a fragment shader (and optionally a vertex
 *        shader) to achieve a certain effect.</li>
 *    <li>The output of each pass is used as the input for the next pass; if it's the
 *        final pass, it will be rendered directly to the back buffer.</li>
 *  </ol>
 *
 *  <p>All of this is set up by the abstract FragmentFilter class. Concrete subclasses
 *  just need to override the protected methods 'createPrograms', 'activate' and
 *  (optionally) 'deactivate' to create and execute its custom shader code. Each filter
 *  can be configured to either replace the original object, or be drawn below or above it.
 *  This can be done through the 'mode' property, which accepts one of the Strings defined
 *  in the 'FragmentFilterMode' class.</p>
 *
 *  <p>Beware that each filter should be used only on one object at a time. Otherwise, it
 *  will get slower and require more resources; and caching will lead to undefined
 *  results.</p>
 */

using namespace flash::display3D;
using namespace flash::errors;
using namespace flash::geom;
using namespace flash::system;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace filters
    {
        class FragmentFilter
        {
            /** All filter processing is expected to be done with premultiplied alpha. */
        protected:
            const bool PMA;

            /** The standard vertex shader code. It will be used automatically if you don't create
             *  a custom vertex shader yourself. */
        protected:
            const std::string STD_VERTEX_SHADER;  // pass texture coordinates to fragment program

            /** The standard fragment shader code. It just forwards the texture color to the output. */
        protected:
            const std::string STD_FRAGMENT_SHADER; // just forward texture color

        private:
            int mVertexPosAtID;
        private:
            int mTexCoordsAtID;
        private:
            int mBaseTextureID;
        private:
            int mMvpConstantID;

        private:
            int mNumPasses;
        private:
            std::vector<Texture *> mPassTextures;

        private:
            std::string mMode;
        private:
            float mResolution;
        private:
            float mMarginX;
        private:
            float mMarginY;
        private:
            float mOffsetX;
        private:
            float mOffsetY;

        private:
            VertexData *mVertexData;
        private:
            VertexBuffer3D *mVertexBuffer;
        private:
            std::vector<unsigned int> mIndexData;
        private:
            IndexBuffer3D *mIndexBuffer;

        private:
            bool mCacheRequested;
        private:
            QuadBatch *mCache;

            /** helper objects. */
        private:
            Matrix *mProjMatrix;
        private:
            static Rectangle *sBounds;
        private:
            static Rectangle *sStageBounds;
        private:
            static Matrix *sTransformationMatrix;

            /** Creates a new Fragment filter with the specified number of passes and resolution.
             *  This constructor may only be called by the constructor of a subclass. */
        public:
            FragmentFilter(int numPasses=1, float resolution=1.0);

            /** Disposes the filter (programs, buffers, textures). */
        public:
            void     dispose();

        private:
            void     onContextCreated(Object *event);

            /** Applies the filter on a certain display object, rendering the output into the current
             *  render target. This method is called automatically by Starling's rendering system
             *  for the object the filter is attached to. */
        public:
            void     render(DisplayObject *object, RenderSupport *support, float parentAlpha);

        private:
            QuadBatch *renderPasses(DisplayObject *object, RenderSupport *support,
                                    float parentAlpha, bool intoCache=false);

            // helper methods// final pass

        private:
            void     updateBuffers(Context3D *context, Rectangle *bounds);

        private:
            void     updatePassTextures(int width, int height, float scale);

        private:
            Texture  *getPassTexture(int pass);

            /** Calculates the bounds of the filter in stage coordinates, while making sure that the
             *  according textures will have powers of two. */
        private:
            void     calculateBounds(DisplayObject *object, Stage *stage,
                                     bool intersectWithStage, Rectangle *resultRect);

        private:
            void     disposePassTextures();

        private:
            void     disposeCache();

            // protected methods

            /** Subclasses must override this method and use it to create their
             *  fragment- and vertex-programs. */
        protected:
            void     createPrograms();

            /** Subclasses must override this method and use it to activate their fragment- and
             *  to vertext-programs.
             *  The 'activate' call directly precedes the call to 'context.drawTriangles'. Set up
             *  the context the way your filter needs it. The following constants and attributes
             *  are set automatically:
             *
             *  <ul><li>vertex constants 0-3: mvpMatrix (3D)</li>
             *      <li>vertex attribute 0: vertex position (FLOAT_2)</li>
             *      <li>vertex attribute 1: texture coordinates (FLOAT_2)</li>
             *      <li>texture 0: input texture</li>
             *  </ul>
             *
             *  @param pass: the current render pass, starting with '0'. Multipass filters can
             *               provide different logic for each pass.
             *  @param context: the current context3D (the same as in Starling.context, passed
             *               just for convenience)
             *  @param texture: the input texture, which is already bound to sampler 0. */
        protected:
            void     activate(int pass, Context3D *context, Texture *texture);

            /** This method is called directly after 'context.drawTriangles'.
             *  If you need to clean up any resources, you can do so in this method. */
        protected:
            void     deactivate(int pass, Context3D *context, Texture *texture);

            /** Assembles fragment- and vertex-shaders, passed as Strings, to a Program3D.
             *  If any argument is  null, it is replaced by the class constants STD_FRAGMENT_SHADER or
             *  STD_VERTEX_SHADER, respectively. */
        protected:
            Program3D *assembleAgal(std::string fragmentShader="", std::string vertexShader="");

            // cache

            /** Caches the filter output into a texture. An uncached filter is rendered in every frame;
             *  a cached filter only once. However, if the filtered object or the filter settings
             *  change, it has to be updated manually; to do that, call "cache" again. */
        public:
            void     cache();

            /** Clears the cached output of the filter. After calling this method, the filter will
             *  be executed once per frame again. */
        public:
            void     clearCache();

            // flattening

            /** @private */
            QuadBatch *compile(DisplayObject *object);

            // properties

            /** Indicates if the filter is cached (via the "cache" method). */
        public:
            bool      isCached();

            /** The resolution of the filter texture. "1" means stage resolution, "0.5" half the
             *  stage resolution. A lower resolution saves memory and execution time (depending on
             *  the GPU), but results in a lower output quality. Values greater than 1 are allowed;
             *  such values might make sense for a cached filter when it is scaled up. @default 1 */
        public:
            float       resolution();
        public:
            void         resolution(float value);

            /** The filter mode, which is one of the constants defined in the "FragmentFilterMode"
             *  class. @default "replace" */
        public:
            std::string       mode();
        public:
            void         mode(std::string value);

            /** Use the x-offset to move the filter output to the right or left. */
        public:
            float       offsetX();
        public:
            void         offsetX(float value);

            /** Use the y-offset to move the filter output to the top or bottom. */
        public:
            float       offsetY();
        public:
            void         offsetY(float value);

            /** The x-margin will extend the size of the filter texture along the x-axis.
             *  Useful when the filter will "grow" the rendered object. */
        protected:
            float       marginX();
        protected:
            void         marginX(float value);

            /** The y-margin will extend the size of the filter texture along the y-axis.
             *  Useful when the filter will "grow" the rendered object. */
        protected:
            float       marginY();
        protected:
            void         marginY(float value);

            /** The number of passes the filter is applied. The "activate" and "deactivate" methods
             *  will be called that often. */
        protected:
            void         numPasses(int value);
        protected:
            int          numPasses();

            /** The ID of the vertex buffer attribute that stores the vertex position. */
        protected:
            int          vertexPosAtID();
        protected:
            void         vertexPosAtID(int value);

            /** The ID of the vertex buffer attribute that stores the texture coordinates. */
        protected:
            int          texCoordsAtID();
        protected:
            void         texCoordsAtID(int value);

            /** The ID (sampler) of the input texture (containing the output of the previous pass). */
        protected:
            int          baseTextureID();
        protected:
            void         baseTextureID(int value);

            /** The ID of the first register of the modelview-projection constant (a 4x4 matrix). */
        protected:
            int          mvpConstantID();
        protected:
            void         mvpConstantID(int value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTER_AS
#endif // __cplusplus

