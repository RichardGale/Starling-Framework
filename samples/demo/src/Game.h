#if !defined(__SAMPLES_DEMO_SRC_GAME_AS)
#define __SAMPLES_DEMO_SRC_GAME_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace flash
{
    namespace ui
    {
        class Keyboard;
    }
}
namespace flash
{
    namespace utils
    {
        class getDefinitionByName;
    }
}

namespace scenes
{
    class Scene;
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
        class Event;
    }
}
namespace starling
{
    namespace events
    {
        class KeyboardEvent;
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
        class AssetManager;
    }
}

namespace utils
{
    class ProgressBar;
}

using namespace flash::ui;
using namespace flash::utils;
using namespace scenes;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;
using namespace utils;

class Game : public starling::display::Sprite
{
    // Embed the Ubuntu Font. Beware: the 'embedAsCFF'-part IS REQUIRED!!!

private:
    static const Class *UbuntuRegular;

private:
    ProgressBar *mLoadingProgress;
private:
    MainMenu *mMainMenu;
private:
    Scene *mCurrentScene;

private:
    static AssetManager *sAssets;

public:
    Game();

public:
    void     start(Texture *background, AssetManager *assets);

private:
    void     showMainMenu();

private:
    void     onKey(KeyboardEvent *event);

private:
    void     onButtonTriggered(Event *event);

private:
    void     closeScene();

private:
    void     showScene(std::string name);

public:
    static AssetManager *assets();
};

#endif // __SAMPLES_DEMO_SRC_GAME_AS
#endif // __cplusplus

