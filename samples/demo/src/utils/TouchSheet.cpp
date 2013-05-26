

#include "TouchSheet.h"
#include "flash/geom/Point.h"

#include "starling/display/DisplayObject.h"
#include "starling/display/Sprite.h"
#include "starling/events/Touch.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"

using namespace flash::geom;
using namespace starling::display;
using namespace starling::events;

namespace utils {


        TouchSheet::TouchSheet(DisplayObject* contents)
        {
            addEventListener(TouchEvent::TOUCH, onTouch);
            useHandCursor = true;

            if (contents)
            {
                contents->x ( int(contents->width() / -2));
                contents->y ( int(contents->height() / -2));
                addChild(contents);
            }
        }

        void TouchSheet::onTouch(TouchEvent* event)
        {
            std::vector<Touch*> touches=event->getTouches(this,TouchPhase::MOVED);

            if (touches.size() == 1)
            {
                // one finger touching -> move
                Point* delta = touches[0]->getMovement(parent);
                x += delta->x();
                y += delta->y();
            }
            else if (touches.size() == 2)
            {
                // two fingers touching -> rotate and scale
                Touch* touchA = touches[0];
                Touch* touchB = touches[1];

                Point* currentPosA  = touchA->getLocation(parent);
                Point* previousPosA = touchA->getPreviousLocation(parent);
                Point* currentPosB  = touchB->getLocation(parent);
                Point* previousPosB = touchB->getPreviousLocation(parent);

                Point* currentVector  = currentPosA->subtract(currentPosB);
                Point* previousVector = previousPosA->subtract(previousPosB);

                float currentAngle  = Math::atan2(currentVector->y(), currentVector->x());
                float previousAngle = Math::atan2(previousVector->y(), previousVector->x());
                float deltaAngle = currentAngle - previousAngle;

                // update pivot point based on previous center
                Point* previousLocalA  = touchA->getPreviousLocation(this);
                Point* previousLocalB  = touchB->getPreviousLocation(this);
                pivotX = (previousLocalA->x() + previousLocalB->x()) * 0.5;
                pivotY = (previousLocalA->y() + previousLocalB->y()) * 0.5;

                // update location based on the current center
                x = (currentPosA->x() + currentPosB->x()) * 0.5;
                y = (currentPosA->y() + currentPosB->y()) * 0.5;

                // rotate
                rotation += deltaAngle;

                // scale
                float sizeDiff = currentVector->length() / previousVector->length();
                scaleX *= sizeDiff;
                scaleY *= sizeDiff;
            }

            Touch* touch = event->getTouch(this, TouchPhase::ENDED);

            if (touch && touch->tapCount() == 2)
                parent()->addChild(this); // bring self to front

            // enable this code to see when you're hovering over the object
            // touch = event.getTouch(this, TouchPhase.HOVER);            
            // alpha = touch ? 0.8 : 1.0;
        }

        void TouchSheet::dispose()
        {
            removeEventListener(TouchEvent::TOUCH, onTouch);
            Sprite::dispose();
        }
}

