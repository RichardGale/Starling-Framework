#if !defined(__SAMPLES_SCAFFOLD_MOBILE_SRC_SCAFFOLD_MOBILE_AS)
#define __SAMPLES_SCAFFOLD_MOBILE_SRC_SCAFFOLD_MOBILE_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace flash
{
    namespace desktop
    {
        class NativeApplication;
    }
}
namespace flash
{
    namespace display
    {
        class Bitmap;
    }
}
#include "flash/display/Sprite.h"
namespace flash
{
    namespace events
    {
        class Event;
    }
}
namespace flash
{
    namespace filesystem
    {
        class File;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}
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
namespace starling
{
    namespace utils
    {
        class RectangleUtil;
    }
}
namespace starling
{
    namespace utils
    {
        class ScaleMode;
    }
}
namespace starling
{
    namespace utils
    {
        class formatString;
    }
}

//[SWF(frameRate="30",backgroundColor="#000")]

using namespace flash::desktop;
using namespace flash::display;
using namespace flash::events;
using namespace flash::filesystem;
using namespace flash::geom;
using namespace flash::system;
using namespace starling::core;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;

class Scaffold_Mobile : public flash::display::Sprite
{
    // We embed the "Ubuntu" font. Beware: the 'embedAsCFF'-part IS REQUIRED!!!

private:
    static const Class *UbuntuRegular;

    // Startup image for SD screens

private:
    static Class *Background;

    // Startup image for HD screens

private:
    static Class *BackgroundHD;

private:
    Starling *mStarling;

public:
    Scaffold_Mobile();
};

#endif // __SAMPLES_SCAFFOLD_MOBILE_SRC_SCAFFOLD_MOBILE_AS
#endif // __cplusplus

