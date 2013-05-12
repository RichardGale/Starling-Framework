#if !defined(__SAMPLES_DEMO_SRC_SCENES_MOVIESCENE_AS)
#define __SAMPLES_DEMO_SRC_SCENES_MOVIESCENE_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace media { class Sound; } }

namespace starling { namespace core { class Starling; } }
namespace starling { namespace display { class MovieClip; } }
namespace starling { namespace events { class Event; } }
namespace starling { namespace textures { class Texture; } }

using namespace flash::media;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;

namespace scenes {
    class MovieScene: public Scene
    {
        private:  MovieClip* mMovie;

        public:          MovieScene();

        private: void     onAddedToStage();

        private: void     onRemovedFromStage();

        public: virtual void     dispose();
    };
}

#endif // __SAMPLES_DEMO_SRC_SCENES_MOVIESCENE_AS
#endif // __cplusplus

