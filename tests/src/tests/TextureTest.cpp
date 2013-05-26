// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextureTest.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/textures/ConcreteTexture.h"
#include "starling/textures/SubTexture.h"
#include "starling/textures/Texture.h"
#include "starling/utils/VertexData.h"

using namespace flash::geom;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::textures;
using namespace starling::utils;

namespace tests {


        const float TextureTest::E = 0.0001;


        void TextureTest::                testCreateTexture()
        {
            new Texture();
        }


        void TextureTest::                testTextureCoordinates()
        {
            int rootWidth = 256;
            int rootHeight = 128;
            SubTexture* subTexture;
            SubTexture* subSubTexture;
            VertexData* vertexData = createStandardVertexData();
            VertexData* adjustedVertexData;
            ConcreteTexture* texture = new ConcreteTexture(NULL, NULL, rootWidth, rootHeight, false, false);
            Point* texCoords = new Point();

            // test subtexture filling the whole base texture
            subTexture = new SubTexture(texture, new Rectangle(0, 0, rootWidth, rootHeight));
            adjustedVertexData = vertexData->clone();
            subTexture->adjustVertexData(adjustedVertexData, 0, 4);
            Helpers()->compareVectors(vertexData->rawData(), adjustedVertexData->rawData());

            // test subtexture with 50% of the size of the base texture
            subTexture = new SubTexture(texture,
                new Rectangle(rootWidth/4, rootHeight/4, rootWidth/2, rootHeight/2));
            adjustedVertexData = vertexData->clone();
            subTexture->adjustVertexData(adjustedVertexData, 0, 4);

            adjustedVertexData->getTexCoords(0, texCoords);
            Helpers()->comparePoints(new Point(0.25, 0.25), texCoords);
            adjustedVertexData->getTexCoords(1, texCoords);
            Helpers()->comparePoints(new Point(0.75, 0.25), texCoords);
            adjustedVertexData->getTexCoords(2, texCoords);
            Helpers()->comparePoints(new Point(0.25, 0.75), texCoords);
            adjustedVertexData->getTexCoords(3, texCoords);
            Helpers()->comparePoints(new Point(0.75, 0.75), texCoords);

            // test subtexture of subtexture
            subSubTexture = new SubTexture(subTexture,
                new Rectangle(subTexture->width()/4, subTexture->height()/4,
                              subTexture->width()/2, subTexture->height()/2));
            adjustedVertexData = vertexData->clone();
            subSubTexture->adjustVertexData(adjustedVertexData, 0, 4);

            adjustedVertexData->getTexCoords(0, texCoords);
            Helpers()->comparePoints(new Point(0.375, 0.375), texCoords);
            adjustedVertexData->getTexCoords(1, texCoords);
            Helpers()->comparePoints(new Point(0.625, 0.375), texCoords);
            adjustedVertexData->getTexCoords(2, texCoords);
            Helpers()->comparePoints(new Point(0.375, 0.625), texCoords);
            adjustedVertexData->getTexCoords(3, texCoords);
            Helpers()->comparePoints(new Point(0.625, 0.625), texCoords);

            // test subtexture over moved texture coords (same effect as above)
            vertexData = createVertexDataWithMovedTexCoords();
            adjustedVertexData = vertexData->clone();
            subTexture->adjustVertexData(adjustedVertexData, 0, 4);

            adjustedVertexData->getTexCoords(0, texCoords);
            Helpers()->comparePoints(new Point(0.375, 0.375), texCoords);
            adjustedVertexData->getTexCoords(1, texCoords);
            Helpers()->comparePoints(new Point(0.625, 0.375), texCoords);
            adjustedVertexData->getTexCoords(2, texCoords);
            Helpers()->comparePoints(new Point(0.375, 0.625), texCoords);
            adjustedVertexData->getTexCoords(3, texCoords);
            Helpers()->comparePoints(new Point(0.625, 0.625), texCoords);
        }

        VertexData* TextureTest::createStandardVertexData()
        {
            VertexData* vertexData = new VertexData(4);
            vertexData->setTexCoords(0, 0.0, 0.0);
            vertexData->setTexCoords(1, 1.0, 0.0);
            vertexData->setTexCoords(2, 0.0, 1.0);
            vertexData->setTexCoords(3, 1.0, 1.0);
            return vertexData;
        }

        VertexData* TextureTest::createVertexDataWithMovedTexCoords()
        {
            VertexData* vertexData = new VertexData(4);
            vertexData->setTexCoords(0, 0.25, 0.25);
            vertexData->setTexCoords(1, 0.75, 0.25);
            vertexData->setTexCoords(2, 0.25, 0.75);
            vertexData->setTexCoords(3, 0.75, 0.75);
            return vertexData;
        }


        void TextureTest::                testGetRoot()
        {
            ConcreteTexture* texture = new ConcreteTexture(NULL, NULL, 32, 32, false, false);
            SubTexture* subTexture = new SubTexture(texture, new Rectangle(0, 0, 16, 16));
            SubTexture* subSubTexture = new SubTexture(texture, new Rectangle(0, 0, 8, 8));

            assertEquals(texture, texture->root());
            assertEquals(texture, subTexture->root());
            assertEquals(texture, subSubTexture->root());
            assertEquals(texture->base(), subSubTexture->base());
        }


        void TextureTest::                testGetSize()
        {
            ConcreteTexture* texture = new ConcreteTexture(NULL, NULL, 32, 16, false, false, false, 2);
            SubTexture* subTexture = new SubTexture(texture, new Rectangle(0, 0, 12, 8));

            assertThat(texture->width(), closeTo(16, E));
            assertThat(texture->height(), closeTo(8, E));
            assertThat(texture->nativeWidth(), closeTo(32, E));
            assertThat(texture->nativeHeight(), closeTo(16, E));

            assertThat(subTexture->width(), closeTo(12, E));
            assertThat(subTexture->height(), closeTo(8, E));
            assertThat(subTexture->nativeWidth(), closeTo(24, E));
            assertThat(subTexture->nativeHeight(), closeTo(16, E));
        }
}

