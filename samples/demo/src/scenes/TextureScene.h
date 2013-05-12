#if !defined(__SAMPLES_DEMO_SRC_SCENES_TEXTURESCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_TEXTURESCENE_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling
{
    namespace display
    {
        class Image;
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

using namespace starling::display;
using namespace starling::text;
using namespace starling::textures;

namespace scenes
{
    class TextureScene: public Scene
    {
    public:
        TextureScene();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_TEXTURESCENE_AS
#endif // __cplusplus

