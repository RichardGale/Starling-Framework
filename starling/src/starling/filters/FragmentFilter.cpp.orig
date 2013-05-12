// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "FragmentFilter.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DProgramType.h"
#include "flash/display3D/Context3DVertexBufferFormat.h"
#include "flash/display3D/IndexBuffer3D.h"
#include "flash/display3D/Program3D.h"
#include "flash/display3D/VertexBuffer3D.h"
#include "flash/errors/IllegalOperationError.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/RenderSupport.h"
#include "starling/core/Starling.h"
#include "starling/core/starling_internal.h"
#include "starling/display/BlendMode.h"
#include "starling/display/DisplayObject.h"
#include "starling/display/Image.h"
#include "starling/display/QuadBatch.h"
#include "starling/display/Stage.h"
#include "starling/errors/AbstractClassError.h"
#include "starling/errors/MissingContextError.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"
#include "starling/utils/MatrixUtil.h"
#include "starling/utils/RectangleUtil.h"
#include "starling/utils/VertexData.h"
#include "starling/utils/getNextPowerOfTwo.h"

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
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::errors;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::system;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::core;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;
using namespace starling::utils;
using namespace starling::utils;
using namespace starling::utils;

namespace starling {
namespace filters {


        /** All filter processing is expected to be done with premultiplied alpha. */

        /** The standard vertex shader code. It will be used automatically if you don't create
         *  a custom vertex shader yourself. */

                                    // pass texture coordinates to fragment program

        /** The standard fragment shader code. It just forwards the texture color to the output. */
                                                            // just forward texture color

        /** helper objects. */
         Rectangle* FragmentFilter::sBounds=newRectangle();
         Rectangle* FragmentFilter::sStageBounds=newRectangle();
         Matrix* FragmentFilter::sTransformationMatrix=newMatrix();

        /** Creates a new Fragment filter with the specified number of passes and resolution.
         *  This constructor may only be called by the constructor of a subclass. */
        FragmentFilter::FragmentFilter(int numPasses, float resolution)
        {
            if (Capabilities::isDebugger&&
                getQualifiedClassName(this) == "starling.filters::FragmentFilter")
            {
                throw new AbstractClassError();
            }

            if (numPasses < 1) throw new ArgumentError("At least one pass is required.");

            mNumPasses = numPasses;
            mMarginX = mMarginY = 0.0;
            mOffsetX = mOffsetY = 0;
            mResolution = resolution;
            mMode = FragmentFilterMode->REPLACE;

            mVertexData = new VertexData(4);
            mVertexData->setTexCoords(0,0, 0);
            mVertexData->setTexCoords(1,1, 0);
            mVertexData->setTexCoords(2,0, 1);
            mVertexData->setTexCoords(3,1, 1);

            mIndexData.clear();
            mIndexData.fixed = true;

            createPrograms();

            // Handle lost context. By using the conventional event, we can make it weak; this  
            // avoids memory leaks when people forget to call "dispose" on the filter.
            Starling::current->stage3D->addEventListener(Event::CONTEXT3D_CREATE,
                onContextCreated, false, 0, true);
        }

        /** Disposes the filter (programs, buffers, textures). */
        void FragmentFilter::dispose()
        {
            Starling::current->stage3D->removeEventListener(Event::CONTEXT3D_CREATE,onContextCreated);
            if (mVertexBuffer) mVertexBuffer->dispose();
            if (mIndexBuffer)  mIndexBuffer->dispose();
            disposePassTextures();
            disposeCache();
        }

        void FragmentFilter::onContextCreated(Object* event)
        {
            mVertexBuffer = NULL;
            mIndexBuffer  = NULL;
            mPassTextures.clear();

            createPrograms();
        }

        /** Applies the filter on a certain display object, rendering the output into the current 
         *  render target. This method is called automatically by Starling's rendering system 
         *  for the object the filter is attached to. */
        void FragmentFilter::render(DisplayObject* object, RenderSupport* support, float parentAlpha)
        {
            // bottom layer

            if (mode == FragmentFilterMode->ABOVE)
                object->render(support,parentAlpha);

            // center layer

            if (mCacheRequested)
            {
                mCacheRequested = false;
                mCache = renderPasses(object, support, 1.0, true);
                disposePassTextures();
            }

            if (mCache)
                mCache->render(support,parentAlpha);
            else
                renderPasses(object, support, parentAlpha, false);

            // top layer

            if (mode == FragmentFilterMode->BELOW)
                object->render(support,parentAlpha);
        }

        QuadBatch* FragmentFilter::renderPasses(DisplayObject* object, RenderSupport* support,
                                      float parentAlpha, bool intoCache)
        {
             Texture* cacheTexture=NULL;
             Stage* stage=object->stage;
             Context3D* context=Starling::context;
             float scale = Starling::current->contentScaleFactor;

            if (stage   == NULL) throw new Error("Filtered object must be on the stage.");
            if (context == NULL) throw new MissingContextError();

            // the bounds of the object in stage coordinates 
            calculateBounds(object, stage, !intoCache, sBounds);

            if (sBounds->isEmpty())
            {
                disposePassTextures();
                return intoCache ? new QuadBatch() : NULL;
            }

            updateBuffers(context, sBounds);
            updatePassTextures(sBounds->width,sBounds->height,mResolution * scale);

            support->finishQuadBatch();
            support->raiseDrawCount(mNumPasses);
            support->pushMatrix();

            // save original projection matrix and render target
            mProjMatrix->copyFrom(support->projectionMatrix);
             Texture* previousRenderTarget=support->renderTarget;

            if (previousRenderTarget)
                throw new IllegalOperationError(
                    "It's currently not possible to stack filters! " +
                    "This limitation will be removed in a future Stage3D version.");

            if (intoCache)
                cacheTexture = Texture::empty(sBounds->width,sBounds->height,PMA,true,
                                             mResolution * scale);

            // draw the original object into a texture
            support->renderTarget= mPassTextures[0];
            support->clear();
            support->blendMode= BlendMode::NORMAL;
            support->setOrthographicProjection(sBounds->x,sBounds->y,sBounds->width,sBounds->height);
            object->render(support,parentAlpha);
            support->finishQuadBatch();

            // prepare drawing of actual filter passes
            RenderSupport::setBlendFactors(PMA);
            support->loadIdentity(); // now we'll draw in stage coordinates!

            context->setVertexBufferAt(mVertexPosAtID,mVertexBuffer, VertexData::POSITION_OFFSET,
                                      Context3DVertexBufferFormat::FLOAT_2);
            context->setVertexBufferAt(mTexCoordsAtID,mVertexBuffer, VertexData::TEXCOORD_OFFSET,
                                      Context3DVertexBufferFormat::FLOAT_2);

            // draw all passes
            for ( int i=0;i<mNumPasses; ++i)
            {
                if (i < mNumPasses - 1) // intermediate pass  
                {
                    // draw into pass texture
                    support->renderTarget= getPassTexture(i+1);
                    support->clear();
                }
                else
                {
                    if (intoCache)
                    {
                        // draw into cache texture
                        support->renderTarget= cacheTexture;
                        support->clear();
                    }
                    else
                    {
                        // draw into back buffer, at original (stage) coordinates
                        support->projectionMatrix= mProjMatrix;
                        support->renderTarget= previousRenderTarget;
                        support->translateMatrix(mOffsetX,mOffsetY);
                        support->blendMode= object->blendMode;
                        support->applyBlendMode(PMA);
                    }
                }

                 Texture* passTexture=getPassTexture(i);

                context->setProgramConstantsFromMatrix(Context3DProgramType::VERTEX,mMvpConstantID,
                                                      support->mvpMatrix3D,true);
                context->setTextureAt(mBaseTextureID,passTexture->base);

                activate(i, context, passTexture);
                context->drawTriangles(mIndexBuffer,0, 2);
                deactivate(i, context, passTexture);
            }

            // reset shader attributes
            context->setVertexBufferAt(mVertexPosAtID,NULL);
            context->setVertexBufferAt(mTexCoordsAtID,NULL);
            context->setTextureAt(mBaseTextureID,NULL);

            support->popMatrix();

            if (intoCache)
            {
                // restore support settings
                support->renderTarget= previousRenderTarget;
                support->projectionMatrix->copyFrom(mProjMatrix);

                // Create an image containing the cache. To have a display object that contains
                // the filter output in object coordinates, we wrap it in a QuadBatch: that way,
                // we can modify it with a transformation matrix.

                 QuadBatch* quadBatch=new QuadBatch();
                 Image* image=new Image(cacheTexture);

                stage->getTransformationMatrix(object,sTransformationMatrix);
                MatrixUtil::prependTranslation(sTransformationMatrix,
                                              sBounds->x+ mOffsetX, sBounds->y+ mOffsetY);
                quadBatch->addImage(image,1.0, sTransformationMatrix);

                return quadBatch;
            }
            else return NULL;
        }

        // helper methods// final pass

        void FragmentFilter::updateBuffers(Context3D* context, Rectangle* bounds)
        {
            mVertexData->setPosition(0,bounds->x,bounds->y);
            mVertexData->setPosition(1,bounds->right,bounds->y);
            mVertexData->setPosition(2,bounds->x,bounds->bottom);
            mVertexData->setPosition(3,bounds->right,bounds->bottom);

            if (mVertexBuffer == NULL)
            {
                mVertexBuffer = context->createVertexBuffer(4,VertexData::ELEMENTS_PER_VERTEX);
                mIndexBuffer  = context->createIndexBuffer(6);
                mIndexBuffer->uploadFromVector(mIndexData,0, 6);
            }

            mVertexBuffer->uploadFromVector(mVertexData->rawData,0,4);
        }

        void FragmentFilter::updatePassTextures(int width, int height, float scale)
        {
             int numPassTextures= mNumPasses > 1 ? 2 : 1;

             bool needsUpdate   = mPassTextures.empty() ||
                mPassTextures.length != numPassTextures ||
                mPassTextures[0]->width!= width || mPassTextures[0]->height!= height;

            if (needsUpdate)
            {
                if (mPassTextures)
                {
                    for each (var Texture* texturein mPassTextures)
                        texture->dispose();

                    mPassTextures.length = numPassTextures;
                }
                else
                {
                    mPassTextures.clear();
                }

                for ( int i=0;i<numPassTextures; ++i)
                    mPassTextures[i] = Texture::empty(width,height, PMA, true, scale);
            }
        }

        Texture* FragmentFilter::getPassTexture(int pass)
        {
            return mPassTextures[pass % 2];
        }

        /** Calculates the bounds of the filter in stage coordinates, while making sure that the 
         *  according textures will have powers of two. */
        void FragmentFilter::calculateBounds(DisplayObject* object, Stage* stage,
                                         bool intersectWithStage, Rectangle* resultRect)
        {
            // optimize for full-screen effects
            if (object == stage || object == Starling::current->root)
                resultRect->setTo(0,0, stage->stageWidth,stage->stageHeight);
            else
                object->getBounds(stage,resultRect);

            if (intersectWithStage)
            {
                sStageBounds->setTo(0,0, stage->stageWidth,stage->stageHeight);
                RectangleUtil::intersect(resultRect,sStageBounds, resultRect);
            }

            if (!resultRect->isEmpty())
            {
                // the bounds are a rectangle around the object, in stage coordinates,
                // and with an optional margin. To fit into a POT-texture, it will grow towards
                // the right and bottom.
                 float deltaMargin = mResolution == 1.0 ? 0.0 : 1.0 / mResolution; // avoid hard edges
                resultRect->x-= mMarginX + deltaMargin;
                resultRect->y-= mMarginY + deltaMargin;
                resultRect->width += 2 * (mMarginX + deltaMargin);
                resultRect->height+= 2 * (mMarginY + deltaMargin);
                resultRect->width = getNextPowerOfTwo(resultRect->width * mResolution) / mResolution;
                resultRect->height= getNextPowerOfTwo(resultRect->height* mResolution) / mResolution;
            }
        }

        void FragmentFilter::disposePassTextures()
        {
            for each (var Texture* texturein mPassTextures)
                texture->dispose();

            mPassTextures.clear();
        }

        void FragmentFilter::disposeCache()
        {
            if (mCache)
            {
                if (mCache->texture)mCache->texture->dispose();
                mCache->dispose();
                mCache = NULL;
            }
        }

        // protected methods

        /** Subclasses must override this method and use it to create their 
         *  fragment- and vertex-programs. */
        void FragmentFilter::createPrograms()
        {
            throw new Error("Method has to be implemented in subclass!");
        }

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
        void FragmentFilter::activate(int pass, Context3D* context, Texture* texture)
        {
            throw new Error("Method has to be implemented in subclass!");
        }

        /** This method is called directly after 'context.drawTriangles'. 
         *  If you need to clean up any resources, you can do so in this method. */
        void FragmentFilter::deactivate(int pass, Context3D* context, Texture* texture)
        {
            // clean up resources
        }

        /** Assembles fragment- and vertex-shaders, passed as Strings, to a Program3D. 
         *  If any argument is  null, it is replaced by the class constants STD_FRAGMENT_SHADER or
         *  STD_VERTEX_SHADER, respectively. */
        Program3D* FragmentFilter::assembleAgal(std::string fragmentShader, std::string vertexShader)
        {
            if (fragmentShader == NULL) fragmentShader = STD_FRAGMENT_SHADER;
            if (vertexShader   == NULL) vertexShader   = STD_VERTEX_SHADER;

            return RenderSupport::assembleAgal(vertexShader,fragmentShader);
        }

        // cache

        /** Caches the filter output into a texture. An uncached filter is rendered in every frame;
         *  a cached filter only once. However, if the filtered object or the filter settings
         *  change, it has to be updated manually; to do that, call "cache" again. */
        void FragmentFilter::cache()
        {
            mCacheRequested = true;
            disposeCache();
        }

        /** Clears the cached output of the filter. After calling this method, the filter will
         *  be executed once per frame again. */
        void FragmentFilter::clearCache()
        {
            mCacheRequested = false;
            disposeCache();
        }

        // flattening

        /** @private */
        QuadBatch* FragmentFilter::compile(DisplayObject* object)
        {
            if (mCache) return mCache;
            else
            {
                 RenderSupport* renderSupport;
                 Stage* stage=object->stage;

                if (stage == NULL)
                    throw new Error("Filtered object must be on the stage.");

                renderSupport = new RenderSupport();
                object->getTransformationMatrix(stage,renderSupport->modelViewMatrix);
                return renderPasses(object, renderSupport, 1.0, true);
            }
        }

        // properties

        /** Indicates if the filter is cached (via the "cache" method). */
        bool FragmentFilter::isCached()         { return (mCache != NULL) || mCacheRequested; }

        /** The resolution of the filter texture. "1" means stage resolution, "0.5" half the
         *  stage resolution. A lower resolution saves memory and execution time (depending on 
         *  the GPU), but results in a lower output quality. Values greater than 1 are allowed;
         *  such values might make sense for a cached filter when it is scaled up. @default 1 */
        float FragmentFilter::resolution()        { return mResolution; }
        void FragmentFilter::resolution(float value)
        {
            if (value <= 0) throw new ArgumentError("Resolution must be > 0");
            else mResolution = value;
        }

        /** The filter mode, which is one of the constants defined in the "FragmentFilterMode" 
         *  class. @default "replace" */
        std::string FragmentFilter::mode()        { return mMode; }
        void FragmentFilter::mode(std::string value)      { mMode = value; }

        /** Use the x-offset to move the filter output to the right or left. */
        float FragmentFilter::offsetX()        { return mOffsetX; }
        void FragmentFilter::offsetX(float value)      { mOffsetX = value; }

        /** Use the y-offset to move the filter output to the top or bottom. */
        float FragmentFilter::offsetY()        { return mOffsetY; }
        void FragmentFilter::offsetY(float value)      { mOffsetY = value; }

        /** The x-margin will extend the size of the filter texture along the x-axis.
         *  Useful when the filter will "grow" the rendered object. */
        float FragmentFilter::marginX()        { return mMarginX; }
        void FragmentFilter::marginX(float value)      { mMarginX = value; }

        /** The y-margin will extend the size of the filter texture along the y-axis.
         *  Useful when the filter will "grow" the rendered object. */
        float FragmentFilter::marginY()        { return mMarginY; }
        void FragmentFilter::marginY(float value)      { mMarginY = value; }

        /** The number of passes the filter is applied. The "activate" and "deactivate" methods
         *  will be called that often. */
        void FragmentFilter::numPasses(int value)      { mNumPasses = value; }
        int FragmentFilter::numPasses()     { return mNumPasses; }

        /** The ID of the vertex buffer attribute that stores the vertex position. */
                  int FragmentFilter::vertexPosAtID()     { return mVertexPosAtID; }
                  void FragmentFilter::vertexPosAtID(int value)      { mVertexPosAtID = value; }

        /** The ID of the vertex buffer attribute that stores the texture coordinates. */
                  int FragmentFilter::texCoordsAtID()     { return mTexCoordsAtID; }
                  void FragmentFilter::texCoordsAtID(int value)      { mTexCoordsAtID = value; }

        /** The ID (sampler) of the input texture (containing the output of the previous pass). */
                  int FragmentFilter::baseTextureID()     { return mBaseTextureID; }
                  void FragmentFilter::baseTextureID(int value)      { mBaseTextureID = value; }

        /** The ID of the first register of the modelview-projection constant (a 4x4 matrix). */
                  int FragmentFilter::mvpConstantID()     { return mMvpConstantID; }
                  void FragmentFilter::mvpConstantID(int value)      { mMvpConstantID = value; }
}
}

