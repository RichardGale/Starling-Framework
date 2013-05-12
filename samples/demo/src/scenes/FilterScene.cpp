

#include "FilterScene.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/events/Event.h"
#include "starling/filters/BlurFilter.h"
#include "starling/filters/ColorMatrixFilter.h"
#include "starling/text/TextField.h"

using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::filters;
using namespace starling::text;

namespace scenes {


        FilterScene::FilterScene()
        {
            initFilters();

            mButton = new Button(Game->assets->getTexture("button_normal"),"Switch Filter");
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

        void FilterScene::onButtonTriggered()
        {
             std::vector<void*> filterInfo=mFilterInfos->shift()asArray;
            mFilterInfos->push(filterInfo);

            mInfoText->text= filterInfo[0];
            mImage->filter = filterInfo[1];
        }

        void FilterScene::initFilters()
        {
            mFilterInfos = [
                ["Identity", new ColorMatrixFilter()],
                ["Blur", new BlurFilter()],
                ["Drop Shadow", BlurFilter::createDropShadow()],
                ["Glow", BlurFilter::createGlow()]
            ];

             ColorMatrixFilter* invertFilter=new ColorMatrixFilter();
            invertFilter->invert();
            mFilterInfos->push(["Invert",invertFilter]);

             ColorMatrixFilter* grayscaleFilter=new ColorMatrixFilter();
            grayscaleFilter->adjustSaturation(-1);
            mFilterInfos->push(["Grayscale",grayscaleFilter]);

             ColorMatrixFilter* saturationFilter=new ColorMatrixFilter();
            saturationFilter->adjustSaturation(1);
            mFilterInfos->push(["Saturation",saturationFilter]);

             ColorMatrixFilter* contrastFilter=new ColorMatrixFilter();
            contrastFilter->adjustContrast(0.75);
            mFilterInfos->push(["Contrast",contrastFilter]);

             ColorMatrixFilter* brightnessFilter=new ColorMatrixFilter();
            brightnessFilter->adjustBrightness(-0.25);
            mFilterInfos->push(["Brightness",brightnessFilter]);

             ColorMatrixFilter* hueFilter=new ColorMatrixFilter();
            hueFilter->adjustHue(1);
            mFilterInfos->push(["Hue",hueFilter]);
        }
}

