// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "MatrixUtil.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Matrix3D.h"
#include "flash/geom/Point.h"

#include "starling/errors/AbstractClassError.h"

    /** A utility class containing methods related to the Matrix class. */

using namespace flash::geom;
using namespace starling::errors;

namespace starling {
namespace utils {


        /** Helper object. */
         std::vector<float> MatrixUtil::sRawData=
            new <float >[1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1];

        /** @private */
        MatrixUtil::MatrixUtil() { throw new AbstractClassError(); }

        /** Converts a 2D matrix to a 3D matrix. If you pass a 'resultMatrix',  
         *  the result will be stored in this matrix instead of creating a new object. */
        Matrix3D* MatrixUtil::convertTo3D(Matrix* matrix, Matrix3D* resultMatrix)
        {
            if (resultMatrix == NULL) resultMatrix = new Matrix3D();

            sRawData[0] = matrix->a();
            sRawData[1] = matrix->b();
            sRawData[4] = matrix->c();
            sRawData[5] = matrix->d();
            sRawData[12] = matrix->tx();
            sRawData[13] = matrix->ty();

            resultMatrix->copyRawDataFrom(sRawData);
            return resultMatrix;
        }

        /** Uses a matrix to transform 2D coordinates into a different space. If you pass a 
         *  'resultPoint', the result will be stored in this point instead of creating a new object.*/
        Point* MatrixUtil::transformCoords(Matrix* matrix, float x, float y,
                                               Point* resultPoint)
        {
            if (resultPoint == NULL) resultPoint = new Point();

            resultPoint->x ( matrix->a * x + matrix->c * y + matrix->tx());
            resultPoint->y ( matrix->d * y + matrix->b * x + matrix->ty());

            return resultPoint;
        }

        /** Appends a skew transformation to a matrix (angles in radians). The skew matrix
         *  has the following form: 
         *  <pre>
         *  | cos(skewY)  -sin(skewX)  0 |
         *  | sin(skewY)   cos(skewX)  0 |
         *  |     0            0       1 |
         *  </pre> 
         */
        void MatrixUtil::skew(Matrix* matrix, float skewX, float skewY)
        {
             float sinX  = Math::sin(skewX);
             float cosX  = Math::cos(skewX);
             float sinY  = Math::sin(skewY);
             float cosY  = Math::cos(skewY);

            matrix->setTo(matrix->a  * cosY - matrix->b()  * sinX,
                         matrix->a  * sinY + matrix->b()  * cosX,
                         matrix->c  * cosY - matrix->d()  * sinX,
                         matrix->c  * sinY + matrix->d()  * cosX,
                         matrix->tx * cosY - matrix->ty() * sinX,
                         matrix->tx * sinY + matrix->ty() * cosX);
        }

        /** Prepends a matrix to 'base' by multiplying it with another matrix. */
        void MatrixUtil::prependMatrix(Matrix* base, Matrix* prep)
        {
            base->setTo(base->a() * prep->a() + base->c() * prep->b(),
                       base->b() * prep->a() + base->d() * prep->b(),
                       base->a() * prep->c() + base->c() * prep->d(),
                       base->b() * prep->c() + base->d() * prep->d(),
                       base->tx() + base->a() * prep->tx() + base->c() * prep->ty(),
                       base->ty() + base->b() * prep->tx() + base->d() * prep->ty());
        }

        /** Prepends an incremental translation to a Matrix object. */
        void MatrixUtil::prependTranslation(Matrix* matrix, float tx, float ty)
        {
            matrix->tx() += matrix->a * tx + matrix->c() * ty;
            matrix->ty() += matrix->b * tx + matrix->d() * ty;
        }

        /** Prepends an incremental scale change to a Matrix object. */
        void MatrixUtil::prependScale(Matrix* matrix, float sx, float sy)
        {
            matrix->setTo(matrix->a() * sx, matrix->b() * sx,
                         matrix->c() * sy, matrix->d() * sy,
                         matrix->tx(), matrix->ty());
        }

        /** Prepends an incremental rotation to a Matrix object (angle in radians). */
        void MatrixUtil::prependRotation(Matrix* matrix, float angle)
        {
             float sin  = Math::sin(angle);
             float cos  = Math::cos(angle);

            matrix->setTo(matrix->a * cos + matrix->c() * sin,  matrix->b * cos + matrix->d() * sin,
                         matrix->c * cos - matrix->a() * sin,  matrix->d * cos - matrix->b() * sin,
                         matrix->tx(), matrix->ty());
        }

        /** Prepends a skew transformation to a Matrix object (angles in radians). The skew matrix
         *  has the following form: 
         *  <pre>
         *  | cos(skewY)  -sin(skewX)  0 |
         *  | sin(skewY)   cos(skewX)  0 |
         *  |     0            0       1 |
         *  </pre> 
         */
        void MatrixUtil::prependSkew(Matrix* matrix, float skewX, float skewY)
        {
             float sinX  = Math::sin(skewX);
             float cosX  = Math::cos(skewX);
             float sinY  = Math::sin(skewY);
             float cosY  = Math::cos(skewY);

            matrix->setTo(matrix->a * cosY + matrix->c() * sinY,
                         matrix->b * cosY + matrix->d() * sinY,
                         matrix->c * cosX - matrix->a() * sinX,
                         matrix->d * cosX - matrix->b() * sinX,
                         matrix->tx(), matrix->ty());
        }
}
}

