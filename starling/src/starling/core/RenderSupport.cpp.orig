// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "RenderSupport.h"
#include "com/adobe/utils/AGALMiniAssembler.h"

#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DProgramType.h"
#include "flash/display3D/Program3D.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Matrix3D.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/display/BlendMode.h"
#include "starling/display/DisplayObject.h"
#include "starling/display/Quad.h"
#include "starling/display/QuadBatch.h"
#include "starling/errors/MissingContextError.h"
#include "starling/textures/Texture.h"
#include "starling/utils/Color.h"
#include "starling/utils/MatrixUtil.h"
#include "starling/utils/RectangleUtil.h"

    /** A class that contains helper methods simplifying Stage3D rendering.
     *
     *  A RenderSupport instance is passed to any "render" method of display objects. 
     *  It allows manipulation of the current transformation matrix (similar to the matrix 
     *  manipulation methods of OpenGL 1.x) and other helper methods.
     */
using namespace com::adobe::utils;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::textures;
using namespace starling::utils;
using namespace starling::utils;
using namespace starling::utils;

namespace starling {
namespace core {


        // members

        /** helper objects */
         Point* RenderSupport::sPoint=newPoint();
         Rectangle* RenderSupport::sRectangle=newRectangle();
         AGALMiniAssembler* RenderSupport::sAssembler=newAGALMiniAssembler();

        // construction

        /** Creates a new RenderSupport object with an empty matrix stack. */
        RenderSupport::RenderSupport()
        {
            mProjectionMatrix = new Matrix();
            mModelViewMatrix = new Matrix();
            mMvpMatrix = new Matrix();
            mMvpMatrix3D = new Matrix3D();
            mMatrixStack.clear();
            mMatrixStackSize = 0;
            mDrawCount = 0;
            mRenderTarget = NULL;
            mBlendMode = BlendMode::NORMAL;
            mClipRectStack.clear();

            mCurrentQuadBatchID = 0;
            mQuadBatches.clear();

            loadIdentity();
            setOrthographicProjection(0, 0, 400, 300);
        }

        /** Disposes all quad batches. */
        void RenderSupport::dispose()
        {
            for each (var QuadBatch* quadBatchin mQuadBatches)
                quadBatch->dispose();
        }

        // matrix manipulation

        /** Sets up the projection matrix for ortographic 2D rendering. */
        void RenderSupport::setOrthographicProjection(float x, float y, float width, float height)
        {
            mProjectionMatrix->setTo(2.0/width,0, 0, -2.0/height,
                -(2*x + width) / width, (2*y + height) / height);

            applyClipRect();
        }

        /** Changes the modelview matrix to the identity matrix. */
        void RenderSupport::loadIdentity()
        {
            mModelViewMatrix->identity();
        }

        /** Prepends a translation to the modelview matrix. */
        void RenderSupport::translateMatrix(float dx, float dy)
        {
            MatrixUtil::prependTranslation(mModelViewMatrix,dx, dy);
        }

        /** Prepends a rotation (angle in radians) to the modelview matrix. */
        void RenderSupport::rotateMatrix(float angle)
        {
            MatrixUtil::prependRotation(mModelViewMatrix,angle);
        }

        /** Prepends an incremental scale change to the modelview matrix. */
        void RenderSupport::scaleMatrix(float sx, float sy)
        {
            MatrixUtil::prependScale(mModelViewMatrix,sx, sy);
        }

        /** Prepends a matrix to the modelview matrix by multiplying it with another matrix. */
        void RenderSupport::prependMatrix(Matrix* matrix)
        {
            MatrixUtil::prependMatrix(mModelViewMatrix,matrix);
        }

        /** Prepends translation, scale and rotation of an object to the modelview matrix. */
        void RenderSupport::transformMatrix(DisplayObject* object)
        {
            MatrixUtil::prependMatrix(mModelViewMatrix,object->transformationMatrix);
        }

        /** Pushes the current modelview matrix to a stack from which it can be restored later. */
        void RenderSupport::pushMatrix()
        {
            if (mMatrixStack.length < mMatrixStackSize + 1)
                mMatrixStack.push_back(newMatrix());

            mMatrixStack[int(mMatrixStackSize++)]->copyFrom(mModelViewMatrix);
        }

        /** Restores the modelview matrix that was last pushed to the stack. */
        void RenderSupport::popMatrix()
        {
            mModelViewMatrix->copyFrom(mMatrixStack[int(--mMatrixStackSize)]);
        }

        /** Empties the matrix stack, resets the modelview matrix to the identity matrix. */
        void RenderSupport::resetMatrix()
        {
            mMatrixStackSize = 0;
            loadIdentity();
        }

        /** Prepends translation, scale and rotation of an object to a custom matrix. */
        void RenderSupport::transformMatrixForObject(Matrix* matrix, DisplayObject* object)
        {
            MatrixUtil::prependMatrix(matrix,object->transformationMatrix);
        }

        /** Calculates the product of modelview and projection matrix. 
         *  CAUTION: Use with care! Each call returns the same instance. */
        Matrix* RenderSupport::mvpMatrix()
        {
            mMvpMatrix->copyFrom(mModelViewMatrix);
            mMvpMatrix->concat(mProjectionMatrix);
            return mMvpMatrix;
        }

        /** Calculates the product of modelview and projection matrix and saves it in a 3D matrix. 
         *  CAUTION: Use with care! Each call returns the same instance. */
        Matrix3D* RenderSupport::mvpMatrix3D()
        {
            return MatrixUtil::convertTo3D(mvpMatrix,mMvpMatrix3D);
        }

        /** Returns the current modelview matrix.
         *  CAUTION: Use with care! Each call returns the same instance. */
        Matrix* RenderSupport::modelViewMatrix()        { return mModelViewMatrix; }

        /** Returns the current projection matrix.
         *  CAUTION: Use with care! Each call returns the same instance. */
        Matrix* RenderSupport::projectionMatrix()        { return mProjectionMatrix; }
        void RenderSupport::projectionMatrix(Matrix* value)
        {
            mProjectionMatrix->copyFrom(value);
            applyClipRect();
        }

        // blending

        /** Activates the current blend mode on the active rendering context. */
        void RenderSupport::applyBlendMode(bool premultipliedAlpha)
        {
            setBlendFactors(premultipliedAlpha, mBlendMode);
        }

        /** The blend mode to be used on rendering. To apply the factor, you have to manually call
         *  'applyBlendMode' (because the actual blend factors depend on the PMA mode). */
        std::string RenderSupport::blendMode()        { return mBlendMode; }
        void RenderSupport::blendMode(std::string value)
        {
            if (value != BlendMode::AUTO)mBlendMode = value;
        }

        // render targets

        /** The texture that is currently being rendered into, or 'null' to render into the 
         *  back buffer. If you set a new target, it is immediately activated. */
        Texture* RenderSupport::renderTarget()         { return mRenderTarget; }
        void RenderSupport::renderTarget(Texture* target)
        {
            mRenderTarget = target;
            applyClipRect();

            if (target) Starling->context->setRenderToTexture(target->base);
            else        Starling->context->setRenderToBackBuffer();
        }

        /** Configures the back buffer on the current context3D. By using this method, Starling
         *  can store the size of the back buffer and utilize this information in other methods
         *  (e.g. the 'clipRect' property). Back buffer width and height can later be accessed
         *  using the properties with the same name. */
        void RenderSupport::configureBackBuffer(int width, int height, int antiAlias,
                                            bool enableDepthAndStencil,
                                            bool wantsBestResolution)
        {
            mBackBufferWidth  = width;
            mBackBufferHeight = height;

             Function* configureBackBuffer=Starling->context->configureBackBuffer;
             std::vector<void*> methodArgs=[width,height,antiAlias,enableDepthAndStencil];
            if (configureBackBuffer->length> 4) methodArgs->push(wantsBestResolution);
            configureBackBuffer->apply(Starling->context,methodArgs);
        }

        /** The width of the back buffer, as it was configured in the last call to 
         *  'RenderSupport.configureBackBuffer()'. Beware: changing this value does not actually
         *  resize the back buffer; the setter should only be used to inform Starling about the
         *  size of a back buffer it can't control (shared context situations).
         */
        int RenderSupport::backBufferWidth()     { return mBackBufferWidth; }
        void RenderSupport::backBufferWidth(int value)      { mBackBufferWidth = value; }

        /** The height of the back buffer, as it was configured in the last call to 
         *  'RenderSupport.configureBackBuffer()'. Beware: changing this value does not actually
         *  resize the back buffer; the setter should only be used to inform Starling about the
         *  size of a back buffer it can't control (shared context situations).
         */
        int RenderSupport::backBufferHeight()     { return mBackBufferHeight; }
        void RenderSupport::backBufferHeight(int value)      { mBackBufferHeight = value; }

        // clipping

        /** The clipping rectangle can be used to limit rendering in the current render target to
         *  a certain area. This method expects the rectangle in stage coordinates. Internally,
         *  it uses the 'scissorRectangle' of stage3D, which works with pixel coordinates. 
         *  Any pushed rectangle is intersected with the previous rectangle; the method returns
         *  that intersection. */
        Rectangle* RenderSupport::pushClipRect(Rectangle* rectangle)
        {
            if (mClipRectStack.length < mClipRectStackSize + 1)
                mClipRectStack.push_back(newRectangle());

            mClipRectStack[mClipRectStackSize]->copyFrom(rectangle);
            rectangle = mClipRectStack[mClipRectStackSize];

            // intersect with the last pushed clip rect
            if (mClipRectStackSize > 0)
                RectangleUtil::intersect(rectangle,mClipRectStack[mClipRectStackSize-1],
                                        rectangle);

            ++mClipRectStackSize;
            applyClipRect();

            // return the intersected clip rect so callers can skip draw calls if it's empty
            return rectangle;
        }

        /** Restores the clipping rectangle that was last pushed to the stack. */
        void RenderSupport::popClipRect()
        {
            if (mClipRectStackSize > 0)
            {
                --mClipRectStackSize;
                applyClipRect();
            }
        }

        /** Updates the context3D scissor rectangle using the current clipping rectangle. This
         *  method is called automatically when either the render target, the projection matrix,
         *  or the clipping rectangle changes. */
        void RenderSupport::applyClipRect()
        {
            finishQuadBatch();

             Context3D* context=Starling->context;
            if (context == NULL) return;

            if (mClipRectStackSize > 0)
            {
                 Rectangle* rect=mClipRectStack[mClipRectStackSize-1];
                sRectangle->setTo(rect->x,rect->y,rect->width,rect->height);

                 int width = mRenderTarget ? mRenderTarget->root->nativeWidth: mBackBufferWidth;
                 int height= mRenderTarget ? mRenderTarget->root->nativeHeight:mBackBufferHeight;

                // convert to pixel coordinates
                MatrixUtil::transformCoords(mProjectionMatrix,rect->x,rect->y,sPoint);
                sRectangle->x= Math::max(0,( sPoint->x+ 1) / 2) * width;
                sRectangle->y= Math::max(0,(-sPoint->y+ 1) / 2) * height;

                MatrixUtil::transformCoords(mProjectionMatrix,rect->right,rect->bottom,sPoint);
                sRectangle->right = Math::min(1,( sPoint->x+ 1) / 2) * width;
                sRectangle->bottom= Math::min(1,(-sPoint->y+ 1) / 2) * height;

                // an empty rectangle is not allowed, so we set it to the smallest possible size
                // if the bounds are outside the visible area.
                if (sRectangle->width< 1 || sRectangle->height< 1)
                    sRectangle->setTo(0,0, 1, 1);

                context->setScissorRectangle(sRectangle);
            }
            else
            {
                context->setScissorRectangle(NULL);
            }
        }

        // optimized quad rendering

        /** Adds a quad to the current batch of unrendered quads. If there is a state change,
         *  all previous quads are rendered at once, and the batch is reset. */
        void RenderSupport::batchQuad(Quad* quad, float parentAlpha,
                                  Texture* texture, std::string smoothing)
        {
            if (mQuadBatches[mCurrentQuadBatchID]->isStateChange(quad->tinted,parentAlpha,texture,
                                                                smoothing, mBlendMode))
            {
                finishQuadBatch();
            }

            mQuadBatches[mCurrentQuadBatchID]->addQuad(quad,parentAlpha, texture, smoothing,
                                                      mModelViewMatrix, mBlendMode);
        }

        /** Renders the current quad batch and resets it. */
        void RenderSupport::finishQuadBatch()
        {
             QuadBatch* currentBatch=mQuadBatches[mCurrentQuadBatchID];

            if (currentBatch->numQuads!= 0)
            {
                currentBatch->renderCustom(mProjectionMatrix);
                currentBatch->reset();

                ++mCurrentQuadBatchID;
                ++mDrawCount;

                if (mQuadBatches.length <= mCurrentQuadBatchID)
                    mQuadBatches.push_back(newQuadBatch());
            }
        }

        /** Resets matrix stack, blend mode, quad batch index, and draw count. */
        void RenderSupport::nextFrame()
        {
            resetMatrix();
            mBlendMode = BlendMode::NORMAL;
            mCurrentQuadBatchID = 0;
            mDrawCount = 0;
        }

        // other helper methods

        /** Deprecated. Call 'setBlendFactors' instead. */
        void RenderSupport::setDefaultBlendFactors(bool premultipliedAlpha)
        {
            setBlendFactors(premultipliedAlpha);
        }

        /** Sets up the blending factors that correspond with a certain blend mode. */
        void RenderSupport::setBlendFactors(bool premultipliedAlpha, std::string blendMode)
        {
             std::vector<void*> blendFactors=BlendMode::getBlendFactors(blendMode,premultipliedAlpha);
            Starling->context->setBlendFactors(blendFactors[0],blendFactors[1]);
        }

        /** Clears the render context with a certain color and alpha value. */
        void RenderSupport::clear(unsigned int rgb, float alpha)
        {
            Starling->context->clear(
                Color::getRed(rgb)  / 255.0,
                Color::getGreen(rgb)/ 255.0,
                Color::getBlue(rgb) / 255.0,
                alpha);
        }

        /** Clears the render context with a certain color and alpha value. */
        void RenderSupport::clear(unsigned int rgb, float alpha)
        {
            RenderSupport->clear(rgb,alpha);
        }

        /** Assembles fragment- and vertex-shaders, passed as Strings, to a Program3D. If you
         *  pass a 'resultProgram', it will be uploaded to that program; otherwise, a new program
         *  will be created on the current Stage3D context. */
        Program3D* RenderSupport::assembleAgal(std::string vertexShader, std::string fragmentShader,
                                            Program3D* resultProgram)
        {
            if (resultProgram == NULL)
            {
                 Context3D* context=Starling->context;
                if (context == NULL) throw new MissingContextError();
                resultProgram = context->createProgram();
            }

            resultProgram->upload(
                sAssembler->assemble(Context3DProgramType::VERTEX,vertexShader),
                sAssembler->assemble(Context3DProgramType::FRAGMENT,fragmentShader));

            return resultProgram;
        }

        // statistics

        /** Raises the draw count by a specific value. Call this method in custom render methods
         *  to keep the statistics display in sync. */
        void RenderSupport::raiseDrawCount(unsigned int value)      { mDrawCount += value; }

        /** Indicates the number of stage3D draw calls. */
        int RenderSupport::drawCount()     { return mDrawCount; }
}
}

