// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================

// Most of the color transformation math was taken from the excellent ColorMatrix class by
// Mario Klingemann: http://www.quasimondo.com/archives/000565.php -- THANKS!!!



#include "ColorMatrixFilter.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DProgramType.h"
#include "flash/display3D/Program3D.h"

#include "starling/textures/Texture.h"

    /** The ColorMatrixFilter class lets you apply a 4x5 matrix transformation on the RGBA color 
     *  and alpha values of every pixel in the input image to produce a result with a new set 
     *  of RGBA color and alpha values. It allows saturation changes, hue rotation, 
     *  luminance to alpha, and various other effects.
     * 
     *  <p>The class contains several convenience methods for frequently used color 
     *  adjustments. All those methods change the current matrix, which means you can easily 
     *  combine them in one filter:</p>
     *  
     *  <listing>
     *  // create an inverted filter with 50% saturation and 180   hue rotation
     *  var filter:ColorMatrixFilter = new ColorMatrixFilter();
     *  filter.invert();
     *  filter.adjustSaturation(-0.5);
     *  filter.adjustHue(1.0);</listing>
     *  
     *  <p>If you want to gradually animate one of the predefined color adjustments, either reset
     *  the matrix after each step, or use an identical adjustment value for each step; the 
     *  changes will add up.</p>
     */

using namespace flash::display3D;
using namespace starling::textures;

namespace starling {
namespace filters {


                    

                                                   // offset in range 0-255
                                                   // offset in range 0-1, changed order

        const std::vector<float> ColorMatrixFilter::MIN_COLOR=new<float >[0, 0, 0, 0.0001];
        const std::vector<void*> ColorMatrixFilter::IDENTITY=[1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0];
        const float ColorMatrixFilter::LUMA_R  = 0.299;
        const float ColorMatrixFilter::LUMA_G  = 0.587;
        const float ColorMatrixFilter::LUMA_B  = 0.114;

        /** helper objects */
         std::vector<float> ColorMatrixFilter::sTmpMatrix1=newstd::vector<float>(20,true);
         std::vector<float> ColorMatrixFilter::sTmpMatrix2=new<float >[];

        /** Creates a new ColorMatrixFilter instance with the specified matrix. 
         *  @param matrix: a vector of 20 items arranged as a 4x5 matrix.   
         */
        ColorMatrixFilter::ColorMatrixFilter(std::vector<float> matrix)
        {
            mUserMatrix.clear();
            mShaderMatrix.clear();

            this()->matrix = matrix;
        }

        /** @inheritDoc */
        void ColorMatrixFilter::dispose()
        {
            if (mShaderProgram) mShaderProgram->dispose();
            super()->dispose();
        }

        /** @private */
        void ColorMatrixFilter::createPrograms()
        {
            // fc0-3: matrix
            // fc4:   offset
            // fc5:   minimal allowed color value

             std::string fragmentProgramCode=
                "tex ft0, v0,  fs0 <2d, clamp, linear, mipnone>  \\n" + // read texture color
                "max ft0, ft0, fc5              \\n" + // avoid division through zero in next step
                "div ft0.xyz, ft0.xyz, ft0.www  \\n" + // restore original (non-PMA) RGB values
                "m44 ft0, ft0, fc0              \\n" + // multiply color with 4x4 matrix
                "add ft0, ft0, fc4              \\n" + // add offset
                "mul ft0.xyz, ft0.xyz, ft0.www  \\n" +
                "mov oc, ft0                    \\n";  // copy to output

            mShaderProgram = assembleAgal(fragmentProgramCode);
        }

        /** @private */                               // multiply with alpha again (PMA)
        void ColorMatrixFilter::activate(int pass, Context3D* context, Texture* texture)
        {
            context->setProgramConstantsFromVector(Context3DProgramType::FRAGMENT, 0, mShaderMatrix);
            context->setProgramConstantsFromVector(Context3DProgramType::FRAGMENT, 5, MIN_COLOR);
            context->setProgram(mShaderProgram);
        }

        // color manipulation

        /** Inverts the colors of the filtered objects. */
        void ColorMatrixFilter::invert()
        {
            concatValues(-1,  0,  0,  0, 255,
                          0, -1,  0,  0, 255,
                          0,  0, -1,  0, 255,
                          0,  0,  0,  1,   0);
        }

        /** Changes the saturation. Typical values are in the range (-1, 1).
         *  Values above zero will raise, values below zero will reduce the saturation.
         *  '-1' will produce a grayscale image. */
        void ColorMatrixFilter::adjustSaturation(float sat)
        {
            sat += 1;

             float invSat   = 1 - sat;
             float invLumR  = invSat * LUMA_R;
             float invLumG  = invSat * LUMA_G;
             float invLumB  = invSat * LUMA_B;

            concatValues((invLumR + sat), invLumG, invLumB, 0, 0,
                         invLumR, (invLumG + sat), invLumB, 0, 0,
                         invLumR, invLumG, (invLumB + sat), 0, 0,
                         0, 0, 0, 1, 0);
        }

        /** Changes the contrast. Typical values are in the range (-1, 1).
         *  Values above zero will raise, values below zero will reduce the contrast. */
        void ColorMatrixFilter::adjustContrast(float value)
        {
             float s  = value + 1;
             float o  = 128 * (1 - s);

            concatValues(s, 0, 0, 0, o,
                         0, s, 0, 0, o,
                         0, 0, s, 0, o,
                         0, 0, 0, 1, 0);
        }

        /** Changes the brightness. Typical values are in the range (-1, 1).
         *  Values above zero will make the image brighter, values below zero will make it darker.*/
        void ColorMatrixFilter::adjustBrightness(float value)
        {
            value *= 255;

            concatValues(1, 0, 0, 0, value,
                         0, 1, 0, 0, value,
                         0, 0, 1, 0, value,
                         0, 0, 0, 1, 0);
        }

        /** Changes the hue of the image. Typical values are in the range (-1, 1). */
        void ColorMatrixFilter::adjustHue(float value)
        {
            value *= Math::PI;

             float cos  = Math::cos(value);
             float sin  = Math::sin(value);

            concatValues(
                ((LUMA_R + (cos * (1 - LUMA_R))) + (sin * -(LUMA_R))), ((LUMA_G + (cos * -(LUMA_G))) + (sin * -(LUMA_G))), ((LUMA_B + (cos * -(LUMA_B))) + (sin * (1 - LUMA_B))), 0, 0,
                ((LUMA_R + (cos * -(LUMA_R))) + (sin * 0.143)), ((LUMA_G + (cos * (1 - LUMA_G))) + (sin * 0.14)), ((LUMA_B + (cos * -(LUMA_B))) + (sin * -0.283)), 0, 0,
                ((LUMA_R + (cos * -(LUMA_R))) + (sin * -((1 - LUMA_R)))), ((LUMA_G + (cos * -(LUMA_G))) + (sin * LUMA_G)), ((LUMA_B + (cos * (1 - LUMA_B))) + (sin * LUMA_B)), 0, 0,
                0, 0, 0, 1, 0);
        }

        // matrix manipulation

        /** Changes the filter matrix back to the identity matrix. */
        void ColorMatrixFilter::reset()
        {
            matrix = NULL;
        }

        /** Concatenates the current matrix with another one. */
        void ColorMatrixFilter::concat(std::vector<float> matrix)
        {
             int i = 0;

            for ( int y=0; y<4; ++y)
            {
                for ( int x=0; x<5; ++x)
                {
                    sTmpMatrix1[int(i+x)] =
                        matrix[i]        * mUserMatrix[x]           +
                        matrix[int(i+1)] * mUserMatrix[int(x +  5)] +
                        matrix[int(i+2)] * mUserMatrix[int(x + 10)] +
                        matrix[int(i+3)] * mUserMatrix[int(x + 15)] +
                        (x == 4 ? matrix[int(i+4)] : 0);
                }

                i+=5;
            }

            copyMatrix(sTmpMatrix1, mUserMatrix);
            updateShaderMatrix();
        }

        /** Concatenates the current matrix with another one, passing its contents directly. */
        void ColorMatrixFilter::concatValues(float m0, float m1, float m2, float m3, float m4,
                                      float m5, float m6, float m7, float m8, float m9,
                                      float m10, float m11, float m12, float m13, float m14,
                                      float m15, float m16, float m17, float m18, float m19)
        {
            sTmpMatrix2.clear()    ;
            sTmpMatrix2.push_back(m0,m1,m2,m3,m4,m5, m6, m7, m8, m9,
                m10, m11, m12, m13, m14, m15, m16, m17, m18, m19);

            concat(sTmpMatrix2);
        }

        void ColorMatrixFilter::copyMatrix(std::vector<float> from, std::vector<float> to)
        {
            for ( int i=0; i<20; ++i)
                to[i] = from[i];
        }

        void ColorMatrixFilter::updateShaderMatrix()
        {
            // the shader needs the matrix components in a different order, 
            // and it needs the offsets in the range 0-1.

            mShaderMatrix.clear()    ;
            mShaderMatrix.push_back(
                mUserMatrix[0],  mUserMatrix[1],  mUserMatrix[2],  mUserMatrix[3],
                mUserMatrix[5],  mUserMatrix[6],  mUserMatrix[7],  mUserMatrix[8],
                mUserMatrix[10], mUserMatrix[11], mUserMatrix[12], mUserMatrix[13],
                mUserMatrix[15], mUserMatrix[16], mUserMatrix[17], mUserMatrix[18],
                mUserMatrix[4] / 255.0,  mUserMatrix[9] / 255.0,  mUserMatrix[14] / 255.0,
                mUserMatrix[19] / 255.0
            );
        }

        // properties

        /** A vector of 20 items arranged as a 4x5 matrix. */
        std::vector<float> ColorMatrixFilter::matrix()                 { return mUserMatrix; }
        void ColorMatrixFilter::matrix(std::vector<float> value)
        {
            if (value && value.size() != 20)
                throw new ArgumentError("Invalid matrix length: must be 20");

            if (value.empty())
            {
                mUserMatrix.clear()    ;
                mUserMatrix.push()->apply(mUserMatrix, IDENTITY);
            }
            else
            {
                copyMatrix(value, mUserMatrix);
            }

            updateShaderMatrix();
        }
}
}

