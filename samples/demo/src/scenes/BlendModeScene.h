#if !defined(__SAMPLES_DEMO_SRC_SCENES_BLENDMODESCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_BLENDMODESCENE_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling
{
    namespace display
    {
        class BlendMode;
    }
}
namespace starling
{
    namespace display
    {
        class Button;
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
    namespace events
    {
        class Event;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}

using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;

namespace scenes
{
    class BlendModeScene: public Scene
    {
    private:
        Button *mButton;
    private:
        Image *mImage;
    private:
        TextField *mInfoText;

    private:
        std::vector<void *> mBlendModes;

    public:
        BlendModeScene();

    private:
        void     onButtonTriggered();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_BLENDMODESCENE_AS
#endif // __cplusplus

