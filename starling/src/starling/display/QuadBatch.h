#if !defined(__STARLING_SRC_STARLING_DISPLAY_QUADBATCH_AS)
#define __STARLING_SRC_STARLING_DISPLAY_QUADBATCH_AS
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
        class Context3DTextureFormat;
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
        class VertexBuffer3D;
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
        class Rectangle;
    }
}
namespace flash
{
    namespace utils
    {
        class Dictionary;
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
    namespace filters
    {
        class FragmentFilter;
    }
}
namespace starling
{
    namespace filters
    {
        class FragmentFilterMode;
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
    namespace textures
    {
        class TextureSmoothing;
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
        class VertexData;
    }
}

//use starling_internal        ;

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
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::utils;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::core;
using namespace starling::core;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::filters;
using namespace starling::textures;
using namespace starling::textures;
using namespace starling::utils;
using namespace starling::utils;

namespace starling
{
    namespace display
    {
        class QuadBatch: public DisplayObject
        {
        private:
            static const std::string QUAD_PROGRAM_NAME;

        private:
            int mNumQuads;
        private:
            bool mSyncRequired;

        private:
            bool mTinted;
        private:
            Texture *mTexture;
        private:
            std::string mSmoothing;

        private:
            VertexData *mVertexData;
        private:
            VertexBuffer3D *mVertexBuffer;
        private:
            std::vector<unsigned int> *mIndexData;
        private:
            IndexBuffer3D *mIndexBuffer;

            /** Helper objects. */
        private:
            static Matrix *sHelperMatrix;
        private:
            static std::vector<float> *sRenderAlpha;
        private:
            static Matrix3D *sRenderMatrix;
        private:
            static std::map<void *, void *> sProgramNameCache;

            /** Creates a new QuadBatch instance with empty batch data. */
        public:
            QuadBatch();

            /** Disposes vertex- and index-buffer. */
        public:
            virtual void     dispose();

        private:
            void     onContextCreated(Object *event);

            /** Creates a duplicate of the QuadBatch object. */
        public:
            QuadBatch *clone();

        private:
            void     expand(int newCapacity);

        private:
            void     createBuffers();

            /** Uploads the raw data of all batched quads to the vertex buffer. */
        private:
            void     syncBuffers();

            /** Renders the current batch with custom settings for model-view-projection matrix, alpha
             *  and blend mode. This makes it possible to render batches that are not part of the
             *  display list. */
        public:
            void     renderCustom(Matrix *mvpMatrix, float parentAlpha,
                                  std::string blendMode);

            /** Resets the batch. The vertex- and index-buffers remain their size, so that they
             *  can be reused quickly. */
        public:
            void     reset();

            /** Adds an image to the batch. This method internally calls 'addQuad' with the correct
             *  parameters for 'texture' and 'smoothing'. */
        public:
            void     addImage(Image *image, float parentAlpha, Matrix *modelViewMatrix,
                              std::string blendMode);

            /** Adds a quad to the batch. The first quad determines the state of the batch,
             *  i.e. the values for texture, smoothing and blendmode. When you add additional quads,
             *  make sure they share that state (e.g. with the 'isStageChange' method), or reset
             *  the batch. */
        public:
            void     addQuad(Quad *quad, float parentAlpha, Texture *texture,
                             std::string smoothing, Matrix *modelViewMatrix,
                             std::string blendMode);

            /** Adds another QuadBatch to this batch. Just like the 'addQuad' method, you have to
             *  make sure that you only add batches with an equal state. */
        public:
            void     addQuadBatch(QuadBatch *quadBatch, float parentAlpha,
                                  Matrix *modelViewMatrix, std::string blendMode);

            /** Indicates if specific quads can be added to the batch without causing a state change.
             *  A state change occurs if the quad uses a different base texture, has a different
             *  'tinted', 'smoothing', 'repeat' or 'blendMode' setting, or if the batch is full
             *  (one batch can contain up to 8192 quads). */
        public:
            bool     isStateChange(bool tinted, float parentAlpha, Texture *texture,
                                   std::string smoothing, std::string blendMode, int numQuads);

            // display object methods

            /** @inheritDoc */
        public:
            virtual Rectangle *getBounds(DisplayObject *targetSpace, Rectangle *resultRect);

            /** @inheritDoc */
        public:
            virtual void     render(RenderSupport *support, float parentAlpha);

            // compilation (for flattened sprites)

            /** Analyses an object that is made up exclusively of quads (or other containers)
             *  and creates a vector of QuadBatch objects representing it. This can be
             *  used to render the container very efficiently. The 'flatten'-method of the Sprite
             *  class uses this method internally. */
        public:
            static void     compile(DisplayObject *object,
                                    std::vector<QuadBatch *> *quadBatches);

        private:
            static int      compileObject(DisplayObject *object,
                                          std::vector<QuadBatch *> *quadBatches,
                                          int quadBatchID,
                                          Matrix *transformationMatrix,
                                          float alpha,
                                          std::string blendMode,
                                          bool ignoreCurrentFilter);

            // properties

        public:
            int          numQuads();
        public:
            bool         tinted();
        public:
            Texture     *texture();
        public:
            std::string  smoothing();
        public:
            bool         premultipliedAlpha();

        private:
            int          capacity();

            // program management

        private:
            static void     registerPrograms();                              // use explicit string for compatibility

        private:
            static std::string getImageProgramName(bool tinted, bool mipMap,
                                                   bool repeat, std::string format,
                                                   std::string smoothing);
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_QUADBATCH_AS
#endif // __cplusplus

