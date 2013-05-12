#if !defined(__SAMPLES_DEMO_SRC_SCENES_TEXTSCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_TEXTSCENE_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling { namespace text { class BitmapFont; } }
namespace starling { namespace text { class TextField; } }
namespace starling { namespace utils { class Color; } }
namespace starling { namespace utils { class HAlign; } }
namespace starling { namespace utils { class VAlign; } }

using namespace starling::text;
using namespace starling::text;
using namespace starling::utils;
using namespace starling::utils;
using namespace starling::utils;

namespace scenes {
    class TextScene: public Scene
    {
        public:          TextScene();

        private: void     init();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_TEXTSCENE_AS
#endif // __cplusplus

