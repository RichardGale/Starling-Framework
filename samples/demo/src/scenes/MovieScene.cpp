

#include "MovieScene.h"
#include "flash/media/Sound.h"

#include "starling/core/Starling.h"
#include "starling/display/MovieClip.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"

using namespace flash::media;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;

namespace scenes
{


    MovieScene::MovieScene()
    {
        std::vector<Texture *> *frames=Game->assets->getTextures("flight");
        mMovie = new MovieClip(frames, 15);

        // add sounds
        Sound *stepSound=Game->assets->getSound("wing_flap");
        mMovie->setFrameSound(2,stepSound);

        // move the clip to the center and add it to the stage
        mMovie->x= Constants->CenterX- int(mMovie->width/ 2);
        mMovie->y= Constants->CenterY- int(mMovie->height/ 2);
        addChild(mMovie);

        // like any animation, the movie needs to be added to the juggler!
        // this is the recommended way to do that.
        addEventListener(Event::ADDED_TO_STAGE,onAddedToStage);
        addEventListener(Event::REMOVED_FROM_STAGE,onRemovedFromStage);
    }

    void MovieScene::onAddedToStage()
    {
        Starling::juggler->add(mMovie);
    }

    void MovieScene::onRemovedFromStage()
    {
        Starling::juggler->remove(mMovie);
    }

    void MovieScene::dispose()
    {
        removeEventListener(Event::REMOVED_FROM_STAGE,onRemovedFromStage);
        removeEventListener(Event::ADDED_TO_STAGE,onAddedToStage);
        super->dispose();
    }
}

