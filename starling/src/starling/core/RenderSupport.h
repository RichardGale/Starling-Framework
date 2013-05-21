#if !defined(__STARLING_SRC_STARLING_CORE_RENDERSUPPORT_AS)
#define __STARLING_SRC_STARLING_CORE_RENDERSUPPORT_AS
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
namespace com
{
    namespace adobe
    {
        namespace utils
        {
            class AGALMiniAssembler;
        }
    }
}

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
        class Program3D;
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
        class Matrix3D;
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
        class Quad;
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
    namespace errors
    {
        class MissingContextError;
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
        class Color;
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

/** A class that contains helper methods simplifying Stage3D rendering.
 *
 *  A RenderSupport instance is passed to any "render" method of display objects.
 *  It allows manipulation of the current transformation matrix (similar to the matrix
 *  manipulation methods of OpenGL 1.x) and other helper methods.
 */

using namespace com::adobe::utils;
using namespace flash::display3D;
using namespace flash::geom;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace core
    {
        class RenderSupport
        {
            // members

        private:
            Matrix *mProjectionMatrix;
        private:
            Matrix *mModelViewMatrix;
        private:
            Matrix *mMvpMatrix;
        private:
            Matrix3D *mMvpMatrix3D;
        private:
            std::vector<Matrix *> mMatrixStack;
        private:
            int mMatrixStackSize;

        private:
            int mDrawCount;
        private:
            std::string mBlendMode;

        private:
            starling::textures::Texture *mRenderTarget;
        private:
            int mBackBufferWidth;
        private:
            int mBackBufferHeight;

        private:
            std::vector<Rectangle *> mClipRectStack;
        private:
            int mClipRectStackSize;

        private:
            std::vector<QuadBatch *> mQuadBatches;
        private:
            int mCurrentQuadBatchID;

            /** helper objects */
        private:
            static Point *sPoint;
        private:
            static Rectangle *sRectangle;
        private:
            static AGALMiniAssembler *sAssembler;

            // construction

            /** Creates a new RenderSupport object with an empty matrix stack. */
        public:
            RenderSupport();

            /** Disposes all quad batches. */
        public:
            void     dispose();

            // matrix manipulation

            /** Sets up the projection matrix for ortographic 2D rendering. */
        public:
            void     setOrthographicProjection(float x, float y, float width, float height);

            /** Changes the modelview matrix to the identity matrix. */
        public:
            void     loadIdentity();

            /** Prepends a translation to the modelview matrix. */
        public:
            void     translateMatrix(float dx, float dy);

            /** Prepends a rotation (angle in radians) to the modelview matrix. */
        public:
            void     rotateMatrix(float angle);

            /** Prepends an incremental scale change to the modelview matrix. */
        public:
            void     scaleMatrix(float sx, float sy);

            /** Prepends a matrix to the modelview matrix by multiplying it with another matrix. */
        public:
            void     prependMatrix(Matrix *matrix);

            /** Prepends translation, scale and rotation of an object to the modelview matrix. */
        public:
            void     transformMatrix(starling::display::DisplayObject *object);

            /** Pushes the current modelview matrix to a stack from which it can be restored later. */
        public:
            void     pushMatrix();

            /** Restores the modelview matrix that was last pushed to the stack. */
        public:
            void     popMatrix();

            /** Empties the matrix stack, resets the modelview matrix to the identity matrix. */
        public:
            void     resetMatrix();

            /** Prepends translation, scale and rotation of an object to a custom matrix. */
        public:
            static void     transformMatrixForObject(Matrix *matrix, DisplayObject *object);

            /** Calculates the product of modelview and projection matrix.
             *  CAUTION: Use with care! Each call returns the same instance. */
        public:
            Matrix      *mvpMatrix();

            /** Calculates the product of modelview and projection matrix and saves it in a 3D matrix.
             *  CAUTION: Use with care! Each call returns the same instance. */
        public:
            Matrix3D    *mvpMatrix3D();

            /** Returns the current modelview matrix.
             *  CAUTION: Use with care! Each call returns the same instance. */
        public:
            Matrix      *modelViewMatrix();

            /** Returns the current projection matrix.
             *  CAUTION: Use with care! Each call returns the same instance. */
        public:
            Matrix      *projectionMatrix();
        public:
            void         projectionMatrix(Matrix *value);

            // blending

            /** Activates the current blend mode on the active rendering context. */
        public:
            void     applyBlendMode(bool premultipliedAlpha);

            /** The blend mode to be used on rendering. To apply the factor, you have to manually call
             *  'applyBlendMode' (because the actual blend factors depend on the PMA mode). */
        public:
            std::string  blendMode();
        public:
            void         blendMode(std::string value);

            // render targets

            /** The texture that is currently being rendered into, or 'null' to render into the
             *  back buffer. If you set a new target, it is immediately activated. */
        public:
            Texture     *renderTarget();
        public:
            void         renderTarget(Texture *target);

            /** Configures the back buffer on the current context3D. By using this method, Starling
             *  can store the size of the back buffer and utilize this information in other methods
             *  (e.g. the 'clipRect' property). Back buffer width and height can later be accessed
             *  using the properties with the same name. */
        public:
            void     configureBackBuffer(int width, int height, int antiAlias,
                                         bool enableDepthAndStencil,
                                         bool wantsBestResolution   =false);

            /** The width of the back buffer, as it was configured in the last call to
             *  'RenderSupport.configureBackBuffer()'. Beware: changing this value does not actually
             *  resize the back buffer; the setter should only be used to inform Starling about the
             *  size of a back buffer it can't control (shared context situations).
             */
        public:
            int          backBufferWidth();
        public:
            void         backBufferWidth(int value);

            /** The height of the back buffer, as it was configured in the last call to
             *  'RenderSupport.configureBackBuffer()'. Beware: changing this value does not actually
             *  resize the back buffer; the setter should only be used to inform Starling about the
             *  size of a back buffer it can't control (shared context situations).
             */
        public:
            int          backBufferHeight();
        public:
            void         backBufferHeight(int value);

            // clipping

            /** The clipping rectangle can be used to limit rendering in the current render target to
             *  a certain area. This method expects the rectangle in stage coordinates. Internally,
             *  it uses the 'scissorRectangle' of stage3D, which works with pixel coordinates.
             *  Any pushed rectangle is intersected with the previous rectangle; the method returns
             *  that intersection. */
        public:
            Rectangle *pushClipRect(Rectangle *rectangle);

            /** Restores the clipping rectangle that was last pushed to the stack. */
        public:
            void     popClipRect();

            /** Updates the context3D scissor rectangle using the current clipping rectangle. This
             *  method is called automatically when either the render target, the projection matrix,
             *  or the clipping rectangle changes. */
        public:
            void     applyClipRect();

            // optimized quad rendering

            /** Adds a quad to the current batch of unrendered quads. If there is a state change,
             *  all previous quads are rendered at once, and the batch is reset. */
        public:
            void     batchQuad(Quad *quad, float parentAlpha,
                               Texture *texture=NULL, std::string smoothing="");

            /** Renders the current quad batch and resets it. */
        public:
            void     finishQuadBatch();

            /** Resets matrix stack, blend mode, quad batch index, and draw count. */
        public:
            void     nextFrame();

            // other helper methods

            /** Deprecated. Call 'setBlendFactors' instead. */
        public:
            static void     setDefaultBlendFactors(bool premultipliedAlpha);

            /** Sets up the blending factors that correspond with a certain blend mode. */
        public:
            static void     setBlendFactors(bool premultipliedAlpha, std::string blendMode="normal");

            /** Clears the render context with a certain color and alpha value. */
        public:
            static void     clear(unsigned int rgb=0, float alpha =0.0);

            /** Clears the render context with a certain color and alpha value. */
        public:
            void     clear(unsigned int rgb=0, float alpha =0.0);

            /** Assembles fragment- and vertex-shaders, passed as Strings, to a Program3D. If you
             *  pass a 'resultProgram', it will be uploaded to that program; otherwise, a new program
             *  will be created on the current Stage3D context. */
        public:
            static Program3D *assembleAgal(std::string vertexShader, std::string fragmentShader,
                                           Program3D *resultProgram=NULL);

            // statistics

            /** Raises the draw count by a specific value. Call this method in custom render methods
             *  to keep the statistics display in sync. */
        public:
            void     raiseDrawCount(unsigned int value=1);

            /** Indicates the number of stage3D draw calls. */
        public:
            int          drawCount();

        };
    }
}

#endif // __STARLING_SRC_STARLING_CORE_RENDERSUPPORT_AS
#endif // __cplusplus

