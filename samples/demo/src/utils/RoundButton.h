#if !defined(__SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS)
#define __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#if defined(__cplusplus)


#include "flex11.6.h"
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

#include "starling/display/Button.h"
namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}

using namespace flash::geom;
using namespace starling::display;
using namespace starling::textures;

namespace utils
{
    class RoundButton : public starling::display::Button
    {
    public:
        RoundButton(Texture *upState, std::string text="", Texture *downState=NULL);

    public:
        virtual DisplayObject *hitTest(Point *localPoint, bool forTouch   =false);
    };
}

#endif // __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#endif // __cplusplus

