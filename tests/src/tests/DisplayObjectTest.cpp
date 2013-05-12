// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DisplayObjectTest.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/display/Quad.h"
#include "starling/display/Sprite.h"
#include "starling/display/Stage.h"
#include "starling/utils/deg2rad.h"

using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::utils;

namespace tests
{


    const float DisplayObjectTest::E=0.0001;


    void DisplayObjectTest::                testBase()
    {
        Sprite *object1=new Sprite();
        Sprite *object2=new Sprite();
        Sprite *object3=new Sprite();

        object1->addChild(object2);
        object2->addChild(object3);

        Assert::assertEquals(object1,object1->base);
        Assert::assertEquals(object1,object2->base);
        Assert::assertEquals(object1,object3->base);

        Quad *quad=new Quad(100, 100);
        Assert::assertEquals(quad,quad->base);
    }


    void DisplayObjectTest::                testRootAndStage()
    {
        Sprite *object1=new Sprite();
        Sprite *object2=new Sprite();
        Sprite *object3=new Sprite();

        object1->addChild(object2);
        object2->addChild(object3);

        Assert::assertEquals(NULL,object1->root);
        Assert::assertEquals(NULL,object2->root);
        Assert::assertEquals(NULL,object3->root);
        Assert::assertEquals(NULL,object1->stage);
        Assert::assertEquals(NULL,object2->stage);
        Assert::assertEquals(NULL,object3->stage);

        Stage *stage=new Stage(100, 100);
        stage->addChild(object1);

        Assert::assertEquals(object1,object1->root);
        Assert::assertEquals(object1,object2->root);
        Assert::assertEquals(object1,object3->root);
        Assert::assertEquals(stage,object1->stage);
        Assert::assertEquals(stage,object2->stage);
        Assert::assertEquals(stage,object3->stage);
    }


    void DisplayObjectTest::                testGetTransformationMatrix()
    {
        Sprite *sprite=new Sprite();
        Sprite *child=new Sprite();
        child->x= 30;
        child->y= 20;
        child->scaleX= 1.2;
        child->scaleY= 1.5;
        child->rotation= Math::PI/ 4.0;
        sprite->addChild(child);

        Matrix *matrix=sprite->getTransformationMatrix(child);
        Matrix *expectedMatrix=child->transformationMatrix;
        expectedMatrix->invert();
        Helpers->compareMatrices(expectedMatrix,matrix);

        matrix = child->getTransformationMatrix(sprite);
        Helpers->compareMatrices(child->transformationMatrix,matrix);

        // more is tested indirectly via 'testBoundsInSpace' in DisplayObjectContainerTest
    }


    void DisplayObjectTest::                testSetTransformationMatrix()
    {
        Sprite *sprite=new Sprite();
        Matrix *matrix=new Matrix();
        matrix->scale(1.5,2.0);
        matrix->rotate(0.25);
        matrix->translate(10,20);
        sprite->transformationMatrix= matrix;

        assertThat(sprite->scaleX,closeTo(1.5, E));
        assertThat(sprite->scaleY,closeTo(2.0, E));
        assertThat(sprite->rotation,closeTo(0.25, E));
        assertThat(sprite->x,closeTo(10, E));
        assertThat(sprite->y,closeTo(20, E));

        Helpers->compareMatrices(matrix,sprite->transformationMatrix);
    }


    void DisplayObjectTest::                testSetTransformationMatrixWithPivot()
    {
        // pivot point information is redundant; instead, x/y properties will be modified.

        Sprite *sprite=new Sprite();
        sprite->pivotX= 50;
        sprite->pivotY= 20;

        Matrix *matrix=sprite->transformationMatrix;
        sprite->transformationMatrix= matrix;

        assertThat(sprite->x,closeTo(-50, E));
        assertThat(sprite->y,closeTo(-20, E));
        assertThat(sprite->pivotX,closeTo(0.0, E));
        assertThat(sprite->pivotY,closeTo(0.0, E));
    }


    void DisplayObjectTest::                testBounds()
    {
        Quad *quad=new Quad(10, 20);
        quad->x= -10;
        quad->y=  10;
        quad->rotation= Math::PI/ 2;

        Rectangle *bounds=quad->bounds;
        assertThat(bounds->x,closeTo(-30, E));
        assertThat(bounds->y,closeTo(10, E));
        assertThat(bounds->width,closeTo(20, E));
        assertThat(bounds->height,closeTo(10, E));

        bounds = quad->getBounds(quad);
        assertThat(bounds->x,closeTo(0, E));
        assertThat(bounds->y,closeTo(0, E));
        assertThat(bounds->width,closeTo(10, E));
        assertThat(bounds->height,closeTo(20, E));
    }


    void DisplayObjectTest::                testZeroSize()
    {
        Sprite *sprite=new Sprite();
        assertEquals(1.0, sprite->scaleX);
        assertEquals(1.0, sprite->scaleY);

        // sprite is empty, scaling should thus have no effect!
        sprite->width= 100;
        sprite->height= 200;
        assertEquals(1.0, sprite->scaleX);
        assertEquals(1.0, sprite->scaleY);
        assertEquals(0.0, sprite->width);
        assertEquals(0.0, sprite->height);

        // setting a value to zero should be no problem -- and the original size
        // should be remembered.
        Quad *quad=new Quad(100, 200);
        quad->scaleX= 0.0;
        quad->scaleY= 0.0;
        assertThat(quad->width,closeTo(0, E));
        assertThat(quad->height,closeTo(0, E));

        quad->scaleX= 1.0;
        quad->scaleY= 1.0;
        assertThat(quad->width,closeTo(100, E));
        assertThat(quad->height,closeTo(200, E));
    }


    void DisplayObjectTest::                testLocalToGlobal()
    {
        Sprite *root=new Sprite();
        Sprite *sprite=new Sprite();
        sprite->x= 10;
        sprite->y= 20;
        root->addChild(sprite);
        Sprite *sprite2=new Sprite();
        sprite2->x= 150;
        sprite2->y= 200;
        sprite->addChild(sprite2);

        Point *localPoint=new Point(0, 0);
        Point *globalPoint=sprite2->localToGlobal(localPoint);
        Point *expectedPoint=new Point(160, 220);
        Helpers->comparePoints(expectedPoint,globalPoint);

        // the position of the root object should be irrelevant -- we want the coordinates
        // *within* the root coordinate system!
        root->x= 50;
        globalPoint = sprite2->localToGlobal(localPoint);
        Helpers->comparePoints(expectedPoint,globalPoint);
    }


    void DisplayObjectTest::                testGlobalToLocal()
    {
        Sprite *root=new Sprite();
        Sprite *sprite=new Sprite();
        sprite->x= 10;
        sprite->y= 20;
        root->addChild(sprite);
        Sprite *sprite2=new Sprite();
        sprite2->x= 150;
        sprite2->y= 200;
        sprite->addChild(sprite2);

        Point *globalPoint=new Point(160, 220);
        Point *localPoint=sprite2->globalToLocal(globalPoint);
        Point *expectedPoint=new Point();
        Helpers->comparePoints(expectedPoint,localPoint);

        // the position of the root object should be irrelevant -- we want the coordinates
        // *within* the root coordinate system!
        root->x= 50;
        localPoint = sprite2->globalToLocal(globalPoint);
        Helpers->comparePoints(expectedPoint,localPoint);
    }


    void DisplayObjectTest::                testHitTestPoint()
    {
        Quad *quad=new Quad(25, 10);
        Assert::assertNotNull(quad->hitTest(newPoint(15,5), true));
        Assert::assertNotNull(quad->hitTest(newPoint(0,0), true));
        Assert::assertNotNull(quad->hitTest(newPoint(24.99,0), true));
        Assert::assertNotNull(quad->hitTest(newPoint(24.99,9.99), true));
        Assert::assertNotNull(quad->hitTest(newPoint(0,9.99), true));
        Assert::assertNull(quad->hitTest(newPoint(-1,-1), true));
        Assert::assertNull(quad->hitTest(newPoint(25.01,10.01), true));

        quad->visible= false;
        Assert::assertNull(quad->hitTest(newPoint(15,5), true));

        quad->visible= true;
        quad->touchable= false;
        Assert::assertNull(quad->hitTest(newPoint(10,5), true));

        quad->visible= false;
        quad->touchable= false;
        Assert::assertNull(quad->hitTest(newPoint(10,5), true));
    }


    void DisplayObjectTest::                testRotation()
    {
        Quad *quad=new Quad(100, 100);
        quad->rotation= deg2rad(400);
        assertThat(quad->rotation,closeTo(deg2rad(40), E));
        quad->rotation= deg2rad(220);
        assertThat(quad->rotation,closeTo(deg2rad(-140), E));
        quad->rotation= deg2rad(180);
        assertThat(quad->rotation,closeTo(deg2rad(180), E));
        quad->rotation= deg2rad(-90);
        assertThat(quad->rotation,closeTo(deg2rad(-90), E));
        quad->rotation= deg2rad(-179);
        assertThat(quad->rotation,closeTo(deg2rad(-179), E));
        quad->rotation= deg2rad(-180);
        assertThat(quad->rotation,closeTo(deg2rad(-180), E));
        quad->rotation= deg2rad(-181);
        assertThat(quad->rotation,closeTo(deg2rad(179), E));
        quad->rotation= deg2rad(-300);
        assertThat(quad->rotation,closeTo(deg2rad(60), E));
        quad->rotation= deg2rad(-370);
        assertThat(quad->rotation,closeTo(deg2rad(-10), E));
    }


    void DisplayObjectTest::                testPivotPoint()
    {
        float width = 100.0;
        float height = 150.0;

        // a quad with a pivot point should behave exactly as a quad without
        // pivot point inside a sprite

        Sprite *sprite=new Sprite();
        Quad *innerQuad=new Quad(width, height);
        sprite->addChild(innerQuad);
        Quad *quad=new Quad(width, height);
        Helpers->compareRectangles(sprite->bounds,quad->bounds);

        innerQuad->x= -50;
        quad->pivotX= 50;
        innerQuad->y= -20;
        quad->pivotY= 20;
        Helpers->compareRectangles(sprite->bounds,quad->bounds);

        sprite->rotation= quad->rotation= deg2rad(45);
        Helpers->compareRectangles(sprite->bounds,quad->bounds);

        sprite->scaleX= quad->scaleX= 1.5;
        sprite->scaleY= quad->scaleY= 0.6;
        Helpers->compareRectangles(sprite->bounds,quad->bounds);

        sprite->x= quad->x= 5;
        sprite->y= quad->y= 20;
        Helpers->compareRectangles(sprite->bounds,quad->bounds);
    }


    void DisplayObjectTest::                testPivotWithSkew()
    {
        int width= 200;
        int height= 100;
        float skewX = 0.2;
        float skewY = 0.35;
        float scaleY = 0.5;
        float rotation = 0.5;

        // create a scaled, rotated and skewed object from a sprite and a quad

        Quad *quad=new Quad(width, height);
        quad->x= width / -2;
        quad->y= height / -2;

        Sprite *sprite=new Sprite();
        sprite->x= width / 2;
        sprite->y= height / 2;
        sprite->skewX= skewX;
        sprite->skewY= skewY;
        sprite->rotation= rotation;
        sprite->scaleY= scaleY;
        sprite->addChild(quad);

        // do the same without a sprite, but with a pivoted quad

        Quad *pQuad=new Quad(width, height);
        pQuad->x= width / 2;
        pQuad->y= height / 2;
        pQuad->pivotX= width / 2;
        pQuad->pivotY= height / 2;
        pQuad->skewX= skewX;
        pQuad->skewY= skewY;
        pQuad->scaleY= scaleY;
        pQuad->rotation= rotation;

        // the bounds have to be the same

        Helpers->compareRectangles(sprite->bounds,pQuad->bounds,1.0);
    }


    void DisplayObjectTest::                testName()
    {
        Sprite *sprite=new Sprite();
        Assert::assertNull(sprite->name);

        sprite->name= "hugo";
        Assert::assertEquals("hugo",sprite->name);
    }
}

