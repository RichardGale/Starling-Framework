#if !defined(__STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECT_AS)
#define __STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECT_AS
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



#include "flex11.6.h"
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
        class Point;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}
namespace flash
{
    namespace system
    {
        class Capabilities;
    }
}
namespace flash
{
    namespace ui
    {
        class Mouse;
    }
}
namespace flash
{
    namespace ui
    {
        class MouseCursor;
    }
}
namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
    }
}

namespace starling
{
    namespace core
    {
        class RenderSupport;
    }
}
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}
namespace starling
{
    namespace errors
    {
        class AbstractMethodError;
    }
}
#include "starling/events/EventDispatcher.h"
namespace starling
{
    namespace events
    {
        class TouchEvent;
    }
}
namespace starling
{
    namespace filters
    {
        class FragmentFilter;
    }
}
namespace starling
{
    namespace utils
    {
        class MatrixUtil;
    }
}

namespace starling
{
    namespace display
    {
        class BlendMode;
    }
}
namespace starling
{
    namespace display
    {
        class DisplayObjectContainer;
    }
}
namespace starling
{
    namespace display
    {
        class Stage;
    }
}

/** Dispatched when an object is added to a parent. */
//[Event(name="added",type="starling.events.Event")]
/** Dispatched when an object is connected to the stage (directly or indirectly). */
//[Event(name="addedToStage",type="starling.events.Event")]
/** Dispatched when an object is removed from its parent. */
//[Event(name="removed",type="starling.events.Event")]
/** Dispatched when an object is removed from the stage and won't be rendered any longer. */
//[Event(name="removedFromStage",type="starling.events.Event")]
/** Dispatched once every frame on every object that is connected to the stage. */
//[Event(name="enterFrame",type="starling.events.EnterFrameEvent")]
/** Dispatched when an object is touched. Bubbles. */
//[Event(name="touch",type="starling.events.TouchEvent")]

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

namespace starling
{
    namespace display
    {
        /**
         *  The DisplayObject class is the base class for all objects that are rendered on the
         *  screen.
         *
         *  <p><strong>The Display Tree</strong></p>
         *
         *  <p>In Starling, all displayable objects are organized in a display tree. Only objects that
         *  are part of the display tree will be displayed (rendered).</p>
         *
         *  <p>The display tree consists of leaf nodes (Image, Quad) that will be rendered directly to
         *  the screen, and of container nodes (subclasses of "DisplayObjectContainer", like "Sprite").
         *  A container is simply a display object that has child nodes - which can, again, be either
         *  leaf nodes or other containers.</p>
         *
         *  <p>At the base of the display tree, there is the Stage, which is a container, too. To create
         *  a Starling application, you create a custom Sprite subclass, and Starling will add an
         *  instance of this class to the stage.</p>
         *
         *  <p>A display object has properties that define its position in relation to its parent
         *  (x, y), as well as its rotation and scaling factors (scaleX, scaleY). Use the
         *  <code>alpha</code> and <code>visible</code> properties to make an object translucent or
         *  invisible.</p>
         *
         *  <p>Every display object may be the target of touch events. If you don't want an object to be
         *  touchable, you can disable the "touchable" property. When it's disabled, neither the object
         *  nor its children will receive any more touch events.</p>
         *
         *  <strong>Transforming coordinates</strong>
         *
         *  <p>Within the display tree, each object has its own local coordinate system. If you rotate
         *  a container, you rotate that coordinate system - and thus all the children of the
         *  container.</p>
         *
         *  <p>Sometimes you need to know where a certain point lies relative to another coordinate
         *  system. That's the purpose of the method <code>getTransformationMatrix</code>. It will
         *  create a matrix that represents the transformation of a point in one coordinate system to
         *  another.</p>
         *
         *  <strong>Subclassing</strong>
         *
         *  <p>Since DisplayObject is an abstract class, you cannot instantiate it directly, but have
         *  to use one of its subclasses instead. There are already a lot of them available, and most
         *  of the time they will suffice.</p>
         *
         *  <p>However, you can create custom subclasses as well. That way, you can create an object
         *  with a custom render function. You will need to implement the following methods when you
         *  subclass DisplayObject:</p>
         *
         *  <ul>
         *    <li><code>function render(support:RenderSupport, parentAlpha:Number):void</code></li>
         *    <li><code>function getBounds(targetSpace:DisplayObject,
         *                                 resultRect:Rectangle=null):Rectangle</code></li>
         *  </ul>
         *
         *  <p>Have a look at the Quad class for a sample implementation of the 'getBounds' method.
         *  For a sample on how to write a custom render function, you can have a look at this
         *  <a href="http://wiki.starling-framework.org/manual/custom_display_objects">article</a>
         *  in the Starling Wiki.</p>
         *
         *  <p>When you override the render method, it is important that you call the method
         *  'finishQuadBatch' of the support object. This forces Starling to render all quads that
         *  were accumulated before by different render methods (for performance reasons). Otherwise,
         *  the z-ordering will be incorrect.</p>
         *
         *  @see DisplayObjectContainer
         *  @see Sprite
         *  @see Stage
         */
        class DisplayObject : public starling::events::EventDispatcher
        {
            // members

        private:
            float mX;
        private:
            float mY;
        private:
            float mPivotX;
        private:
            float mPivotY;
        private:
            float mScaleX;
        private:
            float mScaleY;
        private:
            float mSkewX;
        private:
            float mSkewY;
        private:
            float mRotation;
        private:
            float mAlpha;
        private:
            bool mVisible;
        private:
            bool mTouchable;
        private:
            std::string mBlendMode;
        private:
            std::string mName;
        private:
            bool mUseHandCursor;
        private:
            DisplayObjectContainer *mParent;
        private:
            Matrix *mTransformationMatrix;
        private:
            bool mOrientationChanged;
        private:
            FragmentFilter *mFilter;

            /** Helper objects. */
        private:
            static std::vector<DisplayObject *> sAncestors;
        private:
            static Rectangle *sHelperRect;
        private:
            static Matrix *sHelperMatrix;

            /** @private */
        public:
            DisplayObject();

            /** Disposes all resources of the display object.
              * GPU buffers are released, event listeners are removed, filters are disposed. */
        public:
            void     dispose();

            /** Removes the object from its parent, if it has one. */
        public:
            void     removeFromParent(bool dispose=false);

            /** Creates a matrix that represents the transformation from the local coordinate system
             *  to another. If you pass a 'resultMatrix', the result will be stored in this matrix
             *  instead of creating a new object. */
        public:
            Matrix   *getTransformationMatrix(DisplayObject *targetSpace,
                                              Matrix *resultMatrix=NULL);

            /** Returns a rectangle that completely encloses the object as it appears in another
             *  coordinate system. If you pass a 'resultRectangle', the result will be stored in this
             *  rectangle instead of creating a new object. */// optimization
        public:
            Rectangle *getBounds(DisplayObject *targetSpace, Rectangle *resultRect=NULL);

            /** Returns the object that is found topmost beneath a point in local coordinates, or nil if
             *  the test fails. If "forTouch" is true, untouchable and invisible objects will cause
             *  the test to fail. */
        public:
            DisplayObject *hitTest(Point *localPoint, bool forTouch=false);

            /** Transforms a point from the local coordinate system to global (stage) coordinates.
             *  If you pass a 'resultPoint', the result will be stored in this point instead of
             *  creating a new object. */
        public:
            Point    *localToGlobal(Point *localPoint, Point *resultPoint=NULL);

            /** Transforms a point from global (stage) coordinates to the local coordinate system.
             *  If you pass a 'resultPoint', the result will be stored in this point instead of
             *  creating a new object. */
        public:
            Point    *globalToLocal(Point *globalPoint, Point *resultPoint=NULL);

            /** Renders the display object with the help of a support object. Never call this method
             *  directly, except from within another render method.
             *  @param support Provides utility functions for rendering.
             *  @param parentAlpha The accumulated alpha value from the object's parent up to the stage. */
        public:
            void     render(RenderSupport *support, float parentAlpha);

            /** Indicates if an object occupies any visible area. (Which is the case when its 'alpha',
             *  'scaleX' and 'scaleY' values are not zero, and its 'visible' property is enabled.) */
        public:
            bool      hasVisibleArea();

            // internal methods

            /** @private */
            void     setParent(DisplayObjectContainer *value);

            // helpers

        private:
            bool  isEquivalent(float a, float b, float epsilon=0.0001);

        private:
            float   normalizeAngle(float angle);

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
        public:
            Matrix       *transformationMatrix();

        public:
            void         transformationMatrix(Matrix *matrix);

            /** Indicates if the mouse cursor should transform into a hand while it's over the sprite.
             *  @default false */
        public:
            bool      useHandCursor();
        public:
            void         useHandCursor(bool value);

        private:
            void     onTouch(starling::events::TouchEvent *event);

            /** The bounds of the object relative to the local coordinates of the parent. */
        public:
            Rectangle    *bounds();

            /** The width of the object in pixels. */
        public:
            float       width();
        public:
            void         width(float value);

            /** The height of the object in pixels. */
        public:
            float       height();
        public:
            void         height(float value);

            /** The x coordinate of the object relative to the local coordinates of the parent. */
        public:
            float       x();
        public:
            void         x(float value);

            /** The y coordinate of the object relative to the local coordinates of the parent. */
        public:
            float       y();
        public:
            void         y(float value);

            /** The x coordinate of the object's origin in its own coordinate space (default: 0). */
        public:
            float       pivotX();
        public:
            void         pivotX(float value);

            /** The y coordinate of the object's origin in its own coordinate space (default: 0). */
        public:
            float       pivotY();
        public:
            void         pivotY(float value);

            /** The horizontal scale factor. '1' means no scale, negative values flip the object. */
        public:
            float       scaleX();
        public:
            void         scaleX(float value);

            /** The vertical scale factor. '1' means no scale, negative values flip the object. */
        public:
            float       scaleY();
        public:
            void         scaleY(float value);

            /** The horizontal skew angle in radians. */
        public:
            float       skewX();
        public:
            void         skewX(float value);

            /** The vertical skew angle in radians. */
        public:
            float       skewY();
        public:
            void         skewY(float value);

            /** The rotation of the object in radians. (In Starling, all angles are measured
             *  in radians.) */
        public:
            float       rotation();
        public:
            void         rotation(float value);

            /** The opacity of the object. 0 = transparent, 1 = opaque. */
        public:
            float       alpha();
        public:
            void         alpha(float value);

            /** The visibility of the object. An invisible object will be untouchable. */
        public:
            bool      visible();
        public:
            void         visible(bool value);

            /** Indicates if this object (and its children) will receive touch events. */
        public:
            bool      touchable();
        public:
            void         touchable(bool value);

            /** The blend mode determines how the object is blended with the objects underneath.
             *   @default auto
             *   @see starling.display.BlendMode */
        public:
            std::string       blendMode();
        public:
            void         blendMode(std::string value);

            /** The name of the display object (default: null). Used by 'getChildByName()' of
             *  display object containers. */
        public:
            std::string       name();
        public:
            void         name(std::string value);

            /** The filter that is attached to the display object. The starling.filters
             *  package contains several classes that define specific filters you can use.
             *  Beware that you should NOT use the same filter on more than one object (for
             *  performance reasons). */
        public:
            FragmentFilter *filter();
        public:
            void         filter(FragmentFilter *value);

            /** The display object container that contains this display object. */
        public:
            DisplayObjectContainer *parent();

            /** The topmost object in the display tree the object is part of. */
        public:
            DisplayObject *base();

            /** The root object the display object is connected to (i.e. an instance of the class
             *  that was passed to the Starling constructor), or null if the object is not connected
             *  to the stage. */
        public:
            DisplayObject *root();

            /** The stage the display object is connected to, or null if it is not connected
             *  to the stage. */
        public:
            Stage        *stage();
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECT_AS
#endif // __cplusplus

