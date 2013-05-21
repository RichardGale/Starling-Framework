#if !defined(__SAMPLES_DEMO_SRC_SCENES_TOUCHSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_TOUCHSCENE_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace starling
{
    namespace display
    {
        class Image;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}
namespace starling
{
    namespace utils
    {
        class deg2rad;
    }
}

namespace utils
{
    class TouchSheet;
}

using namespace starling::display;
using namespace starling::text;
using namespace starling::utils;
using namespace utils;

namespace scenes
{
    class TouchScene : public Scene
    {
    public:
        TouchScene();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_TOUCHSCENE_AS
#endif // __cplusplus

