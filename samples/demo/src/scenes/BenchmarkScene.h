#if !defined(__SAMPLES_DEMO_SRC_SCENES_BENCHMARKSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_BENCHMARKSCENE_AS
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
    namespace system
    {
        class System;
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
    namespace display
    {
        class Sprite;
    }
}
namespace starling
{
    namespace events
    {
        class EnterFrameEvent;
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
namespace starling
{
    namespace utils
    {
        class formatString;
    }
}

using namespace flash::system;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::text;
using namespace starling::utils;

namespace scenes
{
    class BenchmarkScene: public Scene
    {
    private:
        Button *mStartButton;
    private:
        TextField *mResultText;

    private:
        Sprite *mContainer;
    private:
        int mFrameCount;
    private:
        float mElapsed;
    private:
        bool mStarted;
    private:
        int mFailCount;
    private:
        int mWaitFrames;

    public:
        BenchmarkScene();

    public:
        virtual void     dispose();

    private:
        void     onEnterFrame(EnterFrameEvent *event);

    private:
        void     onStartButtonTriggered();

    private:
        void     addTestObjects();

    private:
        void     benchmarkComplete();


    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_BENCHMARKSCENE_AS
#endif // __cplusplus

