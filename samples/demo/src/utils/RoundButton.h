#if !defined(__SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS)
#define __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#if defined(__cplusplus)


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
using namespace flash::geom;
using namespace starling::display;
using namespace starling::display;
using namespace starling::textures;

namespace utils
{
    class RoundButton: public Button
    {
    public:
        RoundButton(Texture *upState, std::string text, Texture *downState);

    public:
        virtual DisplayObject *hitTest(Point *localPoint, bool forTouch);
    };
}

#endif // __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#endif // __cplusplus

