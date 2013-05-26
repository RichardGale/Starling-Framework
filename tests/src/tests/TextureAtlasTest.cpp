// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextureAtlasTest.h"
#include "flash/display3D/Context3DTextureFormat.h"
#include "flash/geom/Rectangle.h"

#include "flexunit/framework/Assert.h"

#include "starling/textures/ConcreteTexture.h"
#include "starling/textures/SubTexture.h"
#include "starling/textures/Texture.h"
#include "starling/textures/TextureAtlas.h"

using namespace flash::display3D;
using namespace flash::geom;
using namespace flexunit::framework;
using namespace starling::textures;

namespace tests {



        void TextureAtlasTest::                testXmlParsing()
        {
            std::string format = Context3DTextureFormat::BGRA;
            XML* xml =
                <TextureAtlas>
                    <SubTexture name='ann' x='0'   y='0'  width='55.5' height='16' />
                    <SubTexture name='bob' x='16'  y='32' width='16'   height='32' />
                </TextureAtlas>;

            Texture* texture = new ConcreteTexture(NULL, format, 64, 64, false, false);
            TextureAtlas* atlas = new TextureAtlas(texture, xml);

            Texture* ann = atlas->getTexture("ann");
            Texture* bob = atlas->getTexture("bob");

            Assert::assertTrue(dynamic_cast<SubTexture*>(ann));
            Assert::assertTrue(dynamic_cast<SubTexture*>(bob));

            Assert::assertEquals(55.5, ann->width());
            Assert::assertEquals(16, ann->height());
            Assert::assertEquals(16, bob->width());
            Assert::assertEquals(32, bob->height());

            SubTexture* annST = dynamic_cast<SubTexture*>(ann);
            SubTexture* bobST = dynamic_cast<SubTexture*>(bob);

            Assert::assertEquals(0, annST->clipping()->x());
            Assert::assertEquals(0, annST->clipping()->y());
            Assert::assertEquals(0.25, bobST->clipping()->x());
            Assert::assertEquals(0.5, bobST->clipping()->y());
        }


        void TextureAtlasTest::                testManualCreation()
        {
            std::string format = Context3DTextureFormat::BGRA;
            Texture* texture = new ConcreteTexture(NULL, format, 64, 64, false, false);
            TextureAtlas* atlas = new TextureAtlas(texture);

            atlas->addRegion("ann", new Rectangle(0, 0, 55.5, 16));
            atlas->addRegion("bob", new Rectangle(16, 32, 16, 32));

            Assert::assertNotNull(atlas->getTexture("ann"));
            Assert::assertNotNull(atlas->getTexture("bob"));
            Assert::assertNull(atlas->getTexture("carl"));

            atlas->removeRegion("carl"); // should not blow up
            atlas->removeRegion("bob");

            Assert::assertNull(atlas->getTexture("bob"));
        }


        void TextureAtlasTest::                testGetTextures()
        {
            std::string format = Context3DTextureFormat::BGRA;
            Texture* texture = new ConcreteTexture(NULL, format, 64, 64, false, false);
            TextureAtlas* atlas = new TextureAtlas(texture);

            Assert::assertEquals(texture, atlas->texture);

            atlas->addRegion("ann", new Rectangle(0, 0, 8, 8));
            atlas->addRegion("prefix_3", new Rectangle(8, 0, 3, 8));
            atlas->addRegion("prefix_1", new Rectangle(16, 0, 1, 8));
            atlas->addRegion("bob", new Rectangle(24, 0, 8, 8));
            atlas->addRegion("prefix_2", new Rectangle(32, 0, 2, 8));

            std::vector<Texture*> textures=atlas->getTextures("prefix_");

            Assert::assertEquals(3, textures.size());
            Assert::assertEquals(1, textures[0]->width());
            Assert::assertEquals(2, textures[1]->width());
            Assert::assertEquals(3, textures[2]->width());
        }
}

