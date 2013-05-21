#if !defined(__SAMPLES_DEMO_SRC_SCENES_RENDERTEXTURESCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_RENDERTEXTURESCENE_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace flash
{
    namespace geom
    {
        class Point;
    }
}
namespace flash
{
    namespace utils
    {
        class Dictionary;
    }
}

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
    namespace events
    {
        class Touch;
    }
}
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
        class RenderTexture;
    }
}

using namespace flash::geom;
using namespace flash::utils;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;

namespace scenes
{
    class RenderTextureScene : public Scene
    {
    private:
        RenderTexture *mRenderTexture;
    private:
        Image *mCanvas;
    private:
        Image *mBrush;
    private:
        Button *mButton;
    private:
        std::map<std::string, void *> mColors;

    public:
        RenderTextureScene();

    private:
        void     onTouch(TouchEvent *event);

    private:
        void     onButtonTriggered();

    public:
        virtual void     dispose();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_RENDERTEXTURESCENE_AS
#endif // __cplusplus

