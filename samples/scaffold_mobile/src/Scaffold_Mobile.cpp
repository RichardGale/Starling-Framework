

#include "Scaffold_Mobile.h"
#include "flash/desktop/NativeApplication.h"
#include "flash/display/Bitmap.h"
#include "flash/display/Sprite.h"
#include "flash/events/Event.h"
#include "flash/filesystem/File.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"

#include "starling/core/Starling.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"
#include "starling/utils/AssetManager.h"
#include "starling/utils/RectangleUtil.h"
#include "starling/utils/ScaleMode.h"
#include "starling/utils/formatString.h"

using namespace flash::desktop;
using namespace flash::display;
using namespace flash::events;
using namespace flash::filesystem;
using namespace flash::geom;
using namespace flash::system;
using namespace starling::core;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;



        // We embed the "Ubuntu" font. Beware: the 'embedAsCFF'-part IS REQUIRED!!!

        const Class* Scaffold_Mobile::UbuntuRegular;

        // Startup image for SD screens

        Class* Scaffold_Mobile::Background;

        // Startup image for HD screens

        Class* Scaffold_Mobile::BackgroundHD;

                    

        Scaffold_Mobile::Scaffold_Mobile()
        {
            // set general properties

            int stageWidth   = Constants()->STAGE_WIDTH();
            int stageHeight  = Constants()->STAGE_HEIGHT();
            bool iOS = Capabilities::manufacturer()->indexOf("iOS") != -1;

            Starling::multitouchEnabled ( true); // useful on mobile devices
            Starling::handleLostContext ( !iOS); // not necessary on iOS. Saves a lot of memory!

            // create a suitable viewport for the screen size
            // 
            // we develop the game in a *fixed* coordinate system of 320x480; the game might 
            // then run on a device with a different resolution; for that case, we zoom the 
            // viewPort to the optimal size for any display and load the optimal textures.

            Rectangle* viewPort = RectangleUtil::fit(
                new Rectangle(0, 0, stageWidth, stageHeight),
                new Rectangle(0, 0, stage()->fullScreenWidth(), stage()->fullScreenHeight()),
                ScaleMode::SHOW_ALL);

            // create the AssetManager, which handles all required assets for this resolution

            int scaleFactor = viewPort->width < 480 ? 1 : 2; // midway between 320 and 640
            File* appDir = File::applicationDirectory;
            AssetManager* assets = new AssetManager(scaleFactor);

            assets->verbose ( Capabilities::isDebugger);
            assets->enqueue(
                appDir->resolvePath("audio"),
                appDir->resolvePath(formatString("fonts//{0}x", scaleFactor)),
                appDir->resolvePath(formatString("textures//{0}x", scaleFactor))
            );

            // While Stage3D is initializing, the screen will be blank. To avoid any flickering, 
            // we display a startup image now and remove it below, when Starling is ready to go.
            // This is especially useful on iOS, where "Default.png" (or a variant) is displayed
            // during Startup. You can create an absolute seamless startup that way.
            // 
            // These are the only embedded graphics in this app. We can't load them from disk,
            // because that can only be done asynchronously (resulting in a short flicker).
            // 
            // Note that we cannot embed "Default.png" (or its siblings), because any embedded
            // files will vanish from the application package, and those are picked up by the OS!

            Bitmap* background = scaleFactor == 1 ? new Background() : new BackgroundHD();
            Background = BackgroundHD = NULL; // no longer needed!

            background->x ( viewPort->x());
            background->y ( viewPort->y());
            background->width  ( viewPort->width());
            background->height ( viewPort->height());
            background->smoothing ( true);
            addChild(background);

            // launch Starling

            mStarling = new Starling(Root, stage, viewPort);
            mStarling->stage()->stageWidth  ( stageWidth); // <- same size on all devices!
            mStarling->stage()->stageHeight ( stageHeight);// <- same size on all devices!
            mStarling->simulateMultitouch  ( false);
            mStarling->enableErrorChecking ( Capabilities::isDebugger);

            mStarling->addEventListener(starling()->events()->Event::ROOT_CREATED,void Scaffold_Mobile::(Object* event, Root* app)
                {
                    mStarling->removeEventListener(starling()->events()->Event::ROOT_CREATED, onRootCreated);
                    removeChild(background);

                    Texture* bgTexture = Texture::fromBitmap(background, false, false, scaleFactor);

                    app->start(bgTexture, assets);
                    mStarling->start();
                });

            // When the game becomes inactive, we pause Starling; otherwise, the enter frame event
            // would report a very long 'passedTime' when the app is reactivated. 

            NativeApplication::nativeApplication()->addEventListener(
                flash()->events()->Event::ACTIVATE, function(void* e)void  { mStarling->start(); });

            NativeApplication::nativeApplication()->addEventListener(
                flash()->events()->Event::DEACTIVATE, function(void* e)void  { mStarling->stop(); });
        }

