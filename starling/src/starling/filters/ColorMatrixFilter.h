#if !defined(__STARLING_SRC_STARLING_FILTERS_COLORMATRIXFILTER_AS)
#define __STARLING_SRC_STARLING_FILTERS_COLORMATRIXFILTER_AS
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

// Most of the color transformation math was taken from the excellent ColorMatrix class by
// Mario Klingemann: http://www.quasimondo.com/archives/000565.php -- THANKS!!!



#include "flex11.6.h"
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

namespace starling
{
    namespace filters
    {
        class ColorMatrixFilter : public FragmentFilter
        {
        private:
            Program3D *mShaderProgram;

        private:
            std::vector<float> mUserMatrix;   // offset in range 0-255
        private:
            std::vector<float> mShaderMatrix; // offset in range 0-1, changed order

        private:
            static const std::vector<float> MIN_COLOR;
        private:
            static const std::vector<void *> IDENTITY;
        private:
            static const float LUMA_R;
        private:
            static const float LUMA_G;
        private:
            static const float LUMA_B;

            /** helper objects */
        private:
            static std::vector<float> sTmpMatrix1;
        private:
            static std::vector<float> sTmpMatrix2;

            /** Creates a new ColorMatrixFilter instance with the specified matrix.
             *  @param matrix: a vector of 20 items arranged as a 4x5 matrix.
             */
        public:
            ColorMatrixFilter(std::vector<float> matrix=std::vector<float>());

            /** @inheritDoc */
        public:
            virtual void     dispose();

            /** @private */
        protected:
            virtual void     createPrograms();

            /** @private */                               // multiply with alpha again (PMA)
        protected:
            virtual void     activate(int pass, Context3D *context, Texture *texture);

            // color manipulation

            /** Inverts the colors of the filtered objects. */
        public:
            void     invert();

            /** Changes the saturation. Typical values are in the range (-1, 1).
             *  Values above zero will raise, values below zero will reduce the saturation.
             *  '-1' will produce a grayscale image. */
        public:
            void     adjustSaturation(float sat);

            /** Changes the contrast. Typical values are in the range (-1, 1).
             *  Values above zero will raise, values below zero will reduce the contrast. */
        public:
            void     adjustContrast(float value);

            /** Changes the brightness. Typical values are in the range (-1, 1).
             *  Values above zero will make the image brighter, values below zero will make it darker.*/
        public:
            void     adjustBrightness(float value);

            /** Changes the hue of the image. Typical values are in the range (-1, 1). */
        public:
            void     adjustHue(float value);

            // matrix manipulation

            /** Changes the filter matrix back to the identity matrix. */
        public:
            void     reset();

            /** Concatenates the current matrix with another one. */
        public:
            void     concat(std::vector<float> matrix);

            /** Concatenates the current matrix with another one, passing its contents directly. */
        private:
            void     concatValues(float m0, float m1, float m2, float m3, float m4,
                                  float m5, float m6, float m7, float m8, float m9,
                                  float m10, float m11, float m12, float m13, float m14,
                                  float m15, float m16, float m17, float m18, float m19);

        private:
            void     copyMatrix(std::vector<float> from, std::vector<float> to);

        private:
            void     updateShaderMatrix();

            // properties

            /** A vector of 20 items arranged as a 4x5 matrix. */
        public:
            std::vector<float> matrix();
        public:
            void         matrix(std::vector<float> value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_FILTERS_COLORMATRIXFILTER_AS
#endif // __cplusplus

