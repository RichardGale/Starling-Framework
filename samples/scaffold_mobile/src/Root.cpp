

#include "Root.h"
#include "starling/core/Starling.h"
#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"
#include "starling/utils/AssetManager.h"

#include "utils/ProgressBar.h"

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



         AssetManager* Root::sAssets;

        Root::Root()
        {
            addEventListener(Menu->START_GAME,onStartGame);
            addEventListener(Game->GAME_OVER, onGameOver);

            // not more to do here -- Startup will call "start" immediately.
        }

        void Root::start(Texture* background, AssetManager* assets)
        {
            // the asset manager is saved as a static variable; this allows us to easily access
            // all the assets from everywhere by simply calling "Root.assets"
            sAssets = assets;

            // The background is passed into this method for two reasons:
            // 
            // 1) we need it right away, otherwise we have an empty frame
            // 2) the Startup class can decide on the right image, depending on the device.

            addChild(new Image(background));

            // The AssetManager contains all the raw asset data, but has not created the textures
            // yet. This takes some time (the assets might be loaded from disk or even via the
            // network), during which we display a progress indicator. 

             ProgressBar* progressBar=new ProgressBar(175, 20);
            progressBar->x= (background->width - progressBar->width) / 2;
            progressBar->y= (background->height- progressBar->height)/ 2;
            progressBar->y= background->height* 0.85;
            addChild(progressBar);

            assets->loadQueue(void Root::(float ratio)
            {
                progressBar->ratio= ratio;

                // a progress bar should always show the 100% for a while,
                // so we show the main menu only after a short delay. 

                if (ratio == 1)
                    Starling::juggler->delayCall(function()void
                    {
                        progressBar->removeFromParent(true);
                        showScene(Menu);
                    }, 0.15);
            });
        }

        void Root::onGameOver(Event* event, int score)
        {
            trace("Game Over! Score: " + score);
            showScene(Menu);
        }

        void Root::onStartGame(Event* event, std::string gameMode)
        {
            trace("Game starts! Mode: " + gameMode);
            showScene(Game);
        }

        void Root::showScene(Class* screen)
        {
            if (mActiveScene) mActiveScene->removeFromParent(true);
            mActiveScene = new screen();
            addChild(mActiveScene);
        }

        AssetManager* Root::assets()              { return sAssets; }

