

#include "MaskScene.h"
#include "flash/geom/Point.h"
#include "flash/geom/Rectangle.h"

#include "starling/core/Starling.h"
#include "starling/display/Image.h"
#include "starling/display/Quad.h"
#include "starling/display/Sprite.h"
#include "starling/events/Touch.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/filters/ColorMatrixFilter.h"
#include "starling/text/TextField.h"

using namespace flash::geom;
using namespace flash::geom;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;
using namespace starling::filters;
using namespace starling::text;

namespace scenes
{


    MaskScene::MaskScene()
    {
        mContents = new Sprite();
        addChild(mContents);

        float stageWidth  = Starling::current->stage->stageWidth;
        float stageHeight = Starling::current->stage->stageHeight;

        Quad *touchQuad=new Quad(stageWidth, stageHeight);
        touchQuad->alpha= 0; // only used to get touch events
        addChildAt(touchQuad, 0);

        Image *image=new Image(Game->assets->getTexture("flight_00"));
        image->x= (stageWidth - image->width)/ 2;
        image->y= 80;
        mContents->addChild(image);

        // just to prove it works, use a filter on the image.
        ColorMatrixFilter *cm=new ColorMatrixFilter();
        cm->adjustHue(-0.5);
        image->filter= cm;

        TextField *scissorText=new TextField(256, 128,
                                             "Move the mouse (or a finger) over the screen to move the clipping rectangle.");
        scissorText->x= (stageWidth - scissorText->width)/ 2;
        scissorText->y= 240;
        mContents->addChild(scissorText);

        TextField *maskText=new TextField(256, 128,
                                          "Currently, Starling supports only stage-aligned clipping; more complex masks " +
                                          "will be supported in future versions.");
        maskText->x= scissorText->x;
        maskText->y= 290;
        mContents->addChild(maskText);

        Rectangle *scissorRect=new Rectangle(0, 0, 150, 150);
        scissorRect->x= (stageWidth  - scissorRect->width) / 2;
        scissorRect->y= (stageHeight - scissorRect->height)/ 2 + 5;
        mContents->clipRect= scissorRect;

        mClipQuad = new Quad(scissorRect->width,scissorRect->height,0xff0000);
        mClipQuad->x= scissorRect->x;
        mClipQuad->y= scissorRect->y;
        mClipQuad->alpha= 0.1;
        mClipQuad->touchable= false;
        addChild(mClipQuad);

        addEventListener(TouchEvent::TOUCH,onTouch);
    }

    void MaskScene::onTouch(TouchEvent *event)
    {
        Touch *touch=event->getTouch(this,TouchPhase::HOVER)||
                     event->getTouch(this,TouchPhase::BEGAN)||
                     event->getTouch(this,TouchPhase::MOVED);

        if (touch)
        {
            Point *localPos=touch->getLocation(this);
            Rectangle *clipRect=mContents->clipRect;
            clipRect->x= localPos->x- clipRect->width / 2;
            clipRect->y= localPos->y- clipRect->height/ 2;

            mClipQuad->x= clipRect->x;
            mClipQuad->y= clipRect->y;
        }
    }
}

