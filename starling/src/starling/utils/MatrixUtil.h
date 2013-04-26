#if !defined(__STARLING_SRC_STARLING_UTILS_MATRIXUTIL_AS)
#define __STARLING_SRC_STARLING_UTILS_MATRIXUTIL_AS
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
        class Point;
    }
}

namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A utility class containing methods related to the Matrix class. */
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class MatrixUtil
        {
            /** Helper object. */
        private:
            static std::vector<float> *sRawData;

            /** @private */
        public:
            MatrixUtil();

            /** Converts a 2D matrix to a 3D matrix. If you pass a 'resultMatrix',
             *  the result will be stored in this matrix instead of creating a new object. */
        public:
            static Matrix3D *convertTo3D(Matrix *matrix, Matrix3D *resultMatrix);

            /** Uses a matrix to transform 2D coordinates into a different space. If you pass a
             *  'resultPoint', the result will be stored in this point instead of creating a new object.*/
        public:
            static Point   *transformCoords(Matrix *matrix, float x, float y,
                                            Point *resultPoint);

            /** Appends a skew transformation to a matrix (angles in radians). The skew matrix
             *  has the following form:
             *  <pre>
             *  | cos(skewY)  -sin(skewX)  0 |
             *  | sin(skewY)   cos(skewX)  0 |
             *  |     0            0       1 |
             *  </pre>
             */
        public:
            static void     skew(Matrix *matrix, float skewX, float skewY);

            /** Prepends a matrix to 'base' by multiplying it with another matrix. */
        public:
            static void     prependMatrix(Matrix *base, Matrix *prep);

            /** Prepends an incremental translation to a Matrix object. */
        public:
            static void     prependTranslation(Matrix *matrix, float tx, float ty);

            /** Prepends an incremental scale change to a Matrix object. */
        public:
            static void     prependScale(Matrix *matrix, float sx, float sy);

            /** Prepends an incremental rotation to a Matrix object (angle in radians). */
        public:
            static void     prependRotation(Matrix *matrix, float angle);

            /** Prepends a skew transformation to a Matrix object (angles in radians). The skew matrix
             *  has the following form:
             *  <pre>
             *  | cos(skewY)  -sin(skewX)  0 |
             *  | sin(skewY)   cos(skewX)  0 |
             *  |     0            0       1 |
             *  </pre>
             */
        public:
            static void     prependSkew(Matrix *matrix, float skewX, float skewY);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_MATRIXUTIL_AS
#endif // __cplusplus

