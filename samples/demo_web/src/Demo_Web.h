#if !defined(__SAMPLES_DEMO_WEB_SRC_DEMO_WEB_AS)
#define __SAMPLES_DEMO_WEB_SRC_DEMO_WEB_AS
#if defined(__cplusplus)


#include "flex11.6.h"
#include "flash/display/Sprite.h"
namespace flash
{
    namespace system
    {
        class Capabilities;
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
    namespace events
    {
        class Event;
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

// If you set this class as your 'default application', it will run without a preloader.
// To use a preloader, see 'Demo_Web_Preloader.as'.

//[SWF(width="320",height="480",frameRate="60", backgroundColor="#222222")]

using namespace flash::display;
using namespace flash::system;
using namespace starling::core;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;

class Demo_Web : public flash::display::Sprite
{

private:
    Class *Background;

private:
    Starling *mStarling;

public:
    Demo_Web();

private:
    void     start();

private:
    void     onAddedToStage(Object *event);

private:
    void     onRootCreated(Event *event, Game *game);
};

#endif // __SAMPLES_DEMO_WEB_SRC_DEMO_WEB_AS
#endif // __cplusplus

