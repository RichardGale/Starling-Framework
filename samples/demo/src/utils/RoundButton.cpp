

#include "RoundButton.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/display/Button.h"
#include "starling/display/DisplayObject.h"
#include "starling/textures/Texture.h"

using namespace flash::geom;
using namespace starling::display;
using namespace starling::textures;

namespace utils {


        RoundButton::RoundButton(Texture* upState, std::string text, Texture* downState)
        {
            Button(upState, text, downState);
        }

        DisplayObject* RoundButton::hitTest(Point* localPoint, bool forTouch)
        {
            // When the user touches the screen, this method is used to find out if an object was 
            // hit. By default, this method uses the bounding box, but by overriding it, 
            // we can change the box (rectangle) to a circle (or whatever necessary).

            // when the hit test is done to check if a touch is hitting the object, invisible or
            // untouchable objects must cause the hit test to fail.
            if (forTouch && (!visible || !touchable))
                return NULL;

            // get center of button
            Rectangle* bounds = this->bounds;
            float centerX = bounds->width() / 2;
            float centerY = bounds->height() / 2;

            // calculate distance of localPoint to center. 
            // we keep it squared, since we want to avoid the 'sqrt()'-call.
            float sqDist = Math::pow(localPoint->x() - centerX, 2) +
                                Math::pow(localPoint->y() - centerY, 2);

            // when the squared distance is smaller than the squared radius, 
            // the point is inside the circle
            float radius = bounds->width / 2 - 8;
            if (sqDist < Math::pow(radius, 2)) return this;
            else return NULL;
        }
}

