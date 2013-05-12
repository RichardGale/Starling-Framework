#if !defined(__SAMPLES_SCAFFOLD_MOBILE_SRC_MENU_AS)
#define __SAMPLES_SCAFFOLD_MOBILE_SRC_MENU_AS
#if defined(__cplusplus)


#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling { namespace display { class Button; } }
#include "starling/display/Sprite.h"
namespace starling { namespace events { class Event; } }
namespace starling { namespace text { class BitmapFont; } }
namespace starling { namespace text { class TextField; } }

    /** The Menu shows the logo of the game and a start button that will, once triggered, 
     *  start the actual game. In a real game, it will probably contain several buttons and
     *  link to several screens (e.g. a settings screen or the credits). If your menu contains
     *  a lot of logic, you could use the "Feathers" library to make your life easier. */
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::text;

    class Menu: public Sprite
    {
        public: static const std::string START_GAME;

        public:          Menu();

        private: void     init();

        private: void     onButtonTriggered();
    };

#endif // __SAMPLES_SCAFFOLD_MOBILE_SRC_MENU_AS
#endif // __cplusplus

