#if !defined(__STARLING_SRC_STARLING_TEXTURES_RENDERTEXTURE_AS)
#define __STARLING_SRC_STARLING_TEXTURES_RENDERTEXTURE_AS
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
    namespace errors
    {
        class MissingContextError;
    }
}
namespace starling
{
    namespace utils
    {
        class getNextPowerOfTwo;
    }
}

/** A RenderTexture is a dynamic texture onto which you can draw any display object.
 *
 *  <p>After creating a render texture, just call the <code>drawObject</code> method to render
 *  an object directly onto the texture. The object will be drawn onto the texture at its current
 *  position, adhering its current rotation, scale and alpha properties.</p>
 *
 *  <p>Drawing is done very efficiently, as it is happening directly in graphics memory. After
 *  you have drawn objects onto the texture, the performance will be just like that of a normal
 *  texture - no matter how many objects you have drawn.</p>
 *
 *  <p>If you draw lots of objects at once, it is recommended to bundle the drawing calls in
 *  a block via the <code>drawBundled</code> method, like shown below. That will speed it up
 *  immensely, allowing you to draw hundreds of objects very quickly.</p>
 *
 *  <pre>
 *  renderTexture.drawBundled(function():void
 *  {
 *     for (var i:int=0; i&lt;numDrawings; ++i)
 *     {
 *         image.rotation = (2 &#42; Math.PI / numDrawings) &#42; i;
 *         renderTexture.draw(image);
 *     }
 *  });
 *  </pre>
 *
 *  <p>To erase parts of a render texture, you can use any display object like a "rubber" by
 *  setting its blending mode to "BlendMode.ERASE".</p>
 *
 *  <p>Beware that render textures can't be restored when the Starling's render context is lost.
 *  </p>
 *
 */
using namespace flash::display3D;
using namespace flash::display3D::textures;
using namespace flash::geom;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::utils;

namespace starling
{
    namespace textures
    {
        class RenderTexture: public SubTexture
        {
        private:
            const bool PMA;

        private:
            Texture *mActiveTexture;
        private:
            Texture *mBufferTexture;
        private:
            Image *mHelperImage;
        private:
            bool mDrawing;
        private:
            bool mBufferReady;
        private:
            RenderSupport *mSupport;

            /** helper object */
        private:
            static Rectangle *sClipRect;

            /** Creates a new RenderTexture with a certain size. If the texture is persistent, the
             *  contents of the texture remains intact after each draw call, allowing you to use the
             *  texture just like a canvas. If it is not, it will be cleared before each draw call.
             *  Persistancy doubles the required graphics memory! Thus, if you need the texture only
             *  for one draw (or drawBundled) call, you should deactivate it. */
        public:
            RenderTexture(int width, int height, bool persistent, float scale);

            /** @inheritDoc */
        public:
            virtual void     dispose();

            /** Draws an object into the texture. Note that any filters on the object will currently
             *  be ignored.
             *
             *  @param object       The object to draw.
             *  @param matrix       If 'matrix' is null, the object will be drawn adhering its
             *                      properties for position, scale, and rotation. If it is not null,
             *                      the object will be drawn in the orientation depicted by the matrix.
             *  @param alpha        The object's alpha value will be multiplied with this value.
             *  @param antiAliasing This parameter is currently ignored by Stage3D.
             */
        public:
            void     draw(DisplayObject *object, Matrix *matrix, float alpha,
                          int antiAliasing);

            /** Bundles several calls to <code>draw</code> together in a block. This avoids buffer
             *  switches and allows you to draw multiple objects into a non-persistent texture. */
        public:
            void     drawBundled(Function *drawingBlock, int antiAliasing);

            /** Clears the texture (restoring full transparency). */
        public:
            void     clear();

            /** Indicates if the texture is persistent over multiple draw calls. */
        public:
            bool         isPersistent();

            /** @inheritDoc */
        public:
            virtual TextureBase *base();

            /** @inheritDoc */
        public:
            virtual ConcreteTexture *root();
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXTURES_RENDERTEXTURE_AS
#endif // __cplusplus

