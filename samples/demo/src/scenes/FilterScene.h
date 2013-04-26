#if !defined(__SAMPLES_DEMO_SRC_SCENES_FILTERSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_FILTERSCENE_AS
#if defined(__cplusplus)


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
    namespace filters
    {
        class BlurFilter;
    }
}
namespace starling
{
    namespace filters
    {
        class ColorMatrixFilter;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}

using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::filters;
using namespace starling::text;

namespace scenes
{
    class FilterScene: public Scene
    {
    private:
        Button *mButton;
    private:
        Image *mImage;
    private:
        TextField *mInfoText;
    private:
        std::vector<void *> mFilterInfos;

    public:
        FilterScene();

    private:
        void     onButtonTriggered();

    private:
        void     initFilters();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_FILTERSCENE_AS
#endif // __cplusplus

