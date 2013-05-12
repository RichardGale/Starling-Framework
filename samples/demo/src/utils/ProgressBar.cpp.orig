

#include "ProgressBar.h"
#include "flash/display/BitmapData.h"
#include "flash/display/Shape.h"

#include "starling/core/Starling.h"
#include "starling/display/Image.h"
#include "starling/display/Quad.h"
#include "starling/display/Sprite.h"
#include "starling/textures/Texture.h"

using namespace flash::display;
using namespace flash::display;
using namespace starling::core;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::textures;

namespace utils {


        ProgressBar::ProgressBar(int width, int height)
        {
            init(width, height);
        }

        void ProgressBar::init(int width, int height)
        {
             float scale = Starling::contentScaleFactor;
             float padding = height * 0.2;
             float cornerRadius = padding * scale * 2;

            // create black rounded box for background

             Shape* bgShape=new Shape();
            bgShape->graphics->beginFill(0x0,0.5);
            bgShape->graphics->drawRoundRect(0,0,width*scale, height*scale, cornerRadius, cornerRadius);
            bgShape->graphics->endFill();

             BitmapData* bgBitmapData=new BitmapData(width*scale, height*scale, true, 0x0);
            bgBitmapData->draw(bgShape);
             Texture* bgTexture=Texture::fromBitmapData(bgBitmapData,false, false, scale);

            mBackground = new Image(bgTexture);
            addChild(mBackground);

            // create progress bar quad

            mBar = new Quad(width - 2*padding, height - 2*padding, 0xeeeeee);
            mBar->setVertexColor(2,0xaaaaaa);
            mBar->setVertexColor(3,0xaaaaaa);
            mBar->x= padding;
            mBar->y= padding;
            mBar->scaleX= 0;
            addChild(mBar);
        }

        float ProgressBar::ratio()        { return mBar->scaleX;}
        void ProgressBar::ratio(float value)
        {
            mBar->scaleX= Math::max(0.0,Math::min(1.0,value));
        }
}

