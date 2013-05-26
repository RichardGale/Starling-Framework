// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "QuadBatch.h"
#include "com/adobe/utils/AGALMiniAssembler.h"

#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DProgramType.h"
#include "flash/display3D/Context3DTextureFormat.h"
#include "flash/display3D/Context3DVertexBufferFormat.h"
#include "flash/display3D/IndexBuffer3D.h"
#include "flash/display3D/VertexBuffer3D.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Matrix3D.h"
#include "flash/geom/Rectangle.h"
#include "flash/utils/Dictionary.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/RenderSupport.h"
#include "starling/core/Starling.h"
#include "starling/core/starling_internal.h"
#include "starling/errors/MissingContextError.h"
#include "starling/events/Event.h"
#include "starling/filters/FragmentFilter.h"
#include "starling/filters/FragmentFilterMode.h"
#include "starling/textures/Texture.h"
#include "starling/textures/TextureSmoothing.h"
#include "starling/utils/MatrixUtil.h"
#include "starling/utils/VertexData.h"
#include "starling/display/Image.h"
#include "starling/display/Quad.h"

    //use namespace starling_internal;

    /** Optimizes rendering of a number of quads with an identical state.
     * 
     *  <p>The majority of all rendered objects in Starling are quads. In fact, all the default
     *  leaf nodes of Starling are quads (the Image and Quad classes). The rendering of those 
     *  quads can be accelerated by a big factor if all quads with an identical state are sent 
     *  to the GPU in just one call. That's what the QuadBatch class can do.</p>
     *  
     *  <p>The 'flatten' method of the Sprite class uses this class internally to optimize its 
     *  rendering performance. In most situations, it is recommended to stick with flattened
     *  sprites, because they are easier to use. Sometimes, however, it makes sense
     *  to use the QuadBatch class directly: e.g. you can add one quad multiple times to 
     *  a quad batch, whereas you can only add it once to a sprite. Furthermore, this class
     *  does not dispatch <code>ADDED</code> or <code>ADDED_TO_STAGE</code> events when a quad
     *  is added, which makes it more lightweight.</p>
     *  
     *  <p>One QuadBatch object is bound to a specific render state. The first object you add to a 
     *  batch will decide on the QuadBatch's state, that is: its texture, its settings for 
     *  smoothing and blending, and if it's tinted (colored vertices and/or transparency). 
     *  When you reset the batch, it will accept a new state on the next added quad.</p> 
     *  
     *  <p>The class extends DisplayObject, but you can use it even without adding it to the
     *  display tree. Just call the 'renderCustom' method from within another render method,
     *  and pass appropriate values for transformation matrix, alpha and blend mode.</p>
     *
     *  @see Sprite  
     */

using namespace com::adobe::utils;
using namespace flash::display3D;
using namespace flash::geom;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::textures;
using namespace starling::utils;

namespace starling {
namespace display {


        const std::string QuadBatch::QUAD_PROGRAM_NAME = "QB_q";

                    
                    

                    
                    
                    

                    
                    
                    
                    

        /** Helper objects. */
        Matrix* QuadBatch::sHelperMatrix = new Matrix();
        std::vector<float> QuadBatch::sRenderAlpha=std::vector<float>()                              ;
        Matrix3D* QuadBatch::sRenderMatrix = new Matrix3D();
        std::map<std::string, void*> QuadBatch::sProgramNameCache   std::map<std::string, void*>()                ;

        /** Creates a new QuadBatch instance with empty batch data. */
        QuadBatch::QuadBatch()
        {
            mVertexData = new VertexData(0, true);
            mIndexData.clear();
            mNumQuads = 0;
            mTinted = false;
            mSyncRequired = false;

            // Handle lost context. We use the conventional event here (not the one from Starling)
            // so we're able to create a weak event listener; this avoids memory leaks when people 
            // forget to call "dispose" on the QuadBatch.
            Starling::current()->stage3D()->addEventListener(Event::CONTEXT3D_CREATE,
                                                      onContextCreated, false, 0, true);
        }

        /** Disposes vertex- and index-buffer. */
        void QuadBatch::dispose()
        {
            Starling::current()->stage3D()->removeEventListener(Event::CONTEXT3D_CREATE, onContextCreated);

            if (mVertexBuffer) mVertexBuffer->dispose();
            if (mIndexBuffer)  mIndexBuffer->dispose();

            DisplayObject::dispose();
        }

        void QuadBatch::onContextCreated(Object* event)
        {
            createBuffers();
            registerPrograms();
        }

        /** Creates a duplicate of the QuadBatch object. */
        QuadBatch* QuadBatch::clone()
        {
            QuadBatch* clone = new QuadBatch();
            clone->mVertexData = mVertexData->clone(0, mNumQuads * 4);
            clone->mIndexData = mIndexData.slice(0, mNumQuads * 6);
            clone->mNumQuads = mNumQuads;
            clone->mTinted = mTinted;
            clone->mTexture = mTexture;
            clone->mSmoothing = mSmoothing;
            clone->mSyncRequired = true;
            clone->blendMode = blendMode;
            clone->alpha = alpha;
            return clone;
        }

        void QuadBatch::expand(int newCapacity)
        {
            int oldCapacity = capacity;

            if (newCapacity <  0) newCapacity = oldCapacity * 2;
            if (newCapacity == 0) newCapacity = 16;
            if (newCapacity <= oldCapacity) return;

            mVertexData->numVertices ( newCapacity * 4);

            for (int i=oldCapacity; i<newCapacity; ++i)
            {
                mIndexData[int(i*6  )] = i*4;
                mIndexData[int(i*6+1)] = i*4 + 1;
                mIndexData[int(i*6+2)] = i*4 + 2;
                mIndexData[int(i*6+3)] = i*4 + 1;
                mIndexData[int(i*6+4)] = i*4 + 3;
                mIndexData[int(i*6+5)] = i*4 + 2;
            }

            createBuffers();
            registerPrograms();
        }

        void QuadBatch::createBuffers()
        {
            int numVertices = mVertexData->numVertices;
            int numIndices = mIndexData.size();
            Context3D* context = Starling::context;

            if (mVertexBuffer)    mVertexBuffer->dispose();
            if (mIndexBuffer)     mIndexBuffer->dispose();
            if (numVertices == 0) return;
            if (context == NULL)  throw new MissingContextError();

            mVertexBuffer = context->createVertexBuffer(numVertices, VertexData::ELEMENTS_PER_VERTEX);
            mVertexBuffer->uploadFromVector(mVertexData->rawData(), 0, numVertices);

            mIndexBuffer = context->createIndexBuffer(numIndices);
            mIndexBuffer->uploadFromVector(mIndexData, 0, numIndices);

            mSyncRequired = false;
        }

        /** Uploads the raw data of all batched quads to the vertex buffer. */
        void QuadBatch::syncBuffers()
        {
            if (mVertexBuffer == NULL)
                createBuffers();
            else
            {
                // as 3rd parameter, we could also use 'mNumQuads * 4', but on some GPU hardware (iOS!),
                // this is slower than updating the complete buffer.

                mVertexBuffer->uploadFromVector(mVertexData->rawData(), 0, mVertexData->numVertices());
                mSyncRequired = false;
            }
        }

        /** Renders the current batch with custom settings for model-view-projection matrix, alpha 
         *  and blend mode. This makes it possible to render batches that are not part of the 
         *  display list. */
        void QuadBatch::renderCustom(Matrix* mvpMatrix, float parentAlpha,
                                     std::string blendMode)
        {
            if (mNumQuads == 0) return;
            if (mSyncRequired) syncBuffers();

            bool pma = mVertexData->premultipliedAlpha();
            Context3D* context = Starling::context;
            bool tinted = mTinted || (parentAlpha != 1.0);
            std::string programName = mTexture ?
                getImageProgramName(tinted, mTexture->mipMapping(), mTexture->repeat(), mTexture->format(), mSmoothing) :
                QUAD_PROGRAM_NAME;

            sRenderAlpha[0] = sRenderAlpha[1] = sRenderAlpha[2] = pma ? parentAlpha : 1.0;
            sRenderAlpha[3] = parentAlpha;

            MatrixUtil::convertTo3D(mvpMatrix, sRenderMatrix);
            RenderSupport::setBlendFactors(pma, blendMode ? blendMode : this->blendMode);

            context->setProgram(Starling::current()->getProgram(programName));
            context->setProgramConstantsFromVector(Context3DProgramType::VERTEX, 0, sRenderAlpha, 1);
            context->setProgramConstantsFromMatrix(Context3DProgramType::VERTEX, 1, sRenderMatrix, true);
            context->setVertexBufferAt(0, mVertexBuffer, VertexData::POSITION_OFFSET,
                                      Context3DVertexBufferFormat::FLOAT_2);

            if (mTexture == NULL || tinted)
                context->setVertexBufferAt(1, mVertexBuffer, VertexData::COLOR_OFFSET,
                                          Context3DVertexBufferFormat::FLOAT_4);

            if (mTexture)
            {
                context->setTextureAt(0, mTexture->base());
                context->setVertexBufferAt(2, mVertexBuffer, VertexData::TEXCOORD_OFFSET,
                                          Context3DVertexBufferFormat::FLOAT_2);
            }

            context->drawTriangles(mIndexBuffer, 0, mNumQuads * 2);

            if (mTexture)
            {
                context->setTextureAt(0, NULL);
                context->setVertexBufferAt(2, NULL);
            }

            context->setVertexBufferAt(1, NULL);
            context->setVertexBufferAt(0, NULL);
        }

        /** Resets the batch. The vertex- and index-buffers remain their size, so that they
         *  can be reused quickly. */
        void QuadBatch::reset()
        {
            mNumQuads = 0;
            mTexture = NULL;
            mSmoothing = NULL;
            mSyncRequired = true;
        }

        /** Adds an image to the batch. This method internally calls 'addQuad' with the correct
         *  parameters for 'texture' and 'smoothing'. */
        void QuadBatch::addImage(Image* image, float parentAlpha, Matrix* modelViewMatrix,
                                 std::string blendMode)
        {
            addQuad(image, parentAlpha, image->texture(), image->smoothing(), modelViewMatrix, blendMode);
        }

        /** Adds a quad to the batch. The first quad determines the state of the batch,
         *  i.e. the values for texture, smoothing and blendmode. When you add additional quads,  
         *  make sure they share that state (e.g. with the 'isStageChange' method), or reset
         *  the batch. */
        void QuadBatch::addQuad(Quad* quad, float parentAlpha, starling::textures::Texture* texture,
                                std::string smoothing, Matrix* modelViewMatrix,
                                std::string blendMode)
        {
            if (modelViewMatrix == NULL)
                modelViewMatrix = quad->transformationMatrix();

            float alpha = parentAlpha * quad->alpha;
            int vertexID = mNumQuads * 4;

            if (mNumQuads + 1 > mVertexData->numVertices() / 4) expand();
            if (mNumQuads == 0)
            {
                this->blendMode = blendMode ? blendMode : quad->blendMode;
                mTexture = texture;
                mTinted = texture ? (quad->tinted() || parentAlpha != 1.0) : false;
                mSmoothing = smoothing;
                mVertexData->setPremultipliedAlpha(quad->premultipliedAlpha());
            }

            quad->copyVertexDataTo(mVertexData, vertexID);
            mVertexData->transformVertex(vertexID, modelViewMatrix, 4);

            if (alpha != 1.0)
                mVertexData->scaleAlpha(vertexID, alpha, 4);

            mSyncRequired = true;
            mNumQuads++;
        }

        /** Adds another QuadBatch to this batch. Just like the 'addQuad' method, you have to
         *  make sure that you only add batches with an equal state. */
        void QuadBatch::addQuadBatch(QuadBatch* quadBatch, float parentAlpha,
                                     Matrix* modelViewMatrix, std::string blendMode)
        {
            if (modelViewMatrix == NULL)
                modelViewMatrix = quadBatch->transformationMatrix();

            bool tinted = quadBatch->mTinted || parentAlpha != 1.0;
            float alpha = parentAlpha * quadBatch->alpha;
            int vertexID = mNumQuads * 4;
            int numQuads = quadBatch->numQuads;

            if (mNumQuads + numQuads > capacity) expand(mNumQuads + numQuads);
            if (mNumQuads == 0)
            {
                this->blendMode = blendMode ? blendMode : quadBatch->blendMode;
                mTexture = quadBatch->mTexture;
                mTinted = tinted;
                mSmoothing = quadBatch->mSmoothing;
                mVertexData->setPremultipliedAlpha(quadBatch->mVertexData->premultipliedAlpha(), false);
            }

            quadBatch->mVertexData->copyTo(mVertexData, vertexID, 0, numQuads*4);
            mVertexData->transformVertex(vertexID, modelViewMatrix, numQuads*4);

            if (alpha != 1.0)
                mVertexData->scaleAlpha(vertexID, alpha, numQuads*4);

            mSyncRequired = true;
            mNumQuads += numQuads;
        }

        /** Indicates if specific quads can be added to the batch without causing a state change. 
         *  A state change occurs if the quad uses a different base texture, has a different 
         *  'tinted', 'smoothing', 'repeat' or 'blendMode' setting, or if the batch is full
         *  (one batch can contain up to 8192 quads). */
        bool QuadBatch::isStateChange(bool tinted, float parentAlpha, starling::textures::Texture* texture,
                                      std::string smoothing, std::string blendMode, int numQuads)
        {
            if (mNumQuads == 0) return false;
            else if (mNumQuads + numQuads > 8192) return true; // maximum buffer size
            else if (mTexture == NULL && texture == NULL)
                return this->blendMode != blendMode;
            else if (mTexture != NULL && texture != NULL)
                return mTexture->base() != texture->base() ||
                       mTexture->repeat() != texture->repeat() ||
                       mSmoothing != smoothing ||
                       mTinted != (tinted || parentAlpha != 1.0) ||
                       this->blendMode != blendMode;
            else return true;
        }

        // display object methods

        /** @inheritDoc */
        Rectangle* QuadBatch::getBounds(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            if (resultRect == NULL) resultRect = new Rectangle();

            Matrix* transformationMatrix = targetSpace == this ?
                NULL : getTransformationMatrix(targetSpace, sHelperMatrix);

            return mVertexData->getBounds(transformationMatrix, 0, mNumQuads*4, resultRect);
        }

        /** @inheritDoc */
        void QuadBatch::render(RenderSupport* support, float parentAlpha)
        {
            if (mNumQuads)
            {
                support->finishQuadBatch();
                support->raiseDrawCount();
                renderCustom(support->mvpMatrix(), alpha * parentAlpha, support->blendMode());
            }
        }

        // compilation (for flattened sprites)

        /** Analyses an object that is made up exclusively of quads (or other containers)
         *  and creates a vector of QuadBatch objects representing it. This can be
         *  used to render the container very efficiently. The 'flatten'-method of the Sprite 
         *  class uses this method internally. */
        void QuadBatch::compile(DisplayObject* object,
                                       std::vector<QuadBatch*> quadBatches)
        {
            compileObject(object, quadBatches, -1, new Matrix());
        }

        int QuadBatch::compileObject(DisplayObject* object,
                                              std::vector<QuadBatch*> quadBatches,
                                              int quadBatchID,
                                              Matrix* transformationMatrix,
                                              float alpha,
                                              std::string blendMode,
                                              bool ignoreCurrentFilter)
        {
            int i;
            QuadBatch* quadBatch;
            bool isRootObject = false;
            float objectAlpha = object->alpha;

            DisplayObjectContainer* container = dynamic_cast<DisplayObjectContainer*>(object);
            Quad* quad = dynamic_cast<Quad*>(object);
            QuadBatch* batch = dynamic_cast<QuadBatch*>(object);
            FragmentFilter* filter = object->filter;

            if (quadBatchID == -1)
            {
                isRootObject = true;
                quadBatchID = 0;
                objectAlpha = 1.0;
                blendMode = object->blendMode;
                if (quadBatches.size() == 0) quadBatches.push_back(new QuadBatch());
                else quadBatches[0]->reset();
            }

            if (filter && !ignoreCurrentFilter)
            {
                if (filter->mode() == FragmentFilterMode::ABOVE)
                {
                    quadBatchID = compileObject(object, quadBatches, quadBatchID,
                                                transformationMatrix, alpha, blendMode, true);
                }

                quadBatchID = compileObject(filter->compile(object), quadBatches, quadBatchID,
                                            transformationMatrix, alpha, blendMode);

                if (filter->mode() == FragmentFilterMode::BELOW)
                {
                    quadBatchID = compileObject(object, quadBatches, quadBatchID,
                        transformationMatrix, alpha, blendMode, true);
                }
            }
            else if (container)
            {
                int numChildren = container->numChildren;
                Matrix* childMatrix = new Matrix();

                for (i=0; i<numChildren; ++i)
                {
                    DisplayObject* child = container->getChildAt(i);
                    if (child->hasVisibleArea())
                    {
                        std::string childBlendMode = child->blendMode() == BlendMode()->AUTO ?
                                                    blendMode : child->blendMode;
                        childMatrix->copyFrom(transformationMatrix);
                        RenderSupport::transformMatrixForObject(childMatrix, child);
                        quadBatchID = compileObject(child, quadBatches, quadBatchID, childMatrix,
                                                    alpha*objectAlpha, childBlendMode);
                    }
                }
            }
            else if (quad || batch)
            {
                Texture* texture;
                std::string smoothing;
                bool tinted;
                int numQuads;

                if (quad)
                {
                    Image* image = dynamic_cast<Image*>(quad);
                    texture = image ? image->texture : NULL;
                    smoothing = image ? image->smoothing : NULL;
                    tinted = quad->tinted;
                    numQuads = 1;
                }
                else
                {
                    texture = batch->mTexture;
                    smoothing = batch->mSmoothing;
                    tinted = batch->mTinted;
                    numQuads = batch->mNumQuads;
                }

                quadBatch = quadBatches[quadBatchID];

                if (quadBatch->isStateChange(tinted, alpha*objectAlpha, texture,
                                            smoothing, blendMode, numQuads))
                {
                    quadBatchID++;
                    if (quadBatches.size() <= quadBatchID) quadBatches.push_back(new QuadBatch());
                    quadBatch = quadBatches[quadBatchID];
                    quadBatch->reset();
                }

                if (quad)
                    quadBatch->addQuad(quad, alpha, texture, smoothing, transformationMatrix, blendMode);
                else
                    quadBatch->addQuadBatch(batch, alpha, transformationMatrix, blendMode);
            }
            else
            {
                throw new Error("Unsupported display object: " + getQualifiedClassName(object));
            }

            if (isRootObject)
            {
                // remove unused batches
                for (i=quadBatches.size()-1; i>quadBatchID; --i)
                    quadBatches.pop()->dispose();
            }

            return quadBatchID;
        }

        // properties

        int QuadBatch::numQuads()     { return mNumQuads; }
        bool QuadBatch::tinted()         { return mTinted; }
        starling::textures::Texture* QuadBatch::texture()                           { return mTexture; }
        std::string QuadBatch::smoothing()        { return mSmoothing; }
        bool QuadBatch::premultipliedAlpha()         { return mVertexData->premultipliedAlpha(); }

        int QuadBatch::capacity()     { return mVertexData->numVertices() / 4; }

        // program management

        void QuadBatch::registerPrograms()
        {
            Starling* target = Starling::current;
            if (target->hasProgram(QUAD_PROGRAM_NAME)) return; // already registered

            AGALMiniAssembler* assembler = new AGALMiniAssembler();
            std::string vertexProgramCode;
            std::string fragmentProgramCode;

            // this is the input data we'll pass to the shaders:
            // 
            // va0 -> position
            // va1 -> color
            // va2 -> texCoords
            // vc0 -> alpha
            // vc1 -> mvpMatrix
            // fs0 -> texture

            // Quad:

            vertexProgramCode =
                "m44 op, va0, vc1 \\n" + // 4x4 matrix transform to output clipspace
                "mul v0, va1, vc0 \\n";  // multiply alpha (vc0) with color (va1)

            fragmentProgramCode =
                "mov oc, v0       \\n";  // output color

            target->registerProgram(QUAD_PROGRAM_NAME,
                assembler->assemble(Context3DProgramType::VERTEX, vertexProgramCode),
                assembler->assemble(Context3DProgramType::FRAGMENT, fragmentProgramCode));

            // Image:
            // Each combination of tinted/repeat/mipmap/smoothing has its own fragment shader.

            for (std::vector<bool>::iterator tinted = [.begin(); tinted != [.end(); ++tinted)
            {
                vertexProgramCode = tinted ?
                    "m44 op, va0, vc1 \\n" + // 4x4 matrix transform to output clipspace
                    "mul v0, va1, vc0 \\n" + // multiply alpha (vc0) with color (va1)
                    "mov v1, va2      \\n"   // pass texture coordinates to fragment program
                  :
                    "m44 op, va0, vc1 \\n" + // 4x4 matrix transform to output clipspace
                    "mov v1, va2      \\n";  // pass texture coordinates to fragment program

                fragmentProgramCode = tinted ?
                    "tex ft1,  v1, fs0 <???> \\n" + // sample texture 0
                    "mul  oc, ft1,  v0       \\n"   // multiply color with texel color
                  :
                    "tex  oc,  v1, fs0 <???> \\n";  // sample texture 0

                std::vector<void*> smoothingTypes = [
                    TextureSmoothing::NONE,
                    TextureSmoothing::BILINEAR,
                    TextureSmoothing::TRILINEAR
                ];

                std::vector<void*> formats = [
                    Context3DTextureFormat::BGRA,
                    Context3DTextureFormat::COMPRESSED,
                    "compressedAlpha"
                ];

                for (std::vector<bool>::iterator repeat = [.begin(); repeat != [.end(); ++repeat)
                {
                    for (std::vector<bool>::iterator mipmap = [.begin(); mipmap != [.end(); ++mipmap)
                    {
                        for (std::vector<std::string>::iterator smoothing = smoothingTypes.begin(); smoothing != smoothingTypes.end(); ++smoothing)
                        {
                            for (std::vector<std::string>::iterator format = formats.begin(); format != formats.end(); ++format)
                            {
                                std::vector<void*> options = ["2d", repeat ? "repeat" : "clamp"];

                                if (format == Context3DTextureFormat::COMPRESSED)
                                    options.push_back("dxt1");
                                else if (format == "compressedAlpha")
                                    options.push_back("dxt5");

                                if (smoothing == TextureSmoothing::NONE)
                                    options.push_back("nearest", mipmap ? "mipnearest" : "mipnone");
                                else if (smoothing == TextureSmoothing::BILINEAR)
                                    options.push_back("linear", mipmap ? "mipnearest" : "mipnone");
                                else
                                    options.push_back("linear", mipmap ? "miplinear" : "mipnone");

                                target->registerProgram(
                                    getImageProgramName(tinted, mipmap, repeat, format, smoothing),
                                    assembler->assemble(Context3DProgramType::VERTEX, vertexProgramCode),
                                    assembler->assemble(Context3DProgramType::FRAGMENT,
                                        fragmentProgramCode.replace("???", options.join()))
                                );
                            }
                        }
                    }
                }
            }
        }                             // use explicit string for compatibility

        std::string QuadBatch::getImageProgramName(bool tinted, bool mipMap,
                                                    bool repeat, std::string format,
                                                    std::string smoothing)
        {
            unsigned int bitField = 0;

            if (tinted) bitField |= 1;
            if (mipMap) bitField |= 1 << 1;
            if (repeat) bitField |= 1 << 2;

            if (smoothing == TextureSmoothing::NONE)
                bitField |= 1 << 3;
            else if (smoothing == TextureSmoothing::TRILINEAR)
                bitField |= 1 << 4;

            if (format == Context3DTextureFormat::COMPRESSED)
                bitField |= 1 << 5;
            else if (format == "compressedAlpha")
                bitField |= 1 << 6;

            std::string name = sProgramNameCache[bitField];

            if (name == NULL)
            {
                name = "QB_i." + bitField->toString(16);
                sProgramNameCache[bitField] = name;
            }

            return name;
        }
}
}

