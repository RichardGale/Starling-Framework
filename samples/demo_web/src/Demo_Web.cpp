

#include "Demo_Web.h"
#include "flash/display/Sprite.h"
#include "flash/system/Capabilities.h"

#include "starling/core/Starling.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"
#include "starling/utils/AssetManager.h"

// If you set this class as your 'default application', it will run without a preloader.
// To use a preloader, see 'Demo_Web_Preloader.as'.


using namespace flash::display;
using namespace flash::system;
using namespace starling::core;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::utils;



Demo_Web::Demo_Web()
{
    if (stage) start();
    else addEventListener(Event::ADDED_TO_STAGE,onAddedToStage);
}

void Demo_Web::start()
{
    Starling::multitouchEnabled= true; // for Multitouch Scene
    Starling::handleLostContext= true; // required on Windows, needs more memory

    mStarling = new Starling(Game, stage);
    mStarling->simulateMultitouch= true;
    mStarling->enableErrorChecking= Capabilities::isDebugger;
    mStarling->start();

    // this event is dispatched when stage3D is set up
    mStarling->addEventListener(Event::ROOT_CREATED,onRootCreated);
}

void Demo_Web::onAddedToStage(Object *event)
{
    removeEventListener(Event::ADDED_TO_STAGE,onAddedToStage);
    start();
}

void Demo_Web::onRootCreated(Event *event, Game *game)
{
    // set framerate to 30 in software mode
    if (mStarling->context->driverInfo->toLowerCase()->indexOf("software")!=-1)
        mStarling->nativeStage->frameRate=30;

    // define which resources to load
    AssetManager *assets=new AssetManager();
    assets->verbose= Capabilities::isDebugger;
    assets->enqueue(EmbeddedAssets);

    // background texture is embedded, because we need it right away!
    Texture *bgTexture=Texture::fromBitmap(newBackground());

    // game will first load resources, then start menu
    game->start(bgTexture,assets);
}

