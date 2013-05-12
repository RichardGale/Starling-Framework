#if !defined(__SAMPLES_DEMO_SRC_UTILS_PROGRESSBAR_AS)
#define __SAMPLES_DEMO_SRC_UTILS_PROGRESSBAR_AS
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
        class BitmapData;
    }
}
namespace flash
{
    namespace display
    {
        class Shape;
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
namespace starling
{
    namespace display
    {
        class Quad;
    }
}
#include "starling/display/Sprite.h"
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}

using namespace flash::display;
using namespace flash::display;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::textures;

namespace utils
{
    class ProgressBar: public Sprite
    {
    private:
        Quad *mBar;
    private:
        Image *mBackground;

    public:
        ProgressBar(int width, int height);

    private:
        void     init(int width, int height);

    public:
        float        ratio();
    public:
        void         ratio(float value);
    };
}

#endif // __SAMPLES_DEMO_SRC_UTILS_PROGRESSBAR_AS
#endif // __cplusplus
