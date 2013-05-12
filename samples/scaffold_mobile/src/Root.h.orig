#if !defined(__SAMPLES_SCAFFOLD_MOBILE_SRC_ROOT_AS)
#define __SAMPLES_SCAFFOLD_MOBILE_SRC_ROOT_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling { namespace core { class Starling; } }
namespace starling { namespace display { class Image; } }
#include "starling/display/Sprite.h"
namespace starling { namespace events { class Event; } }
namespace starling { namespace textures { class Texture; } }
namespace starling { namespace utils { class AssetManager; } }

namespace utils { class ProgressBar; }

    /** The Root class is the topmost display object in your game. It loads all the assets
     *  and displays a progress bar while this is happening. Later, it is responsible for
     *  switching between game and menu. For this, it listens to "START_GAME" and "GAME_OVER"
     *  events fired by the Menu and Game classes. Keep this class rather lightweight: it 
     *  controls the high level behaviour of your game. */
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;
using namespace utils;

    class Root: public Sprite
    {
        private: static  AssetManager* sAssets;

        private:  Sprite* mActiveScene;

        public:          Root();

        public: void     start(Texture* background, AssetManager* assets);

        private: void     onGameOver(Event* event, int score);

        private: void     onStartGame(Event* event, std::string gameMode);

        private: void     showScene(Class* screen);

        public: static AssetManager* assets();
    };

#endif // __SAMPLES_SCAFFOLD_MOBILE_SRC_ROOT_AS
#endif // __cplusplus

