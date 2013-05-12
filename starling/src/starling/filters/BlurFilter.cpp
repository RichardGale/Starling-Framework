// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "BlurFilter.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DProgramType.h"
#include "flash/display3D/Program3D.h"

#include "starling/textures/Texture.h"
#include "starling/utils/Color.h"

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


        /** helper object */

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
        BlurFilter::BlurFilter(float blurX, float blurY, float resolution)
        {
            super(1, resolution);
            mBlurX = blurX;
            mBlurY = blurY;
            updateMarginsAndPasses();
        }

        /** Creates a blur filter that is set up for a drop shadow effect. */
        BlurFilter *BlurFilter::createDropShadow(float distance, float angle,
                unsigned int color, float alpha, float blur,
                float resolution)
        {
            BlurFilter *dropShadow=new BlurFilter(blur, blur, resolution);
            dropShadow->offsetX= Math::cos(angle)* distance;
            dropShadow->offsetY= Math::sin(angle)* distance;
            dropShadow->mode= FragmentFilterMode->BELOW;
            dropShadow->setUniformColor(true,color, alpha);
            return dropShadow;
        }

        /** Creates a blur filter that is set up for a glow effect. */
        BlurFilter *BlurFilter::createGlow(unsigned int color, float alpha, float blur,
                                           float resolution)
        {
            BlurFilter *glow=new BlurFilter(blur, blur, resolution);
            glow->mode= FragmentFilterMode->BELOW;
            glow->setUniformColor(true,color, alpha);
            return glow;
        }

        /** @inheritDoc */
        void BlurFilter::dispose()
        {
            if (mNormalProgram) mNormalProgram->dispose();
            if (mTintedProgram) mTintedProgram->dispose();

            super->dispose();
        }

        /** @private */
        void BlurFilter::createPrograms()
        {
            mNormalProgram = createProgram(false);
            mTintedProgram = createProgram(true);
        }

        Program3D *BlurFilter::createProgram(bool tinted)
        {
            // vc0-3 - mvp matrix
            // vc4   - kernel offset
            // va0   - position
            // va1   - texture coords

            std::string vertexProgramCode=
                "m44 op, va0, vc0       \n" + // 4x4 matrix transform to output space
                "mov v0, va1            \n" + // pos:  0 |
                "sub v1, va1, vc4.zwxx  \n" + // pos: -2 |
                "sub v2, va1, vc4.xyxx  \n" + // pos: -1 | --> kernel positions
                "add v3, va1, vc4.xyxx  \n" + // pos: +1 |     (only 1st two parts are relevant)
                "add v4, va1, vc4.zwxx  \n";  // pos: +2 |

            // v0-v4 - kernel position
            // fs0   - input texture
            // fc0   - weight data
            // fc1   - color (optional)
            // ft0-4 - pixel color from texture
            // ft5   - output color

            std::string fragmentProgramCode=
                "tex ft0,  v0, fs0 <2d, clamp, linear, mipnone> \n" +  // read center pixel
                "mul ft5, ft0, fc0.xxxx                         \n" +  // multiply with center weight

                "tex ft1,  v1, fs0 <2d, clamp, linear, mipnone> \n" +  // read pixel -2
                "mul ft1, ft1, fc0.zzzz                         \n" +  // multiply with weight
                "add ft5, ft5, ft1                              \n" +  // add to output color

                "tex ft2,  v2, fs0 <2d, clamp, linear, mipnone> \n" +  // read pixel -1
                "mul ft2, ft2, fc0.yyyy                         \n" +  // multiply with weight
                "add ft5, ft5, ft2                              \n" +  // add to output color

                "tex ft3,  v3, fs0 <2d, clamp, linear, mipnone> \n" +  // read pixel +1
                "mul ft3, ft3, fc0.yyyy                         \n" +  // multiply with weight
                "add ft5, ft5, ft3                              \n" +  // add to output color

                "tex ft4,  v4, fs0 <2d, clamp, linear, mipnone> \n" +  // read pixel +2
                "mul ft4, ft4, fc0.zzzz                         \n";   // multiply with weight

            if (tinted) fragmentProgramCode +=
                    "add ft5, ft5, ft4                              \n" + // add to output color
                    "mul ft5.xyz, fc1.xyz, ft5.www                  \n" +
                    "mul oc, ft5, fc1.wwww                          \n";  // multiply alpha

            else fragmentProgramCode +=
                    "add  oc, ft5, ft4                              \n";   // add to output color

            return assembleAgal(fragmentProgramCode, vertexProgramCode);
        }

        /** @private */                                               // set rgb with correct alpha
        void BlurFilter::activate(int pass, Context3D *context, Texture *texture)
        {
            // already set by super class:
            //
            // vertex constants 0-3: mvpMatrix (3D)
            // vertex attribute 0:   vertex position (FLOAT_2)
            // vertex attribute 1:   texture coordinates (FLOAT_2)
            // texture 0:            input texture

            updateParameters(pass, texture->nativeWidth,texture->nativeHeight);

            context->setProgramConstantsFromVector(Context3DProgramType::VERTEX, 4, mOffsets);
            context->setProgramConstantsFromVector(Context3DProgramType::FRAGMENT,0,mWeights);

            if (mUniformColor && pass == numPasses - 1)
            {
                context->setProgramConstantsFromVector(Context3DProgramType::FRAGMENT,1,mColor);
                context->setProgram(mTintedProgram);
            }
            else
            {
                context->setProgram(mNormalProgram);
            }
        }

        void BlurFilter::updateParameters(int pass, int textureWidth, int textureHeight)
        {
            // algorithm described here:
            // http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
            //
            // To run in constrained mode, we can only make 5 texture lookups in the fragment
            // shader. By making use of linear texture sampling, we can produce similar output
            // to what would be 9 lookups.

            float sigma;
            bool horizontal   = pass < mBlurX;
            float pixelSize;

            if (horizontal)
            {
                sigma = Math::min(1.0,mBlurX - pass) * MAX_SIGMA;
                pixelSize = 1.0 / textureWidth;
            }
            else
            {
                sigma = Math::min(1.0,mBlurY - (pass - Math::ceil(mBlurX)))* MAX_SIGMA;
                pixelSize = 1.0 / textureHeight;
            }

            const float twoSigmaSq = 2 * sigma * sigma;
            const float multiplier = 1.0 / Math::sqrt(twoSigmaSq* Math::PI);

            // get weights on the exact pixels (sTmpWeights) and calculate sums (mWeights)

            for ( int i=0; i<5; ++i)
                sTmpWeights[i] = multiplier * Math::exp(-i*i/ twoSigmaSq);

            mWeights[0] = sTmpWeights[0];
            mWeights[1] = sTmpWeights[1] + sTmpWeights[2];
            mWeights[2] = sTmpWeights[3] + sTmpWeights[4];

            // normalize weights so that sum equals "1.0"

            float weightSum = mWeights[0] + 2*mWeights[1] + 2*mWeights[2];
            float invWeightSum = 1.0 / weightSum;

            mWeights[0] *= invWeightSum;
            mWeights[1] *= invWeightSum;
            mWeights[2] *= invWeightSum;

            // calculate intermediate offsets

            float offset1 = (  pixelSize * sTmpWeights[1] + 2*pixelSize * sTmpWeights[2]) / mWeights[1];
            float offset2 = (3*pixelSize * sTmpWeights[3] + 4*pixelSize * sTmpWeights[4]) / mWeights[2];

            // depending on pass, we move in x- or y-direction

            if (horizontal)
            {
                mOffsets[0] = offset1;
                mOffsets[1] = 0;
                mOffsets[2] = offset2;
                mOffsets[3] = 0;
            }
            else
            {
                mOffsets[0] = 0;
                mOffsets[1] = offset1;
                mOffsets[2] = 0;
                mOffsets[3] = offset2;
            }
        }

        void BlurFilter::updateMarginsAndPasses()
        {
            if (mBlurX == 0 && mBlurY == 0) mBlurX = 0.001;

            numPasses = Math::ceil(mBlurX)+ Math::ceil(mBlurY);
            marginX = 4 + Math::ceil(mBlurX);
            marginY = 4 + Math::ceil(mBlurY);
        }

        /** A uniform color will replace the RGB values of the input color, while the alpha
         *  value will be multiplied with the given factor. Pass <code>false</code> as the
         *  first parameter to deactivate the uniform color. */
        void BlurFilter::setUniformColor(bool enable, unsigned int color, float alpha)
        {
            mColor[0] = Color::getRed(color)  / 255.0;
            mColor[1] = Color::getGreen(color)/ 255.0;
            mColor[2] = Color::getBlue(color) / 255.0;
            mColor[3] = alpha;
            mUniformColor = enable;
        }

        /** The blur factor in x-direction (stage coordinates).
         *  The number of required passes will be <code>Math.ceil(value)</code>. */
        float BlurFilter::blurX()
        {
            return mBlurX;
        }
        void BlurFilter::blurX(float value)
        {
            mBlurX = value;
            updateMarginsAndPasses();
        }

        /** The blur factor in y-direction (stage coordinates).
         *  The number of required passes will be <code>Math.ceil(value)</code>. */
        float BlurFilter::blurY()
        {
            return mBlurY;
        }
        void BlurFilter::blurY(float value)
        {
            mBlurY = value;
            updateMarginsAndPasses();
        }
    }
}

