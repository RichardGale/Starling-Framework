#if !defined(__SAMPLES_DEMO_SRC_SCENES_ANIMATIONSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_ANIMATIONSCENE_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace starling
{
    namespace animation
    {
        class Transitions;
    }
}
namespace starling
{
    namespace animation
    {
        class Tween;
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
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace utils
    {
        class Color;
    }
}
namespace starling
{
    namespace utils
    {
        class deg2rad;
    }
}

using namespace starling::animation;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

namespace scenes
{
    class AnimationScene : public Scene
    {
    private:
        Button *mStartButton;
    private:
        Button *mDelayButton;
    private:
        Image *mEgg;
    private:
        TextField *mTransitionLabel;
    private:
        std::vector<void *> mTransitions;

    public:
        AnimationScene();

    private:
        void     resetEgg();

    private:
        void     onStartButtonTriggered();

    private:
        void     onDelayButtonTriggered();

    private:
        void     colorizeEgg(bool colorize);

    public:
        virtual void     dispose();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_ANIMATIONSCENE_AS
#endif // __cplusplus

