

#include "BlendModeScene.h"
#include "starling/display/BlendMode.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/events/Event.h"
#include "starling/text/TextField.h"

using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;

namespace scenes
{









    BlendModeScene::BlendModeScene()
    {
        mButton = new Button(Game->assets->getTexture("button_normal"),"Switch Mode");
        mButton->x= int(Constants->CenterX- mButton->width/ 2);
        mButton->y= 15;
        mButton->addEventListener(Event::TRIGGERED,onButtonTriggered);
        addChild(mButton);

        mImage = new Image(Game->assets->getTexture("starling_rocket"));
        mImage->x= int(Constants->CenterX- mImage->width/ 2);
        mImage->y= 170;
        addChild(mImage);

        mInfoText = new TextField(300, 32, "", "Verdana", 19);
        mInfoText->x= 10;
        mInfoText->y= 330;
        addChild(mInfoText);

        onButtonTriggered();
    }

    void BlendModeScene::onButtonTriggered()
    {
        std::string blendMode=mBlendModes->shift()asString;
        mBlendModes->push(blendMode);

        mInfoText->text= blendMode;
        mImage->blendMode= blendMode;
    }
}

