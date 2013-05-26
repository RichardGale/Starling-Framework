

#include "Game.h"
#include "flash/ui/Keyboard.h"
#include "flash/utils/getDefinitionByName.h"

#include "scenes/Scene.h"

#include "starling/core/Starling.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/events/Event.h"
#include "starling/events/KeyboardEvent.h"
#include "starling/textures/Texture.h"
#include "starling/utils/AssetManager.h"

#include "utils/ProgressBar.h"

using namespace flash::ui;
using namespace flash::utils;
using namespace scenes;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;
using namespace utils;



        // Embed the Ubuntu Font. Beware: the 'embedAsCFF'-part IS REQUIRED!!!

        const Class* Game::UbuntuRegular;

                    
                    
                    

        AssetManager* Game::sAssets;

        Game::Game()
        {
            // nothing to do here -- Startup will call "start" immediately.
        }

        void Game::start(Texture* background, AssetManager* assets)
        {
            sAssets = assets;

            // The background is passed into this method for two reasons:
            // 
            // 1) we need it right away, otherwise we have an empty frame
            // 2) the Startup class can decide on the right image, depending on the device.

            addChild(new Image(background));

            // The AssetManager contains all the raw asset data, but has not created the textures
            // yet. This takes some time (the assets might be loaded from disk or even via the
            // network), during which we display a progress indicator. 

            mLoadingProgress = new ProgressBar(175, 20);
            mLoadingProgress->x ( (background->width()  - mLoadingProgress->width()) / 2);
            mLoadingProgress->y ( background->height() * 0.7);
            addChild(mLoadingProgress);

            assets->loadQueue(function(float ratio)void
            {
                mLoadingProgress->ratio ( ratio);

                // a progress bar should always show the 100% for a while,
                // so we show the main menu only after a short delay. 

                if (ratio == 1)
                    Starling::juggler()->delayCall(function()void
                    {
                        mLoadingProgress->removeFromParent(true);
                        mLoadingProgress = NULL;
                        showMainMenu();
                    }, 0.15);
            });

            addEventListener(Event::TRIGGERED, onButtonTriggered);
            stage()->addEventListener(KeyboardEvent::KEY_DOWN, onKey);
        }

        void Game::showMainMenu()
        {
            if (mMainMenu == NULL)
                mMainMenu = new MainMenu();

            addChild(mMainMenu);
        }

        void Game::onKey(KeyboardEvent* event)
        {
            if (event->keyCode() == Keyboard::SPACE)
                Starling::current()->showStats ( !Starling::current()->showStats());
            else if (event->keyCode() == Keyboard::X)
                Starling::context()->dispose();
        }

        void Game::onButtonTriggered(Event* event)
        {
            Button* button = event->dynamic_cast<Button*>(target);

            if (button->name() == "backButton")
                closeScene();
            else
                showScene(button->name());
        }

        void Game::closeScene()
        {
            mCurrentScene->removeFromParent(true);
            mCurrentScene = NULL;
            showMainMenu();
        }

        void Game::showScene(std::string name)
        {
            if (mCurrentScene) return;

            Class* sceneClass = getDefinitionByName(name) as Class;
            mCurrentScene = new sceneClass() as Scene;
            mMainMenu->removeFromParent();
            addChild(mCurrentScene);
        }

        AssetManager* Game::assets()              { return sAssets; }

