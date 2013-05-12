

#include "Menu.h"
#include "starling/display/Button.h"
#include "starling/display/Sprite.h"
#include "starling/events/Event.h"
#include "starling/text/BitmapFont.h"
#include "starling/text/TextField.h"

    /** The Menu shows the logo of the game and a start button that will, once triggered, 
     *  start the actual game. In a real game, it will probably contain several buttons and
     *  link to several screens (e.g. a settings screen or the credits). If your menu contains
     *  a lot of logic, you could use the "Feathers" library to make your life easier. */
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::text;



        const std::string Menu::START_GAME="startGame";

        Menu::Menu()
        {
            init();
        }

        void Menu::init()
        {
             TextField* textField=new TextField(250, 50, "Game Scaffold",
                "Desyrel", BitmapFont::NATIVE_SIZE,0xffffff);
            textField->x= (Constants->STAGE_WIDTH- textField->width)/ 2;
            textField->y= 50;
            addChild(textField);

             Button* button=new Button(Root->assets->getTexture("button_normal"),"Start");
            button->fontName= "Ubuntu";
            button->fontSize= 16;
            button->x= int((Constants->STAGE_WIDTH- button->width)/ 2);
            button->y= Constants->STAGE_HEIGHT* 0.75;
            button->addEventListener(Event::TRIGGERED,onButtonTriggered);
            addChild(button);
        }

        void Menu::onButtonTriggered()
        {
            dispatchEventWith(START_GAME, true, "classic");
        }

