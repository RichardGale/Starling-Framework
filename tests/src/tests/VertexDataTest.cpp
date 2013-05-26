// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "VertexDataTest.h"
#include "flash/geom/Point.h"
#include "flash/geom/Vector3D.h"

#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/utils/VertexData.h"

using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::hamcrest::number;
using namespace starling::utils;

namespace tests {


        const float VertexDataTest::E = 0.001;


        void VertexDataTest::                testInit()
        {
            int numVertices = 3;
            VertexData* vd = new VertexData(numVertices);
            Point* position = new Point();
            Point* texCoords = new Point();

            for (int i=0; i<numVertices; ++i)
            {
                vd->getPosition(i, position);
                vd->getTexCoords(i, texCoords);

                Helpers()->comparePoints(position, new Point());
                Helpers()->comparePoints(texCoords, new Point());
                Assert::assertEquals(0x0, vd->getColor(i));
                Assert::assertEquals(1.0, vd->getAlpha(i));
            }
        }


        void VertexDataTest::                testGetNumVertices()
        {
            VertexData* vd = new VertexData(4);
            Assert::assertEquals(4, vd->numVertices());
        }


        void VertexDataTest::                testBoundsLow()
        {
            VertexData* vd = new VertexData(3);
            vd->getColor(-1);
        }


        void VertexDataTest::                testBoundsHigh()
        {
            VertexData* vd = new VertexData(3);
            vd->getColor(3);
        }


        void VertexDataTest::                testPosition()
        {
            VertexData* vd = new VertexData(4);
            vd->setPosition(0, 1, 2);
            vd->setPosition(1, 4, 5);

            Point* position = new Point();

            vd->getPosition(0, position);
            Assert::assertEquals(1, position->x());
            Assert::assertEquals(2, position->y());

            vd->getPosition(1, position);
            Assert::assertEquals(4, position->x());
            Assert::assertEquals(5, position->y());
        }


        void VertexDataTest::                testColor()
        {
            VertexData* vd = new VertexData(3, true);
            Assert::assertEquals(3, vd->numVertices());
            Assert::assertTrue(vd->premultipliedAlpha());

            vd->setColor(0, 0xffaabb);
            vd->setColor(1, 0x112233);

            Assert::assertEquals(0xffaabb, vd->getColor(0));
            Assert::assertEquals(0x112233, vd->getColor(1));
            Assert::assertEquals(1.0, vd->getAlpha(0));

            // check premultiplied alpha

            float alpha = 0.5;

            vd->setColor(2, 0x445566);
            vd->setAlpha(2, alpha);
            Assert::assertEquals(0x445566, vd->getColor(2));
            Assert::assertEquals(1.0, vd->getAlpha(1));
            Assert::assertEquals(alpha, vd->getAlpha(2));

            std::vector<float> data=vd->rawData();
            float red   = 0x44 / 255.0;
            float green = 0x55 / 255.0;
            float blue  = 0x66 / 255.0;
            int offset = VertexData::ELEMENTS_PER_VERTEX * 2 + VertexData::COLOR_OFFSET;

            assertThat(data[offset  ], closeTo(red * alpha, E));
            assertThat(data[offset+1], closeTo(green * alpha, E));
            assertThat(data[offset+2], closeTo(blue * alpha, E));

            // changing the pma setting should update contents

            vd->setPremultipliedAlpha(false, true);
            Assert::assertFalse(vd->premultipliedAlpha());

            Assert::assertEquals(0xffaabb, vd->getColor(0));
            Assert::assertEquals(0x112233, vd->getColor(1));
            Assert::assertEquals(1.0, vd->getAlpha(0));

            vd->setColor(2, 0x445566);
            vd->setAlpha(2, 0.5);
            Assert::assertEquals(0x445566, vd->getColor(2));
            Assert::assertEquals(0.5, vd->getAlpha(2));

            assertThat(data[offset  ], closeTo(red, E));
            assertThat(data[offset+1], closeTo(green, E));
            assertThat(data[offset+2], closeTo(blue, E));
        }


        void VertexDataTest::                testTexCoords()
        {
            VertexData* vd = new VertexData(2);
            vd->setTexCoords(0, 0.25, 0.75);
            vd->setTexCoords(1, 0.33, 0.66);

            Point* texCoords = new Point();

            vd->getTexCoords(0, texCoords);
            Assert::assertEquals(0.25, texCoords->x());
            Assert::assertEquals(0.75, texCoords->y());

            vd->getTexCoords(1, texCoords);
            Assert::assertEquals(0.33, texCoords->x());
            Assert::assertEquals(0.66, texCoords->y());
        }
}

