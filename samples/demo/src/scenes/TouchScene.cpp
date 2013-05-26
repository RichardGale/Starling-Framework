

#include "TouchScene.h"
#include "starling/display/Image.h"
#include "starling/text/TextField.h"
#include "starling/utils/deg2rad.h"

#include "utils/TouchSheet.h"

using namespace starling::display;
using namespace starling::text;
using namespace starling::utils;
using namespace utils;

namespace scenes {


        TouchScene::TouchScene()
        {
            std::string description = "[use Ctrl//Cmd & Shift to simulate multi-touch]";

            TextField* infoText = new TextField(300, 25, description);
            infoText->x ( infoText->y ( 10));
            addChild(infoText);

            // to find out how to react to touch events have a look at the TouchSheet class! 
            // It's part of the demo.

            TouchSheet* sheet = new TouchSheet(new Image(Game()->assets()->getTexture("starling_sheet")));
            sheet->x ( Constants()->CenterX());
            sheet->y ( Constants()->CenterY());
            sheet->rotation ( deg2rad(10));
            addChild(sheet);
        }
}

