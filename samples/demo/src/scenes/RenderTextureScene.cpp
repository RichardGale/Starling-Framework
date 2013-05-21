

#include "RenderTextureScene.h"
#include "flash/geom/Point.h"
#include "flash/utils/Dictionary.h"

#include "starling/display/BlendMode.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/events/Event.h"
#include "starling/events/Touch.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/text/TextField.h"
#include "starling/textures/RenderTexture.h"

using namespace flash::geom;
using namespace flash::utils;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;

namespace scenes {


                    
                    
                    
                    
                    

        RenderTextureScene::RenderTextureScene()
        {
            mColors.clear();
            mRenderTexture = new RenderTexture(320, 435);

            mCanvas = new Image(mRenderTexture);
            mCanvas->addEventListener(TouchEvent::TOUCH, onTouch);
            addChild(mCanvas);

            mBrush = new Image(Game()->assets()->getTexture("brush"));
            mBrush->pivotX ( mBrush->width() / 2);
            mBrush->pivotY ( mBrush->height() / 2);
            mBrush->blendMode ( BlendMode::NORMAL);

             TextField* infoText= new TextField(256, 128, "Touch the screen\\nto draw!");
            infoText->fontSize ( 24);
            infoText->x ( Constants()->CenterX() - infoText->width() / 2);
            infoText->y ( Constants()->CenterY() - infoText->height() / 2);
            mRenderTexture->draw(infoText);

            mButton = new Button(Game()->assets()->getTexture("button_normal"), "Mode: Draw");
            mButton->x ( int(Constants()->CenterX() - mButton->width() / 2));
            mButton->y ( 15);
            mButton->addEventListener(Event::TRIGGERED, onButtonTriggered);
            addChild(mButton);
        }

        void RenderTextureScene::onTouch(TouchEvent* event)
        {
            // touching the canvas will draw a brush texture. The 'drawBundled' method is not
            // strictly necessary, but it's faster when you are drawing with several fingers
            // simultaneously.

            mRenderTexture->drawBundled(function()void
            {
                 std::vector<Touch*> touches=event->getTouches(mCanvas);

                for each (var Touch* touchin touches)
                {
                    if (touch->phase() == TouchPhase::BEGAN)
                        mColors[touch->id()] = Math::random() * uint()->MAX_VALUE();

                    if (touch->phase() == TouchPhase::HOVER() || touch->phase() == TouchPhase::ENDED)
                        continue;

                     Point* location= touch->getLocation(mCanvas);
                    mBrush->x ( location->x());
                    mBrush->y ( location->y());
                    mBrush->color ( mColors[touch->id()]);
                    mBrush->rotation ( Math::random() * Math::PI() * 2.0);

                    mRenderTexture->draw(mBrush);
                }
            });
        }

        void RenderTextureScene::onButtonTriggered()
        {
            if (mBrush->blendMode() == BlendMode::NORMAL)
            {
                mBrush->blendMode ( BlendMode::ERASE);
                mButton->text ( "Mode: Erase");
            }
            else
            {
                mBrush->blendMode ( BlendMode::NORMAL);
                mButton->text ( "Mode: Draw");
            }
        }

        void RenderTextureScene::dispose()
        {
            mRenderTexture->dispose();
            super()->dispose();
        }
}

