#if !defined(__STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECTCONTAINER_AS)
#define __STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECTCONTAINER_AS
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



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace geom { class Matrix; } }
namespace flash { namespace geom { class Point; } }
namespace flash { namespace geom { class Rectangle; } }
namespace flash { namespace system { class Capabilities; } }
namespace flash { namespace utils { class getQualifiedClassName; } }

namespace starling { namespace core { class RenderSupport; } }
namespace starling { namespace core { class starling_internal; } }
namespace starling { namespace errors { class AbstractClassError; } }
namespace starling { namespace events { class Event; } }
namespace starling { namespace filters { class FragmentFilter; } }
namespace starling { namespace utils { class MatrixUtil; } }

    //use starling_internal        ;

    /**
     *  A DisplayObjectContainer represents a collection of display objects.
     *  It is the base class of all display objects that act as a container for other objects. By 
     *  maintaining an ordered list of children, it defines the back-to-front positioning of the 
     *  children within the display tree.
     *  
     *  <p>A container does not a have size in itself. The width and height properties represent the 
     *  extents of its children. Changing those properties will scale all children accordingly.</p>
     *  
     *  <p>As this is an abstract class, you can't instantiate it directly, but have to 
     *  use a subclass instead. The most lightweight container class is "Sprite".</p>
     *  
     *  <strong>Adding and removing children</strong>
     *  
     *  <p>The class defines methods that allow you to add or remove children. When you add a child, 
     *  it will be added at the frontmost position, possibly occluding a child that was added 
     *  before. You can access the children via an index. The first child will have index 0, the 
     *  second child index 1, etc.</p> 
     *  
     *  Adding and removing objects from a container triggers non-bubbling events.
     *  
     *  <ul>
     *   <li><code>Event.ADDED</code>: the object was added to a parent.</li>
     *   <li><code>Event.ADDED_TO_STAGE</code>: the object was added to a parent that is 
     *       connected to the stage, thus becoming visible now.</li>
     *   <li><code>Event.REMOVED</code>: the object was removed from a parent.</li>
     *   <li><code>Event.REMOVED_FROM_STAGE</code>: the object was removed from a parent that 
     *       is connected to the stage, thus becoming invisible now.</li>
     *  </ul>
     *  
     *  Especially the <code>ADDED_TO_STAGE</code> event is very helpful, as it allows you to 
     *  automatically execute some logic (e.g. start an animation) when an object is rendered the 
     *  first time.
     *  
     *  @see Sprite
     *  @see DisplayObject
     */
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::geom;
using namespace flash::system;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::core;
using namespace starling::errors;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::utils;

namespace starling {
namespace display {
    class DisplayObjectContainer: public DisplayObject
    {
        // members
        private:  std::vector<DisplayObject*>* mChildren;

        /** Helper objects. */
        private: static  Matrix* sHelperMatrix;
        private: static  Point* sHelperPoint;
        private: static  std::vector<DisplayObject*>* sBroadcastListeners;

        // construction

        /** @private */
        public:          DisplayObjectContainer();

        /** Disposes the resources of all children. */
        public: virtual void     dispose();

        // child management

        /** Adds a child to the container. It will be at the frontmost position. */
        public: DisplayObject* addChild(DisplayObject* child);

        /** Adds a child to the container at a certain index. */
        public: DisplayObject* addChildAt(DisplayObject* child, int index);

        /** Removes a child from the container. If the object is not a child, nothing happens. 
         *  If requested, the child will be disposed right away. */
        public: DisplayObject* removeChild(DisplayObject* child, bool dispose   =false);

        /** Removes a child at a certain index. Children above the child will move down. If
         *  requested, the child will be disposed right away. */
        public: DisplayObject* removeChildAt(int index, bool dispose   =false);

        /** Removes a range of children from the container (endIndex included). 
         *  If no arguments are given, all children will be removed. */
        public: void     removeChildren(int beginIndex=0, int endIndex=-1, bool dispose   =false);

        /** Returns a child object at a certain index. */
        public: DisplayObject* getChildAt(int index);

        /** Returns a child object with a certain name (non-recursively). */
        public: DisplayObject* getChildByName(std::string name);

        /** Returns the index of a child within the container, or "-1" if it is not found. */
        public: int      getChildIndex(DisplayObject* child);

        /** Moves a child to a certain index. Children at and after the replaced position move up.*/
        public: void     setChildIndex(DisplayObject* child, int index);

        /** Swaps the indexes of two children. */
        public: void     swapChildren(DisplayObject* child1, DisplayObject* child2);

        /** Swaps the indexes of two children. */
        public: void     swapChildrenAt(int index1, int index2);

        /** Sorts the children according to a given function (that works just like the sort function
         *  of the Vector class). */
        public: void     sortChildren(Function* compareFunction);

        /** Determines if a certain object is a child of the container (recursively). */
        public: bool     contains(DisplayObject* child);

        /** @inheritDoc */
        public: virtual Rectangle* getBounds(DisplayObject* targetSpace, Rectangle* resultRect=NULL);

        /** @inheritDoc */
        public: virtual DisplayObject* hitTest(Point* localPoint, bool forTouch   =false);

        /** @inheritDoc */                           // front to back!
        public: virtual void     render(RenderSupport* support, float parentAlpha);

        /** Dispatches an event on all children (recursively). The event must not bubble. */
        public: void     broadcastEvent(Event* event);

        /** Dispatches an event with the given parameters on all children (recursively). 
         *  The method uses an internal pool of event objects to avoid allocations. */
        public: void     broadcastEventWith(std::string type, Object* data=NULL);

        private: void     getChildEventListeners(DisplayObject* object, std::string eventType,
                                                std::vector<DisplayObject*>* listeners);

        /** The number of children of this container. */
        public: int          numChildren();
    };
}
}

#endif // __STARLING_SRC_STARLING_DISPLAY_DISPLAYOBJECTCONTAINER_AS
#endif // __cplusplus

