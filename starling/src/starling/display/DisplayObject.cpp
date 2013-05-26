// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DisplayObject.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"
#include "flash/ui/Mouse.h"
#include "flash/ui/MouseCursor.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/RenderSupport.h"
#include "starling/errors/AbstractClassError.h"
#include "starling/errors/AbstractMethodError.h"
#include "starling/events/EventDispatcher.h"
#include "starling/events/TouchEvent.h"
#include "starling/filters/FragmentFilter.h"
#include "starling/utils/MatrixUtil.h"

#include "starling/display/BlendMode.h"
#include "starling/display/DisplayObjectContainer.h"
#include "starling/display/Stage.h"

    /** Dispatched when an object is added to a parent. */

using namespace flash::geom;
using namespace flash::system;
using namespace flash::ui;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::utils;

namespace starling {
namespace display {


        // members

                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    

        /** Helper objects. */
        std::vector<DisplayObject*> DisplayObject::sAncestors=std::vector<void*>()                   ;
        Rectangle* DisplayObject::sHelperRect = new Rectangle();
        Matrix* DisplayObject::sHelperMatrix  = new Matrix();

        /** @private */
        DisplayObject::DisplayObject()
        {
            if (Capabilities::isDebugger() &&
                getQualifiedClassName(this) == "starling.display::DisplayObject")
            {
                throw new AbstractClassError();
            }

            mX = mY = mPivotX = mPivotY = mRotation = mSkewX = mSkewY = 0.0;
            mScaleX = mScaleY = mAlpha = 1.0;
            mVisible = mTouchable = true;
            mBlendMode = BlendMode::AUTO;
            mTransformationMatrix = new Matrix();
            mOrientationChanged = mUseHandCursor = false;
        }

        /** Disposes all resources of the display object. 
          * GPU buffers are released, event listeners are removed, filters are disposed. */
        void DisplayObject::dispose()
        {
            if (mFilter) mFilter->dispose();
            removeEventListeners();
        }

        /** Removes the object from its parent, if it has one. */
        void DisplayObject::removeFromParent(bool dispose)
        {
            if (mParent) mParent->removeChild(this, dispose);
        }

        /** Creates a matrix that represents the transformation from the local coordinate system 
         *  to another. If you pass a 'resultMatrix', the result will be stored in this matrix
         *  instead of creating a new object. */
        Matrix* DisplayObject::getTransformationMatrix(DisplayObject* targetSpace,
                                                Matrix* resultMatrix)
        {
            DisplayObject* commonParent;
            DisplayObject* currentObject;

            if (resultMatrix) resultMatrix->identity();
            else resultMatrix = new Matrix();

            if (targetSpace == this)
            {
                return resultMatrix;
            }
            else if (targetSpace == mParent || (targetSpace == NULL && mParent == NULL))
            {
                resultMatrix->copyFrom(transformationMatrix);
                return resultMatrix;
            }
            else if (targetSpace == NULL || targetSpace == base)
            {
                // targetCoordinateSpace 'null' represents the target space of the base object.
                // -> move up from this to base

                currentObject = this;
                while (currentObject != targetSpace)
                {
                    resultMatrix->concat(currentObject->transformationMatrix());
                    currentObject = currentObject->mParent;
                }

                return resultMatrix;
            }
            else if (targetSpace->mParent == this)
            {
                targetSpace->getTransformationMatrix(this, resultMatrix);
                resultMatrix->invert();

                return resultMatrix;
            }

            // 1. find a common parent of this and the target space

            commonParent = NULL;
            currentObject = this;

            while (currentObject)
            {
                sAncestors.push_back(currentObject);
                currentObject = currentObject->mParent;
            }

            currentObject = targetSpace;
            while (currentObject && sAncestors.indexOf(currentObject) == -1)
                currentObject = currentObject->mParent;

            sAncestors.clear()    ;

            if (currentObject) commonParent = currentObject;
            else throw new ArgumentError("Object not connected to target");

            // 2. move up from this to common parent

            currentObject = this;
            while (currentObject != commonParent)
            {
                resultMatrix->concat(currentObject->transformationMatrix());
                currentObject = currentObject->mParent;
            }

            if (commonParent == targetSpace)
                return resultMatrix;

            // 3. now move up from target until we reach the common parent

            sHelperMatrix->identity();
            currentObject = targetSpace;
            while (currentObject != commonParent)
            {
                sHelperMatrix->concat(currentObject->transformationMatrix());
                currentObject = currentObject->mParent;
            }

            // 4. now combine the two matrices

            sHelperMatrix->invert();
            resultMatrix->concat(sHelperMatrix);

            return resultMatrix;
        }

        /** Returns a rectangle that completely encloses the object as it appears in another 
         *  coordinate system. If you pass a 'resultRectangle', the result will be stored in this 
         *  rectangle instead of creating a new object. */// optimization
        Rectangle* DisplayObject::getBounds(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            throw new AbstractMethodError("Method needs to be implemented in subclass");
            return NULL;
        }

        /** Returns the object that is found topmost beneath a point in local coordinates, or nil if 
         *  the test fails. If "forTouch" is true, untouchable and invisible objects will cause
         *  the test to fail. */
        DisplayObject* DisplayObject::hitTest(Point* localPoint, bool forTouch)
        {
            // on a touch test, invisible or untouchable objects cause the test to fail
            if (forTouch && (!mVisible || !mTouchable)) return NULL;

            // otherwise, check bounding box
            if (getBounds(this, sHelperRect)->containsPoint(localPoint)) return this;
            else return NULL;
        }

        /** Transforms a point from the local coordinate system to global (stage) coordinates.
         *  If you pass a 'resultPoint', the result will be stored in this point instead of 
         *  creating a new object. */
        Point* DisplayObject::localToGlobal(Point* localPoint, Point* resultPoint)
        {
            getTransformationMatrix(base, sHelperMatrix);
            return MatrixUtil::transformCoords(sHelperMatrix, localPoint->x(), localPoint->y(), resultPoint);
        }

        /** Transforms a point from global (stage) coordinates to the local coordinate system.
         *  If you pass a 'resultPoint', the result will be stored in this point instead of 
         *  creating a new object. */
        Point* DisplayObject::globalToLocal(Point* globalPoint, Point* resultPoint)
        {
            getTransformationMatrix(base, sHelperMatrix);
            sHelperMatrix->invert();
            return MatrixUtil::transformCoords(sHelperMatrix, globalPoint->x(), globalPoint->y(), resultPoint);
        }

        /** Renders the display object with the help of a support object. Never call this method
         *  directly, except from within another render method.
         *  @param support Provides utility functions for rendering.
         *  @param parentAlpha The accumulated alpha value from the object's parent up to the stage. */
        void DisplayObject::render(RenderSupport* support, float parentAlpha)
        {
            throw new AbstractMethodError("Method needs to be implemented in subclass");
        }

        /** Indicates if an object occupies any visible area. (Which is the case when its 'alpha', 
         *  'scaleX' and 'scaleY' values are not zero, and its 'visible' property is enabled.) */
        bool DisplayObject::hasVisibleArea()
        {
            return mAlpha != 0.0 && mVisible && mScaleX != 0.0 && mScaleY != 0.0;
        }

        // internal methods

        /** @private */
        void DisplayObject::setParent(DisplayObjectContainer* value)
        {
            // check for a recursion
            DisplayObject* ancestor = value;
            while (ancestor != this && ancestor != NULL)
                ancestor = ancestor->mParent;

            if (ancestor == this)
                throw new ArgumentError("An object cannot be added as a child to itself or one " +
                                        "of its children (or children's children, etc.)");
            else
                mParent = value;
        }

        // helpers

        bool DisplayObject::isEquivalent(float a, float b, float epsilon)
        {
            return (a - epsilon < b) && (a + epsilon > b);
        }

        float DisplayObject::normalizeAngle(float angle)
        {
            // move into range [-180 deg, +180 deg]
            while (angle < -Math::PI) angle += Math::PI() * 2.0;
            while (angle >  Math::PI) angle -= Math::PI() * 2.0;
            return angle;
        }

        // properties

        /** The transformation matrix of the object relative to its parent.
         * 
         *  <p>If you assign a custom transformation matrix, Starling will try to figure out  
         *  suitable values for <code>x, y, scaleX, scaleY,</code> and <code>rotation</code>.
         *  However, if the matrix was created in a different way, this might not be possible. 
         *  In that case, Starling will apply the matrix, but not update the corresponding 
         *  properties.</p>
         * 
         *  @returns CAUTION: not a copy, but the actual object! */
        Matrix* DisplayObject::transformationMatrix()
        {
            if (mOrientationChanged)
            {
                mOrientationChanged = false;

                if (mSkewX == 0.0 && mSkewY == 0.0)
                {
                    // optimization: no skewing / rotation simplifies the matrix math

                    if (mRotation == 0.0)
                    {
                        mTransformationMatrix->setTo(mScaleX, 0.0, 0.0, mScaleY,
                            mX - mPivotX * mScaleX, mY - mPivotY * mScaleY);
                    }
                    else
                    {
                        float cos = Math::cos(mRotation);
                        float sin = Math::sin(mRotation);
                        float a   = mScaleX *  cos;
                        float b   = mScaleX *  sin;
                        float c   = mScaleY * -sin;
                        float d   = mScaleY *  cos;
                        float tx  = mX - mPivotX * a - mPivotY * c;
                        float ty  = mY - mPivotX * b - mPivotY * d;

                        mTransformationMatrix->setTo(a, b, c, d, tx, ty);
                    }
                }
                else
                {
                    mTransformationMatrix->identity();
                    mTransformationMatrix->scale(mScaleX, mScaleY);
                    MatrixUtil::skew(mTransformationMatrix, mSkewX, mSkewY);
                    mTransformationMatrix->rotate(mRotation);
                    mTransformationMatrix->translate(mX, mY);

                    if (mPivotX != 0.0 || mPivotY != 0.0)
                    {
                        // prepend pivot transformation
                        mTransformationMatrix->tx ( mX - mTransformationMatrix->a * mPivotX
                                                      - mTransformationMatrix->c() * mPivotY);
                        mTransformationMatrix->ty ( mY - mTransformationMatrix->b * mPivotX
                                                      - mTransformationMatrix->d() * mPivotY);
                    }
                }
            }

            return mTransformationMatrix;
        }

        void DisplayObject::transformationMatrix(Matrix* matrix)
        {
            mOrientationChanged = false;
            mTransformationMatrix->copyFrom(matrix);
            mPivotX = mPivotY = 0;

            mX = matrix->tx();
            mY = matrix->ty();
            mScaleX = Math::sqrt(matrix->a() * matrix->a() + matrix->b() * matrix->b());
            mSkewY  = Math::acos(matrix->a() / mScaleX);

            if (!isEquivalent(matrix->b(), mScaleX * Math::sin(mSkewY)))
            {
                mScaleX *= -1;
                mSkewY = Math::acos(matrix->a() / mScaleX);
            }

            mScaleY = Math::sqrt(matrix->c() * matrix->c() + matrix->d() * matrix->d());
            mSkewX  = Math::acos(matrix->d() / mScaleY);

            if (!isEquivalent(matrix->c(), -mScaleY * Math::sin(mSkewX)))
            {
                mScaleY *= -1;
                mSkewX = Math::acos(matrix->d() / mScaleY);
            }

            if (isEquivalent(mSkewX, mSkewY))
            {
                mRotation = mSkewX;
                mSkewX = mSkewY = 0;
            }
            else
            {
                mRotation = 0;
            }
        }

        /** Indicates if the mouse cursor should transform into a hand while it's over the sprite. 
         *  @default false */
        bool DisplayObject::useHandCursor()         { return mUseHandCursor; }
        void DisplayObject::useHandCursor(bool value)
        {
            if (value == mUseHandCursor) return;
            mUseHandCursor = value;

            if (mUseHandCursor)
                addEventListener(TouchEvent::TOUCH, onTouch);
            else
                removeEventListener(TouchEvent::TOUCH, onTouch);
        }

        void DisplayObject::onTouch(starling::events::TouchEvent* event)
        {
            Mouse::cursor ( event->interactsWith(this) ? MouseCursor::BUTTON : MouseCursor::AUTO);
        }

        /** The bounds of the object relative to the local coordinates of the parent. */
        Rectangle* DisplayObject::bounds()
        {
            return getBounds(mParent);
        }

        /** The width of the object in pixels. */
        float DisplayObject::width()        { return getBounds(mParent, sHelperRect)->width(); }
        void DisplayObject::width(float value)
        {
            // this method calls 'this.scaleX' instead of changing mScaleX directly.
            // that way, subclasses reacting on size changes need to override only the scaleX method.

            scaleX = 1.0;
            float actualWidth = width;
            if (actualWidth != 0.0) scaleX = value / actualWidth;
        }

        /** The height of the object in pixels. */
        float DisplayObject::height()        { return getBounds(mParent, sHelperRect)->height(); }
        void DisplayObject::height(float value)
        {
            scaleY = 1.0;
            float actualHeight = height;
            if (actualHeight != 0.0) scaleY = value / actualHeight;
        }

        /** The x coordinate of the object relative to the local coordinates of the parent. */
        float DisplayObject::x()        { return mX; }
        void DisplayObject::x(float value)
        {
            if (mX != value)
            {
                mX = value;
                mOrientationChanged = true;
            }
        }

        /** The y coordinate of the object relative to the local coordinates of the parent. */
        float DisplayObject::y()        { return mY; }
        void DisplayObject::y(float value)
        {
            if (mY != value)
            {
                mY = value;
                mOrientationChanged = true;
            }
        }

        /** The x coordinate of the object's origin in its own coordinate space (default: 0). */
        float DisplayObject::pivotX()        { return mPivotX; }
        void DisplayObject::pivotX(float value)
        {
            if (mPivotX != value)
            {
                mPivotX = value;
                mOrientationChanged = true;
            }
        }

        /** The y coordinate of the object's origin in its own coordinate space (default: 0). */
        float DisplayObject::pivotY()        { return mPivotY; }
        void DisplayObject::pivotY(float value)
        {
            if (mPivotY != value)
            {
                mPivotY = value;
                mOrientationChanged = true;
            }
        }

        /** The horizontal scale factor. '1' means no scale, negative values flip the object. */
        float DisplayObject::scaleX()        { return mScaleX; }
        void DisplayObject::scaleX(float value)
        {
            if (mScaleX != value)
            {
                mScaleX = value;
                mOrientationChanged = true;
            }
        }

        /** The vertical scale factor. '1' means no scale, negative values flip the object. */
        float DisplayObject::scaleY()        { return mScaleY; }
        void DisplayObject::scaleY(float value)
        {
            if (mScaleY != value)
            {
                mScaleY = value;
                mOrientationChanged = true;
            }
        }

        /** The horizontal skew angle in radians. */
        float DisplayObject::skewX()        { return mSkewX; }
        void DisplayObject::skewX(float value)
        {
            value = normalizeAngle(value);

            if (mSkewX != value)
            {
                mSkewX = value;
                mOrientationChanged = true;
            }
        }

        /** The vertical skew angle in radians. */
        float DisplayObject::skewY()        { return mSkewY; }
        void DisplayObject::skewY(float value)
        {
            value = normalizeAngle(value);

            if (mSkewY != value)
            {
                mSkewY = value;
                mOrientationChanged = true;
            }
        }

        /** The rotation of the object in radians. (In Starling, all angles are measured 
         *  in radians.) */
        float DisplayObject::rotation()        { return mRotation; }
        void DisplayObject::rotation(float value)
        {
            value = normalizeAngle(value);

            if (mRotation != value)
            {
                mRotation = value;
                mOrientationChanged = true;
            }
        }

        /** The opacity of the object. 0 = transparent, 1 = opaque. */
        float DisplayObject::alpha()        { return mAlpha; }
        void DisplayObject::alpha(float value)
        {
            mAlpha = value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
        }

        /** The visibility of the object. An invisible object will be untouchable. */
        bool DisplayObject::visible()         { return mVisible; }
        void DisplayObject::visible(bool value)      { mVisible = value; }

        /** Indicates if this object (and its children) will receive touch events. */
        bool DisplayObject::touchable()         { return mTouchable; }
        void DisplayObject::touchable(bool value)      { mTouchable = value; }

        /** The blend mode determines how the object is blended with the objects underneath. 
         *   @default auto
         *   @see starling.display.BlendMode */
        std::string DisplayObject::blendMode()        { return mBlendMode; }
        void DisplayObject::blendMode(std::string value)      { mBlendMode = value; }

        /** The name of the display object (default: null). Used by 'getChildByName()' of 
         *  display object containers. */
        std::string DisplayObject::name()        { return mName; }
        void DisplayObject::name(std::string value)      { mName = value; }

        /** The filter that is attached to the display object. The starling.filters 
         *  package contains several classes that define specific filters you can use. 
         *  Beware that you should NOT use the same filter on more than one object (for 
         *  performance reasons). */
        FragmentFilter* DisplayObject::filter()                { return mFilter; }
        void DisplayObject::filter(FragmentFilter* value)      { mFilter = value; }

        /** The display object container that contains this display object. */
        DisplayObjectContainer* DisplayObject::parent()                        { return mParent; }

        /** The topmost object in the display tree the object is part of. */
        DisplayObject* DisplayObject::base()
        {
            DisplayObject* currentObject = this;
            while (currentObject->mParent) currentObject = currentObject->mParent;
            return currentObject;
        }

        /** The root object the display object is connected to (i.e. an instance of the class 
         *  that was passed to the Starling constructor), or null if the object is not connected
         *  to the stage. */
        DisplayObject* DisplayObject::root()
        {
            DisplayObject* currentObject = this;
            while (currentObject->mParent)
            {
                if (currentObject->dynamic_cast<Stage*>(mParent)) return currentObject;
                else currentObject = currentObject->parent();
            }

            return NULL;
        }

        /** The stage the display object is connected to, or null if it is not connected 
         *  to the stage. */
        Stage* DisplayObject::stage()       { return this->dynamic_cast<Stage*>(base); }
}
}

