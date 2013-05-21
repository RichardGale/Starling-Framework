#if !defined(__SAMPLES_DEMO_SRC_SCENES_SCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_SCENE_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace starling
{
    namespace display
    {
        class Button;
    }
}
#include "starling/display/Sprite.h"

using namespace starling::display;

namespace scenes
{
    class Scene : public starling::display::Sprite
    {
    private:
        Button *mBackButton;

    public:
        Scene();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_SCENE_AS
#endif // __cplusplus

