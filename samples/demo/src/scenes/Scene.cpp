

#include "Scene.h"
#include "starling/display/Button.h"
#include "starling/display/Sprite.h"

using namespace starling::display;
using namespace starling::display;

namespace scenes
{


    Scene::Scene()
    {
        // the main menu listens for TRIGGERED events, so we just need to add the button.
        // (the event will bubble up when it's dispatched.)

        mBackButton = new Button(Game->assets->getTexture("button_back"),"Back");
        mBackButton->x= Constants->CenterX- mBackButton->width/ 2;
        mBackButton->y= Constants->GameHeight- mBackButton->height+ 1;
        mBackButton->name= "backButton";
        addChild(mBackButton);
    }
}

