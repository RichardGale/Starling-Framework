#if !defined(__SAMPLES_DEMO_SRC_SCENES_MASKSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_MASKSCENE_AS
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

namespace starling
{
    namespace core
    {
        class Starling;
    }
}
namespace starling
{
    namespace display
    {
        class Image;
    }
}
namespace starling
{
    namespace display
    {
        class Quad;
    }
}
namespace starling
{
    namespace display
    {
        class Sprite;
    }
}
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
namespace starling
{
    namespace filters
    {
        class ColorMatrixFilter;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}

using namespace flash::geom;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::text;

namespace scenes
{
    class MaskScene: public Scene
    {
    private:
        Sprite *mContents;
    private:
        Quad *mClipQuad;

    public:
        MaskScene();

    private:
        void     onTouch(TouchEvent *event);
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_MASKSCENE_AS
#endif // __cplusplus
