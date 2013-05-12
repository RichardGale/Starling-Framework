#if !defined(__SAMPLES_DEMO_WEB_SRC_DEMO_WEB_PRELOADER_AS)
#define __SAMPLES_DEMO_WEB_SRC_DEMO_WEB_PRELOADER_AS
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
    namespace display
    {
        class DisplayObject;
    }
}
#include "flash/display/MovieClip.h"
namespace flash
{
    namespace display
    {
        class Shape;
    }
}
namespace flash
{
    namespace display
    {
        class StageAlign;
    }
}
namespace flash
{
    namespace display
    {
        class StageScaleMode;
    }
}
namespace flash
{
    namespace events
    {
        class Event;
    }
}
namespace flash
{
    namespace utils
    {
        class getDefinitionByName;
    }
}

namespace starling
{
    namespace utils
    {
        class Color;
    }
}

// To show a Preloader while the SWF is being transferred from the server,
// set this class as your 'default application' and add the following
// compiler argument: '-frame StartupFrame Demo_Web'

//[SWF(width="320",height="480",frameRate="60", backgroundColor="#222222")]
using namespace flash::display;
using namespace flash::display;
using namespace flash::display;
using namespace flash::display;
using namespace flash::display;
using namespace flash::events;
using namespace flash::utils;
using namespace starling::utils;

class Demo_Web_Preloader: public MovieClip
{
private:
    const std::string STARTUP_CLASS;

private:
    Shape *mProgressIndicator;
private:
    int mFrameCount;

public:
    Demo_Web_Preloader();

private:
    void     onAddedToStage(Event *event);

private:
    void     onEnterFrame(Event *event);

private:
    Shape   *createProgressIndicator(float radius =12, int elements=8);

private:
    void     dispose();

private:
    void     run();
};

#endif // __SAMPLES_DEMO_WEB_SRC_DEMO_WEB_PRELOADER_AS
#endif // __cplusplus

