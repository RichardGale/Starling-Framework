

#include "MainMenu.h"
#include "flash/utils/getQualifiedClassName.h"

#include "scenes/AnimationScene.h"
#include "scenes/BenchmarkScene.h"
#include "scenes/BlendModeScene.h"
#include "scenes/CustomHitTestScene.h"
#include "scenes/FilterScene.h"
#include "scenes/MaskScene.h"
#include "scenes/MovieScene.h"
#include "scenes/RenderTextureScene.h"
#include "scenes/TextScene.h"
#include "scenes/TextureScene.h"
#include "scenes/TouchScene.h"

#include "starling/core/Starling.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/text/TextField.h"
#include "starling/textures/Texture.h"
#include "starling/utils/VAlign.h"

using namespace flash::utils;
using namespace scenes;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;



        MainMenu::MainMenu()
        {
            init();
        }

        void MainMenu::init()
        {
            Image* logo = new Image(Game()->assets()->getTexture("logo"));
            addChild(logo);

            std::vector<void*> scenesToCreate = [
                ["Textures", TextureScene],
                ["Multitouch", TouchScene],
                ["TextFields", TextScene],
                ["Animations", AnimationScene],
                ["Custom hit-test", CustomHitTestScene],
                ["Movie Clip", MovieScene],
                ["Filters", FilterScene],
                ["Blend Modes", BlendModeScene],
                ["Render Texture", RenderTextureScene],
                ["Benchmark", BenchmarkScene],
                ["Clipping", MaskScene]
            ];

            Texture* buttonTexture = Game()->assets()->getTexture("button_medium");
            int count = 0;

            for (std::vector<std::vector<void*>>::iterator sceneToCreate = scenesToCreate.begin(); sceneToCreate != scenesToCreate.end(); ++sceneToCreate)
            {
                std::string sceneTitle = sceneToCreate[0];
                Class* sceneClass  = sceneToCreate[1];

                Button* button = new Button(buttonTexture, sceneTitle);
                button->x ( count % 2 == 0 ? 28 : 167);
                button->y ( 155 + int(count / 2) * 46);
                button->name ( getQualifiedClassName(sceneClass));
                addChild(button);

                if (scenesToCreate.length % 2 != 0 && count % 2 == 1)
                    button->y() += 24;

                ++count;
            }

            // show information about rendering method (hardware/software)

            std::string driverInfo = Starling::context()->driverInfo;
            TextField* infoText = new TextField(310, 64, driverInfo, "Verdana", 10);
            infoText->x ( 5);
            infoText->y ( 475 - infoText->height());
            infoText->vAlign ( VAlign::BOTTOM);
            infoText->addEventListener(TouchEvent::TOUCH, onInfoTextTouched);
            addChildAt(infoText, 0);
        }

        void MainMenu::onInfoTextTouched(TouchEvent* event)
        {
            if (event->getTouch(this, TouchPhase::ENDED))
                Starling::current()->showStats ( !Starling::current()->showStats());
        }

