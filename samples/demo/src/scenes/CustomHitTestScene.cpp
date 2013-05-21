

#include "CustomHitTestScene.h"
#include "starling/text/TextField.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

#include "utils/RoundButton.h"

using namespace starling::text;
using namespace starling::utils;
using namespace utils;

namespace scenes {


        CustomHitTestScene::CustomHitTestScene()
        {
             std::string description=
                "Pushing the bird only works when the touch occurs within a circle." +
                "This can be accomplished by overriding the method 'hitTest'.";

             TextField* infoText= new TextField(300, 100, description);
            infoText->x ( infoText->y ( 10));
            infoText->vAlign ( VAlign::TOP);
            infoText->hAlign ( HAlign::CENTER);
            addChild(infoText);

            // 'RoundButton' is a helper class of the Demo, not a part of Starling!
            // Have a look at its code to understand this sample.

             RoundButton* button= new RoundButton(Game()->assets()->getTexture("starling_round"));
            button->x ( Constants()->CenterX() - int(button->width() / 2));
            button->y ( 150);
            addChild(button);
        }
}

