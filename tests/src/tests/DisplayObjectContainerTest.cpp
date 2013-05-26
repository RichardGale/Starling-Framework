// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DisplayObjectContainerTest.h"
#include "flash/geom/Rectangle.h"

#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/display/DisplayObject.h"
#include "starling/display/Quad.h"
#include "starling/display/Sprite.h"
#include "starling/display/Stage.h"
#include "starling/events/Event.h"

using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::events;

namespace tests {


        const float DisplayObjectContainerTest::E = 0.0001;

                    
                    
                    
                    
                    
                    


        void DisplayObjectContainerTest::                setUp()
        {
            mAdded = mAddedToStage = mAddedChild =
            mRemoved = mRemovedFromStage = mRemovedChild = 0;
        }


        void DisplayObjectContainerTest::tearDown()      { }


        void DisplayObjectContainerTest::                testChildParentHandling()
        {
            Sprite* parent = new Sprite();
            Sprite* child1 = new Sprite();
            Sprite* child2 = new Sprite();
            DisplayObject* returnValue;

            Assert::assertEquals(0, parent->numChildren());
            Assert::assertNull(child1->parent);

            returnValue = parent->addChild(child1);
            Assert::assertEquals(child1, returnValue);
            Assert::assertEquals(1, parent->numChildren());
            Assert::assertEquals(parent, child1->parent);

            returnValue = parent->addChild(child2);
            Assert::assertEquals(child2, returnValue);
            Assert::assertEquals(2, parent->numChildren());
            Assert::assertEquals(parent, child2->parent);
            Assert::assertEquals(child1, parent->getChildAt(0));
            Assert::assertEquals(child2, parent->getChildAt(1));

            returnValue = parent->removeChild(child1);
            Assert::assertEquals(child1, returnValue);
            Assert::assertNull(child1->parent);
            Assert::assertEquals(child2, parent->getChildAt(0));
            child1->removeFromParent(); // should *not* throw an exception

            returnValue = child2->addChild(child1);
            Assert::assertEquals(child1, returnValue);
            Assert::assertTrue(parent->contains(child1));
            Assert::assertTrue(parent->contains(child2));
            Assert::assertEquals(child2, child1->parent);

            returnValue = parent->addChildAt(child1, 0);
            Assert::assertEquals(child1, returnValue);
            Assert::assertEquals(parent, child1->parent);
            Assert::assertFalse(child2->contains(child1));
            Assert::assertEquals(child1, parent->getChildAt(0));
            Assert::assertEquals(child2, parent->getChildAt(1));

            returnValue = parent->removeChildAt(0);
            Assert::assertEquals(child1, returnValue);
            Assert::assertEquals(child2, parent->getChildAt(0));
            Assert::assertEquals(1, parent->numChildren());
        }


        void DisplayObjectContainerTest::                testRemoveChildren()
        {
            Sprite* parent;
            int numChildren = 10;

            // removing all children

            parent = createSprite(numChildren);
            Assert::assertEquals(10, parent->numChildren);

            parent->removeChildren();
            Assert::assertEquals(0, parent->numChildren);

            // removing a subset

            parent = createSprite(numChildren);
            parent->removeChildren(3, 5);
            Assert::assertEquals(7, parent->numChildren);
            Assert::assertEquals("2", parent->getChildAt(2)->name());
            Assert::assertEquals("6", parent->getChildAt(3)->name());

            // remove beginning from an id

            parent = createSprite(numChildren);
            parent->removeChildren(5);
            Assert::assertEquals(5, parent->numChildren);
            Assert::assertEquals("4", parent->getChildAt(4)->name());Sprite* DisplayObjectContainerTest::(int numChildren)
            {
                Sprite* sprite = new Sprite();
                for (int i=0; i<numChildren; ++i)
                {
                    Sprite* child = new Sprite();
                    child->name ( i->toString());
                    sprite->addChild(child);
                }
                return sprite;
            }
        }


        void DisplayObjectContainerTest::                testGetChildByName()
        {
            Sprite* parent = new Sprite();
            Sprite* child1 = new Sprite();
            Sprite* child2 = new Sprite();
            Sprite* child3 = new Sprite();

            parent->addChild(child1);
            parent->addChild(child2);
            parent->addChild(child3);

            child1->name ( "child1");
            child2->name ( "child2");
            child3->name ( "child3");

            Assert::assertEquals(child1, parent->getChildByName("child1"));
            Assert::assertEquals(child2, parent->getChildByName("child2"));
            Assert::assertEquals(child3, parent->getChildByName("child3"));
            Assert::assertNull(parent->getChildByName("non-existing"));

            child2->name ( "child3");
            Assert::assertEquals(child2, parent->getChildByName("child3"));
        }


        void DisplayObjectContainerTest::                testSetChildIndex()
        {
            Sprite* parent = new Sprite();
            Sprite* childA = new Sprite();
            Sprite* childB = new Sprite();
            Sprite* childC = new Sprite();

            parent->addChild(childA);
            parent->addChild(childB);
            parent->addChild(childC);

            parent->setChildIndex(childB, 0);
            Assert::assertEquals(parent->getChildAt(0), childB);
            Assert::assertEquals(parent->getChildAt(1), childA);
            Assert::assertEquals(parent->getChildAt(2), childC);

            parent->setChildIndex(childB, 1);
            Assert::assertEquals(parent->getChildAt(0), childA);
            Assert::assertEquals(parent->getChildAt(1), childB);
            Assert::assertEquals(parent->getChildAt(2), childC);

            parent->setChildIndex(childB, 2);
            Assert::assertEquals(parent->getChildAt(0), childA);
            Assert::assertEquals(parent->getChildAt(1), childC);
            Assert::assertEquals(parent->getChildAt(2), childB);

            Assert::assertEquals(3, parent->numChildren());
        }


        void DisplayObjectContainerTest::                testSwapChildren()
        {
            Sprite* parent = new Sprite();
            Sprite* childA = new Sprite();
            Sprite* childB = new Sprite();
            Sprite* childC = new Sprite();

            parent->addChild(childA);
            parent->addChild(childB);
            parent->addChild(childC);

            parent->swapChildren(childA, childC);
            Assert::assertEquals(parent->getChildAt(0), childC);
            Assert::assertEquals(parent->getChildAt(1), childB);
            Assert::assertEquals(parent->getChildAt(2), childA);

            parent->swapChildren(childB, childB); // should change nothing
            Assert::assertEquals(parent->getChildAt(0), childC);
            Assert::assertEquals(parent->getChildAt(1), childB);
            Assert::assertEquals(parent->getChildAt(2), childA);

            Assert::assertEquals(3, parent->numChildren());
        }


        void DisplayObjectContainerTest::                testWidthAndHeight()
        {
            Sprite* sprite = new Sprite();

            Quad* quad1 = new Quad(10, 20);
            quad1->x ( -10);
            quad1->y ( -15);

            Quad* quad2 = new Quad(15, 25);
            quad2->x ( 30);
            quad2->y ( 25);

            sprite->addChild(quad1);
            sprite->addChild(quad2);

            assertThat(sprite->width(), closeTo(55, E));
            assertThat(sprite->height(), closeTo(65, E));

            quad1->rotation ( Math::PI() / 2);
            assertThat(sprite->width(), closeTo(75, E));
            assertThat(sprite->height(), closeTo(65, E));

            quad1->rotation ( Math::PI);
            assertThat(sprite->width(), closeTo(65, E));
            assertThat(sprite->height(), closeTo(85, E));
        }


        void DisplayObjectContainerTest::                testBounds()
        {
            Quad* quad = new Quad(10, 20);
            quad->x ( -10);
            quad->y ( 10);
            quad->rotation ( Math::PI() / 2);

            Sprite* sprite = new Sprite();
            sprite->addChild(quad);

            Rectangle* bounds = sprite->bounds;
            assertThat(bounds->x(), closeTo(-30, E));
            assertThat(bounds->y(), closeTo(10, E));
            assertThat(bounds->width(), closeTo(20, E));
            assertThat(bounds->height(), closeTo(10, E));

            bounds = sprite->getBounds(sprite);
            assertThat(bounds->x(), closeTo(-30, E));
            assertThat(bounds->y(), closeTo(10, E));
            assertThat(bounds->width(), closeTo(20, E));
            assertThat(bounds->height(), closeTo(10, E));
        }


        void DisplayObjectContainerTest::                testBoundsInSpace()
        {
            Sprite* root = new Sprite();

            Sprite* spriteA = new Sprite();
            spriteA->x ( 50);
            spriteA->y ( 50);
            addQuadToSprite(spriteA);
            root->addChild(spriteA);

            Sprite* spriteA1 = new Sprite();
            spriteA1->x ( 150);
            spriteA1->y ( 50);
            spriteA1->scaleX ( spriteA1->scaleY ( 0.5));
            addQuadToSprite(spriteA1);
            spriteA->addChild(spriteA1);

            Sprite* spriteA11 = new Sprite();
            spriteA11->x ( 25);
            spriteA11->y ( 50);
            spriteA11->scaleX ( spriteA11->scaleY ( 0.5));
            addQuadToSprite(spriteA11);
            spriteA1->addChild(spriteA11);

            Sprite* spriteA2 = new Sprite();
            spriteA2->x ( 50);
            spriteA2->y ( 150);
            spriteA2->scaleX ( spriteA2->scaleY ( 0.5));
            addQuadToSprite(spriteA2);
            spriteA->addChild(spriteA2);

            Sprite* spriteA21 = new Sprite();
            spriteA21->x ( 50);
            spriteA21->y ( 25);
            spriteA21->scaleX ( spriteA21->scaleY ( 0.5));
            addQuadToSprite(spriteA21);
            spriteA2->addChild(spriteA21);

            // ---

            Rectangle* bounds = spriteA21->getBounds(spriteA11);
            Rectangle* expectedBounds = new Rectangle(-350, 350, 100, 100);
            Helpers()->compareRectangles(bounds, expectedBounds);

            // now rotate as well

            spriteA11->rotation ( Math::PI() / 4.0);
            spriteA21->rotation ( Math::PI() / -4.0);

            bounds = spriteA21->getBounds(spriteA11);
            expectedBounds = new Rectangle(0, 394.974762, 100, 100);
            Helpers()->compareRectangles(bounds, expectedBounds);void DisplayObjectContainerTest::(Sprite* sprite)
            {
                sprite->addChild(new Quad(100, 100));
            }
        }


        void DisplayObjectContainerTest::                testBoundsOfEmptyContainer()
        {
            Sprite* sprite = new Sprite();
            sprite->x ( 100);
            sprite->y ( 200);

            Rectangle* bounds = sprite->bounds;
            assertThat(bounds->x(), closeTo(100, E));
            assertThat(bounds->y(), closeTo(200, E));
            assertThat(bounds->width(), closeTo(0, E));
            assertThat(bounds->height(), closeTo(0, E));
        }


        void DisplayObjectContainerTest::                testSize()
        {
            Quad* quad1 = new Quad(100, 100);
            Quad* quad2 = new Quad(100, 100);
            quad2->x ( quad2->y ( 100));

            Sprite* sprite = new Sprite();
            Sprite* childSprite = new Sprite();

            sprite->addChild(childSprite);
            childSprite->addChild(quad1);
            childSprite->addChild(quad2);

            assertThat(sprite->width(), closeTo(200, E));
            assertThat(sprite->height(), closeTo(200, E));

            sprite->scaleX ( 2.0);
            sprite->scaleY ( 2.0);

            assertThat(sprite->width(), closeTo(400, E));
            assertThat(sprite->height(), closeTo(400, E));
        }


        void DisplayObjectContainerTest::                testSort()
        {
            Sprite* s1 = new Sprite(); s1->y ( 8);
            Sprite* s2 = new Sprite(); s2->y ( 3);
            Sprite* s3 = new Sprite(); s3->y ( 6);
            Sprite* s4 = new Sprite(); s4->y ( 1);

            Sprite* parent = new Sprite();
            parent->addChild(s1);
            parent->addChild(s2);
            parent->addChild(s3);
            parent->addChild(s4);

            Assert::assertEquals(s1, parent->getChildAt(0));
            Assert::assertEquals(s2, parent->getChildAt(1));
            Assert::assertEquals(s3, parent->getChildAt(2));
            Assert::assertEquals(s4, parent->getChildAt(3));

            parent->sortChildren(function(DisplayObject* child1, DisplayObject* child2)int
            {
                if (child1->y() < child2->y()) return -1;
                else if (child1->y() > child2->y()) return 1;
                else return 0;
            });

            Assert::assertEquals(s4, parent->getChildAt(0));
            Assert::assertEquals(s2, parent->getChildAt(1));
            Assert::assertEquals(s3, parent->getChildAt(2));
            Assert::assertEquals(s1, parent->getChildAt(3));
        }


        void DisplayObjectContainerTest::                testAddExistingChild()
        {
            Sprite* sprite = new Sprite();
            Quad* quad = new Quad(100, 100);
            sprite->addChild(quad);
            sprite->addChild(quad);
            Assert::assertEquals(1, sprite->numChildren());
            Assert::assertEquals(0, sprite->getChildIndex(quad));
        }


        void DisplayObjectContainerTest::                testRemoveWithEventHandler()
        {
            Sprite* parent = new Sprite();
            Sprite* child0 = new Sprite();
            Sprite* child1 = new Sprite();
            Sprite* child2 = new Sprite();

            parent->addChild(child0);
            parent->addChild(child1);
            parent->addChild(child2);

            // Remove last child, and in its event listener remove first child.
            // That must work, even though the child changes its index in the event handler.

            child2->addEventListener(Event::REMOVED, function()void
            {
                child0->removeFromParent();
            });

            Helpers()->assertDoesNotThrow(function()void
            {
                parent->removeChildAt(2);
            });

            Assert::assertNull(child2->parent);
            Assert::assertNull(child0->parent);
            Assert::assertEquals(child1, parent->getChildAt(0));
            Assert::assertEquals(1, parent->numChildren());
        }


        void DisplayObjectContainerTest::                testIllegalRecursion()
        {
            Sprite* sprite1 = new Sprite();
            Sprite* sprite2 = new Sprite();
            Sprite* sprite3 = new Sprite();

            sprite1->addChild(sprite2);
            sprite2->addChild(sprite3);

            // this should throw an error
            sprite3->addChild(sprite1);
        }


        void DisplayObjectContainerTest::                testAddAsChildToSelf()
        {
            Sprite* sprite = new Sprite();
            sprite->addChild(sprite);
        }


        void DisplayObjectContainerTest::                testDisplayListEvents()
        {
            Stage* stage = new Stage(100, 100);
            Sprite* sprite = new Sprite();
            Quad* quad = new Quad(20, 20);

            quad->addEventListener(Event::ADDED, onAdded);
            quad->addEventListener(Event::ADDED_TO_STAGE, onAddedToStage);
            quad->addEventListener(Event::REMOVED, onRemoved);
            quad->addEventListener(Event::REMOVED_FROM_STAGE, onRemovedFromStage);

            stage->addEventListener(Event::ADDED, onAddedChild);
            stage->addEventListener(Event::REMOVED, onRemovedChild);

            sprite->addChild(quad);
            Assert::assertEquals(1, mAdded);
            Assert::assertEquals(0, mRemoved);
            Assert::assertEquals(0, mAddedToStage);
            Assert::assertEquals(0, mRemovedFromStage);
            Assert::assertEquals(0, mAddedChild);
            Assert::assertEquals(0, mRemovedChild);

            stage->addChild(sprite);
            Assert::assertEquals(1, mAdded);
            Assert::assertEquals(0, mRemoved);
            Assert::assertEquals(1, mAddedToStage);
            Assert::assertEquals(0, mRemovedFromStage);
            Assert::assertEquals(1, mAddedChild);
            Assert::assertEquals(0, mRemovedChild);

            stage->removeChild(sprite);
            Assert::assertEquals(1, mAdded);
            Assert::assertEquals(0, mRemoved);
            Assert::assertEquals(1, mAddedToStage);
            Assert::assertEquals(1, mRemovedFromStage);
            Assert::assertEquals(1, mAddedChild);
            Assert::assertEquals(1, mRemovedChild);

            sprite->removeChild(quad);
            Assert::assertEquals(1, mAdded);
            Assert::assertEquals(1, mRemoved);
            Assert::assertEquals(1, mAddedToStage);
            Assert::assertEquals(1, mRemovedFromStage);
            Assert::assertEquals(1, mAddedChild);
            Assert::assertEquals(1, mRemovedChild);
        }


        void DisplayObjectContainerTest::                testRemovedFromStage()
        {
            Stage* stage = new Stage(100, 100);
            Sprite* sprite = new Sprite();
            stage->addChild(sprite);
            sprite->addEventListener(Event::REMOVED_FROM_STAGE, onSpriteRemovedFromStage);
            sprite->removeFromParent();
            Assert::assertEquals(1, mRemovedFromStage);void DisplayObjectContainerTest::(Event* e)
            {
                // stage should still be accessible in event listener
                Assert::assertNotNull(sprite->stage);
                mRemovedFromStage++;
            }
        }

        void DisplayObjectContainerTest::onAdded(Event* event)      { mAdded++; }
        void DisplayObjectContainerTest::onAddedToStage(Event* event)      { mAddedToStage++; }
        void DisplayObjectContainerTest::onAddedChild(Event* event)      { mAddedChild++; }

        void DisplayObjectContainerTest::onRemoved(Event* event)      { mRemoved++; }
        void DisplayObjectContainerTest::onRemovedFromStage(Event* event)      { mRemovedFromStage++; }
        void DisplayObjectContainerTest::onRemovedChild(Event* event)      { mRemovedChild++; }
}

