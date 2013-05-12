#if !defined(__SAMPLES_DEMO_SRC_UTILS_TOUCHSHEET_AS)
#define __SAMPLES_DEMO_SRC_UTILS_TOUCHSHEET_AS
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

namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
#include "starling/display/Sprite.h"
namespace starling
{
    namespace events
    {
        class Touch;
    }
}
namespace starling
{
    namespace events
    {
        class TouchEvent;
    }
}
namespace starling
{
    namespace events
    {
        class TouchPhase;
    }
}

using namespace flash::geom;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;

namespace utils
{
    class TouchSheet: public Sprite
    {
    public:
        TouchSheet(DisplayObject *contents=NULL);

    private:
        void     onTouch(TouchEvent *event);

    public:
        virtual void     dispose();
    };
}

#endif // __SAMPLES_DEMO_SRC_UTILS_TOUCHSHEET_AS
#endif // __cplusplus

