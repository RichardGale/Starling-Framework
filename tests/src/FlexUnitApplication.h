#if !defined(__TESTS_SRC_FLEXUNITAPPLICATION_AS)
#define __TESTS_SRC_FLEXUNITAPPLICATION_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
class Array;

#include "flash/display/Sprite.h"

namespace flexunit { namespace flexui { class FlexUnitTestRunnerUIAS; } }

namespace tests { class BlendModeTest; }
namespace tests { class ColorTest; }
namespace tests { class DelayedCallTest; }
namespace tests { class DisplayObjectContainerTest; }
namespace tests { class DisplayObjectTest; }
namespace tests { class EventTest; }
namespace tests { class JugglerTest; }
namespace tests { class MovieClipTest; }
namespace tests { class QuadTest; }
namespace tests { class RectangleUtilTest; }
namespace tests { class TextureAtlasTest; }
namespace tests { class TextureTest; }
namespace tests { class TweenTest; }
namespace tests { class UtilsTest; }
namespace tests { class VertexDataTest; }

using namespace flash::display;
using namespace flexunit::flexui;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;

    class FlexUnitApplication: public Sprite
    {
        public:          FlexUnitApplication();

        private: void     onCreationComplete();

        public: std::vector<void*> currentRunTestSuite();
    };

#endif // __TESTS_SRC_FLEXUNITAPPLICATION_AS
#endif // __cplusplus

