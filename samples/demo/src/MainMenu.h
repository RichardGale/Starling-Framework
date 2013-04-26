#if !defined(__SAMPLES_DEMO_SRC_MAINMENU_AS)
#define __SAMPLES_DEMO_SRC_MAINMENU_AS
#if defined(__cplusplus)


namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
    }
}

namespace scenes
{
    class AnimationScene;
}
namespace scenes
{
    class BenchmarkScene;
}
namespace scenes
{
    class BlendModeScene;
}
namespace scenes
{
    class CustomHitTestScene;
}
namespace scenes
{
    class FilterScene;
}
namespace scenes
{
    class MaskScene;
}
namespace scenes
{
    class MovieScene;
}
namespace scenes
{
    class RenderTextureScene;
}
namespace scenes
{
    class TextScene;
}
namespace scenes
{
    class TextureScene;
}
namespace scenes
{
    class TouchScene;
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
#include "starling/display/Sprite.h"
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
    namespace text
    {
        class TextField;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace utils
    {
        class VAlign;
    }
}

using namespace flash::utils;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace scenes;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

class MainMenu: public Sprite
{
public:
    MainMenu();

private:
    void     init();

private:
    void     onInfoTextTouched(TouchEvent *event);
};

#endif // __SAMPLES_DEMO_SRC_MAINMENU_AS
#endif // __cplusplus

