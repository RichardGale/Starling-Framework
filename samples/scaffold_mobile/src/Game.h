#if !defined(__SAMPLES_SCAFFOLD_MOBILE_SRC_GAME_AS)
#define __SAMPLES_SCAFFOLD_MOBILE_SRC_GAME_AS
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
    namespace utils
    {
        class deg2rad;
    }
}

/** The Game class represents the actual game. In this scaffold, it just displays a
 *  Starling that moves around fast. When the user touches the Starling, the game ends. */

using namespace starling::animation;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::utils;

class Game : public starling::display::Sprite
{
public:
    static const std::string GAME_OVER;

private:
    Image *mBird;

public:
    Game();

private:
    void     init();

private:
    void     moveBird();

private:
    void     onBirdTouched(TouchEvent *event);
};

#endif // __SAMPLES_SCAFFOLD_MOBILE_SRC_GAME_AS
#endif // __cplusplus

