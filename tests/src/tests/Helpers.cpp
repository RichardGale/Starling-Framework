// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Helpers.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"
#include "flash/geom/Vector3D.h"

#include "org/flexunit/Assert.h"
#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/hamcrest/number/closeTo.h"

using namespace flash::geom;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;

namespace tests {


        void Helpers::compareRectangles(Rectangle* rect1, Rectangle* rect2,
                                                 float e)
        {
            assertThat(rect1->x(), closeTo(rect2->x(), e));
            assertThat(rect1->y(), closeTo(rect2->y(), e));
            assertThat(rect1->width(), closeTo(rect2->width(), e));
            assertThat(rect1->height(), closeTo(rect2->height(), e));
        }

        void Helpers::comparePoints(Point* point1, Point* point2, float e)
        {
            assertThat(point1->x(), closeTo(point2->x(), e));
            assertThat(point1->y(), closeTo(point2->y(), e));
        }

        void Helpers::compareVector3Ds(Vector3D* v1, Vector3D* v2, float e)
        {
            assertThat(v1->x(), closeTo(v2->x(), e));
            assertThat(v1->y(), closeTo(v2->y(), e));
            assertThat(v1->z(), closeTo(v2->z(), e));
            assertThat(v1->w(), closeTo(v2->w(), e));
        }

        void Helpers::compareVectors(std::vector<float> vector1, std::vector<float> vector2,
                                              float e)
        {
            assertEquals(vector1.size(), vector2.size());

            for (int i=0; i<vector1.size(); ++i)
                assertThat(vector1[i], closeTo(vector2[i], e));
        }

        void Helpers::compareMatrices(Matrix* matrix1, Matrix* matrix2, float e)
        {
            assertThat(matrix1->a(),  closeTo(matrix2->a(),  e));
            assertThat(matrix1->b(),  closeTo(matrix2->b(),  e));
            assertThat(matrix1->c(),  closeTo(matrix2->c(),  e));
            assertThat(matrix1->d(),  closeTo(matrix2->d(),  e));
            assertThat(matrix1->tx(), closeTo(matrix2->tx(), e));
            assertThat(matrix1->ty(), closeTo(matrix2->ty(), e));
        }

        void Helpers::assertDoesNotThrow(Function* block)
        {
            try
            {
                block();
            }
            catch (e:Error*)
            {
                Assert::fail("Error thrown: " + e()->message());
            }
        }
}

