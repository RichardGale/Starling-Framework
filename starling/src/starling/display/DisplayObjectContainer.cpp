// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "DisplayObjectContainer.h"
#include "flash/geom/Matrix.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"
#include "flash/utils/getQualifiedClassName.h"

#include "starling/core/RenderSupport.h"
#include "starling/core/starling_internal.h"
#include "starling/errors/AbstractClassError.h"
#include "starling/events/Event.h"
#include "starling/filters/FragmentFilter.h"
#include "starling/utils/MatrixUtil.h"

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


        // members

        /** Helper objects. */
         Matrix* DisplayObjectContainer::sHelperMatrix=newMatrix();
         Point* DisplayObjectContainer::sHelperPoint=newPoint();
         std::vector<DisplayObject*>* DisplayObjectContainer::sBroadcastListeners=new<DisplayObject*>[];

        // construction

        /** @private */
        DisplayObjectContainer::DisplayObjectContainer()
        {
            if (Capabilities::isDebugger&&
                getQualifiedClassName(this) == "starling.display::DisplayObjectContainer")
            {
                throw new AbstractClassError();
            }

            mChildren.clear();
        }

        /** Disposes the resources of all children. */
        void DisplayObjectContainer::dispose()
        {
            for ( int i=mChildren.length-1;i>=0; --i)
                mChildren[i]->dispose();

            super->dispose();
        }

        // child management

        /** Adds a child to the container. It will be at the frontmost position. */
        DisplayObject* DisplayObjectContainer::addChild(DisplayObject* child)
        {
            addChildAt(child, numChildren);
            return child;
        }

        /** Adds a child to the container at a certain index. */
        DisplayObject* DisplayObjectContainer::addChildAt(DisplayObject* child, int index)
        {
             int numChildren= mChildren.length;

            if (index >= 0 && index <= numChildren)
            {
                child->removeFromParent();

                // 'splice' creates a temporary object, so we avoid it if it's not necessary
                if (index == numChildren) mChildren.push_back(child);
                else                      mChildren.splice(index, 0, child);

                child->setParent(this);
                child->dispatchEventWith(Event::ADDED,true);

                if (stage)
                {
                     DisplayObjectContainer* container=static_cast<DisplayObjectContainer>(child);
                    if (container) container->broadcastEventWith(Event::ADDED_TO_STAGE);
                    else           child->dispatchEventWith(Event::ADDED_TO_STAGE);
                }

                return child;
            }
            else
            {
                throw new RangeError("Invalid child index");
            }
        }

        /** Removes a child from the container. If the object is not a child, nothing happens. 
         *  If requested, the child will be disposed right away. */
        DisplayObject* DisplayObjectContainer::removeChild(DisplayObject* child, bool dispose)
        {
             int childIndex= getChildIndex(child);
            if (childIndex != -1) removeChildAt(childIndex, dispose);
            return child;
        }

        /** Removes a child at a certain index. Children above the child will move down. If
         *  requested, the child will be disposed right away. */
        DisplayObject* DisplayObjectContainer::removeChildAt(int index, bool dispose)
        {
            if (index >= 0 && index < numChildren)
            {
                 DisplayObject* child=mChildren[index];
                child->dispatchEventWith(Event::REMOVED,true);

                if (stage)
                {
                     DisplayObjectContainer* container=static_cast<DisplayObjectContainer>(child);
                    if (container) container->broadcastEventWith(Event::REMOVED_FROM_STAGE);
                    else           child->dispatchEventWith(Event::REMOVED_FROM_STAGE);
                }

                child->setParent(NULL);
                index = mChildren.indexOf(child); // index might have changed by event handler
                if (index >= 0) mChildren.splice(index, 1);
                if (dispose) child->dispose();

                return child;
            }
            else
            {
                throw new RangeError("Invalid child index");
            }
        }

        /** Removes a range of children from the container (endIndex included). 
         *  If no arguments are given, all children will be removed. */
        void DisplayObjectContainer::removeChildren(int beginIndex, int endIndex, bool dispose)
        {
            if (endIndex < 0 || endIndex >= numChildren)
                endIndex = numChildren - 1;

            for ( int i=beginIndex;i<=endIndex; ++i)
                removeChildAt(beginIndex, dispose);
        }

        /** Returns a child object at a certain index. */
        DisplayObject* DisplayObjectContainer::getChildAt(int index)
        {
            if (index >= 0 && index < numChildren)
                return mChildren[index];
            else
                throw new RangeError("Invalid child index");
        }

        /** Returns a child object with a certain name (non-recursively). */
        DisplayObject* DisplayObjectContainer::getChildByName(std::string name)
        {
             int numChildren= mChildren.length;
            for ( int i=0;i<numChildren; ++i)
                if (mChildren[i]->name== name) return mChildren[i];

            return NULL;
        }

        /** Returns the index of a child within the container, or "-1" if it is not found. */
        int DisplayObjectContainer::getChildIndex(DisplayObject* child)
        {
            return mChildren.indexOf(child);
        }

        /** Moves a child to a certain index. Children at and after the replaced position move up.*/
        void DisplayObjectContainer::setChildIndex(DisplayObject* child, int index)
        {
             int oldIndex= getChildIndex(child);
            if (oldIndex == -1) throw new ArgumentError("Not a child of this container");
            mChildren.splice(oldIndex, 1);
            mChildren.splice(index, 0, child);
        }

        /** Swaps the indexes of two children. */
        void DisplayObjectContainer::swapChildren(DisplayObject* child1, DisplayObject* child2)
        {
             int index1= getChildIndex(child1);
             int index2= getChildIndex(child2);
            if (index1 == -1 || index2 == -1) throw new ArgumentError("Not a child of this container");
            swapChildrenAt(index1, index2);
        }

        /** Swaps the indexes of two children. */
        void DisplayObjectContainer::swapChildrenAt(int index1, int index2)
        {
             DisplayObject* child1=getChildAt(index1);
             DisplayObject* child2=getChildAt(index2);
            mChildren[index1] = child2;
            mChildren[index2] = child1;
        }

        /** Sorts the children according to a given function (that works just like the sort function
         *  of the Vector class). */
        void DisplayObjectContainer::sortChildren(Function* compareFunction)
        {
            mChildren = mChildren.sort(compareFunction);
        }

        /** Determines if a certain object is a child of the container (recursively). */
        bool DisplayObjectContainer::contains(DisplayObject* child)
        {
            while (child)
            {
                if (child == this) return true;
                else child = child->parent;
            }
            return false;
        }

        /** @inheritDoc */
        Rectangle* DisplayObjectContainer::getBounds(DisplayObject* targetSpace, Rectangle* resultRect)
        {
            if (resultRect == NULL) resultRect = new Rectangle();

             int numChildren= mChildren.length;

            if (numChildren == 0)
            {
                getTransformationMatrix(targetSpace, sHelperMatrix);
                MatrixUtil::transformCoords(sHelperMatrix,0.0, 0.0, sHelperPoint);
                resultRect->setTo(sHelperPoint->x,sHelperPoint->y,0,0);
            }
            else if (numChildren == 1)
            {
                resultRect = mChildren[0]->getBounds(targetSpace,resultRect);
            }
            else
            {
                 float minX = Number::MAX_VALUE, float maxX  = -Number::MAX_VALUE;
                 float minY = Number::MAX_VALUE, float maxY  = -Number::MAX_VALUE;

                for ( int i=0;i<numChildren; ++i)
                {
                    mChildren[i]->getBounds(targetSpace,resultRect);
                    minX = minX < resultRect->x? minX : resultRect->x;
                    maxX = maxX > resultRect->right? maxX : resultRect->right;
                    minY = minY < resultRect->y? minY : resultRect->y;
                    maxY = maxY > resultRect->bottom? maxY : resultRect->bottom;
                }

                resultRect->setTo(minX,minY, maxX - minX, maxY - minY);
            }

            return resultRect;
        }

        /** @inheritDoc */
        DisplayObject* DisplayObjectContainer::hitTest(Point* localPoint, bool forTouch)
        {
            if (forTouch && (!visible || !touchable))
                return NULL;

             float localX = localPoint->x;
             float localY = localPoint->y;

             int numChildren= mChildren.length;
            for ( int i=numChildren-1;i>=0; --i)
            {
                 DisplayObject* child=mChildren[i];
                getTransformationMatrix(child, sHelperMatrix);

                MatrixUtil::transformCoords(sHelperMatrix,localX, localY, sHelperPoint);
                 DisplayObject* target=child->hitTest(sHelperPoint,forTouch);

                if (target) return target;
            }

            return NULL;
        }

        /** @inheritDoc */                           // front to back!
        void DisplayObjectContainer::render(RenderSupport* support, float parentAlpha)
        {
             float alpha = parentAlpha * this->alpha;
             int numChildren= mChildren.length;
             std::string blendMode=support->blendMode;

            for ( int i=0;i<numChildren; ++i)
            {
                 DisplayObject* child=mChildren[i];

                if (child->hasVisibleArea)
                {
                     FragmentFilter* filter=child->filter;

                    support->pushMatrix();
                    support->transformMatrix(child);
                    support->blendMode= child->blendMode;

                    if (filter) filter->render(child,support, alpha);
                    else        child->render(support,alpha);

                    support->blendMode= blendMode;
                    support->popMatrix();
                }
            }
        }

        /** Dispatches an event on all children (recursively). The event must not bubble. */
        void DisplayObjectContainer::broadcastEvent(Event* event)
        {
            if (event->bubbles)
                throw new ArgumentError("Broadcast of bubbling events is prohibited");

            // The event listeners might modify the display tree, which could make the loop crash. 
            // Thus, we collect them in a list and iterate over that list instead.
            // And since another listener could call this method internally, we have to take 
            // care that the static helper vector does not get currupted.

             int fromIndex= sBroadcastListeners.length;
            getChildEventListeners(this, event->type,sBroadcastListeners);
             int toIndex= sBroadcastListeners.length;

            for ( int i=fromIndex;i<toIndex; ++i)
                sBroadcastListeners[i]->dispatchEvent(event);

            sBroadcastListeners.length = fromIndex;
        }

        /** Dispatches an event with the given parameters on all children (recursively). 
         *  The method uses an internal pool of event objects to avoid allocations. */
        void DisplayObjectContainer::broadcastEventWith(std::string type, Object* data)
        {
             Event* event=Event::fromPool(type,false, data);
            broadcastEvent(event);
            Event::toPool(event);
        }

        void DisplayObjectContainer::getChildEventListeners(DisplayObject* object, std::string eventType,
                                                std::vector<DisplayObject*>* listeners)
        {
             DisplayObjectContainer* container=static_cast<DisplayObjectContainer>(object);

            if (object->hasEventListener(eventType))
                listeners->push(object);

            if (container)
            {
                 std::vector<DisplayObject*>* children=container->mChildren;
                 int numChildren= children.length;

                for ( int i=0;i<numChildren; ++i)
                    getChildEventListeners(children[i], eventType, listeners);
            }
        }

        /** The number of children of this container. */
        int DisplayObjectContainer::numChildren()     { return mChildren.length; }
}
}

