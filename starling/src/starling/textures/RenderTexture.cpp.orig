// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "RenderTexture.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/textures/TextureBase.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Rectangle.h"

#include "starling/core/RenderSupport.h"
#include "starling/core/Starling.h"
#include "starling/display/DisplayObject.h"
#include "starling/display/Image.h"
#include "starling/errors/MissingContextError.h"
#include "starling/utils/getNextPowerOfTwo.h"

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

namespace starling {
namespace textures {


        /** helper object */
         Rectangle* RenderTexture::sClipRect=newRectangle();

        /** Creates a new RenderTexture with a certain size. If the texture is persistent, the
         *  contents of the texture remains intact after each draw call, allowing you to use the
         *  texture just like a canvas. If it is not, it will be cleared before each draw call.
         *  Persistancy doubles the required graphics memory! Thus, if you need the texture only 
         *  for one draw (or drawBundled) call, you should deactivate it. */
        RenderTexture::RenderTexture(int width, int height, bool persistent, float scale)
        {
            if (scale <= 0) scale = Starling::contentScaleFactor;

             int nativeWidth = getNextPowerOfTwo(width  * scale);
             int nativeHeight= getNextPowerOfTwo(height * scale);
            mActiveTexture = Texture->empty(width,height, PMA, true, scale);

            super(mActiveTexture, new Rectangle(0, 0, width, height), true);

            mSupport = new RenderSupport();
            mSupport->setOrthographicProjection(0,0, nativeWidth/scale, nativeHeight/scale);

            if (persistent)
            {
                mBufferTexture = Texture->empty(width,height, PMA, true, scale);
                mHelperImage = new Image(mBufferTexture);
                mHelperImage->smoothing= TextureSmoothing->NONE;// solves some antialias-issues
            }
        }

        /** @inheritDoc */
        void RenderTexture::dispose()
        {
            mSupport->dispose();
            mActiveTexture->dispose();

            if (isPersistent)
            {
                mBufferTexture->dispose();
                mHelperImage->dispose();
            }

            super->dispose();
        }

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
        void RenderTexture::draw(DisplayObject* object, Matrix* matrix, float alpha,
                             int antiAliasing)
        {
            if (object == NULL) return;

            if (mDrawing)
                render();
            else
                drawBundled(render, antiAliasing);

            void RenderTexture::()
            {
                mSupport->loadIdentity();
                mSupport->blendMode= object->blendMode;

                if (matrix) mSupport->prependMatrix(matrix);
                else        mSupport->transformMatrix(object);

                object->render(mSupport,alpha);
            }
        }

        /** Bundles several calls to <code>draw</code> together in a block. This avoids buffer 
         *  switches and allows you to draw multiple objects into a non-persistent texture. */
        void RenderTexture::drawBundled(Function* drawingBlock, int antiAliasing)
        {
             Context3D* context=Starling::context;
            if (context == NULL) throw new MissingContextError();

            // persistent drawing uses double buffering, as Molehill forces us to call 'clear'
            // on every render target once per update.

            // switch buffers
            if (isPersistent)
            {
                 Texture* tmpTexture=mActiveTexture;
                mActiveTexture = mBufferTexture;
                mBufferTexture = tmpTexture;
                mHelperImage->texture= mBufferTexture;
            }

            // limit drawing to relevant area
            sClipRect->setTo(0,0, mActiveTexture->width,mActiveTexture->height);

            mSupport->pushClipRect(sClipRect);
            mSupport->renderTarget= mActiveTexture;
            mSupport->clear();

            // draw buffer
            if (isPersistent && mBufferReady)
                mHelperImage->render(mSupport,1.0);
            else
                mBufferReady = true;

            try
            {
                mDrawing = true;

                // draw new objects
                if (drawingBlock != NULL)
                    drawingBlock();
            }
            finally
            {
                mDrawing = false;
                mSupport->finishQuadBatch();
                mSupport->nextFrame();
                mSupport->renderTarget= NULL;
                mSupport->popClipRect();
            }
        }

        /** Clears the texture (restoring full transparency). */
        void RenderTexture::clear()
        {
             Context3D* context=Starling::context;
            if (context == NULL) throw new MissingContextError();

            mSupport->renderTarget= mActiveTexture;
            mSupport->clear();
            mSupport->renderTarget= NULL;
        }

        /** Indicates if the texture is persistent over multiple draw calls. */
        bool RenderTexture::isPersistent()         { return mBufferTexture != NULL; }

        /** @inheritDoc */
        TextureBase* RenderTexture::base()             { return mActiveTexture->base;}

        /** @inheritDoc */
        ConcreteTexture* RenderTexture::root()                 { return mActiveTexture->root;}
}
}

