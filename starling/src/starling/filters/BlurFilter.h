#if !defined(__STARLING_SRC_STARLING_FILTERS_BLURFILTER_AS)
#define __STARLING_SRC_STARLING_FILTERS_BLURFILTER_AS
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

/** The BlurFilter applies a Gaussian blur to an object. The strength of the blur can be
 *  set for x- and y-axis separately (always relative to the stage).
 *  A blur filter can also be set up as a drop shadow or glow filter. Use the respective
 *  static methods to create such a filter.
 */
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace filters
    {
        class BlurFilter: public FragmentFilter
        {
        private:
            const float MAX_SIGMA;

        private:
            Program3D *mNormalProgram;
        private:
            Program3D *mTintedProgram;

        private:
            std::vector<float> *mOffsets;
        private:
            std::vector<float> *mWeights;
        private:
            std::vector<float> *mColor;

        private:
            float mBlurX;
        private:
            float mBlurY;
        private:
            bool mUniformColor;

            /** helper object */
        private:
            std::vector<float> *sTmpWeights;

            /** Create a new BlurFilter. For each blur direction, the number of required passes is
             *  <code>Math.ceil(blur)</code>.
             *
             *  <ul><li>blur = 0.5: 1 pass</li>
             *      <li>blur = 1.0: 1 pass</li>
             *      <li>blur = 1.5: 2 passes</li>
             *      <li>blur = 2.0: 2 passes</li>
             *      <li>etc.</li>
             *  </ul>
             *
             *  <p>Instead of raising the number of passes, you should consider lowering the resolution.
             *  A lower resolution will result in a blurrier image, while reducing the rendering
             *  cost.</p>
             */
        public:
            BlurFilter(float blurX, float blurY, float resolution);

            /** Creates a blur filter that is set up for a drop shadow effect. */
        public:
            static BlurFilter *createDropShadow(float distance, float angle,
                                                unsigned int color, float alpha, float blur,
                                                float resolution);

            /** Creates a blur filter that is set up for a glow effect. */
        public:
            static BlurFilter *createGlow(unsigned int color, float alpha, float blur,
                                          float resolution);

            /** @inheritDoc */
        public:
            virtual void     dispose();

            /** @private */
        protected:
            virtual void     createPrograms();

        private:
            Program3D *createProgram(bool tinted);

            /** @private */                                               // set rgb with correct alpha
        protected:
            virtual void     activate(int pass, Context3D *context, Texture *texture);

        private:
            void     updateParameters(int pass, int textureWidth, int textureHeight);

        private:
            void     updateMarginsAndPasses();

            /** A uniform color will replace the RGB values of the input color, while the alpha
             *  value will be multiplied with the given factor. Pass <code>false</code> as the
             *  first parameter to deactivate the uniform color. */
        public:
            void     setUniformColor(bool enable, unsigned int color, float alpha);

            /** The blur factor in x-direction (stage coordinates).
             *  The number of required passes will be <code>Math.ceil(value)</code>. */
        public:
            float        blurX();
        public:
            void         blurX(float value);

            /** The blur factor in y-direction (stage coordinates).
             *  The number of required passes will be <code>Math.ceil(value)</code>. */
        public:
            float        blurY();
        public:
            void         blurY(float value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_FILTERS_BLURFILTER_AS
#endif // __cplusplus

