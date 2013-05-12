#if !defined(__SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS)
#define __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
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
        RoundButton(Texture *upState, std::string text="", Texture *downState=NULL);

    public:
        virtual DisplayObject *hitTest(Point *localPoint, bool forTouch   =false);
    };
}

#endif // __SAMPLES_DEMO_SRC_UTILS_ROUNDBUTTON_AS
#endif // __cplusplus

