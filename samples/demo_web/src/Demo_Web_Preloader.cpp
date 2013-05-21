

#include "Demo_Web_Preloader.h"
#include "flash/display/DisplayObject.h"
#include "flash/display/MovieClip.h"
#include "flash/display/Shape.h"
#include "flash/display/StageAlign.h"
#include "flash/display/StageScaleMode.h"
#include "flash/events/Event.h"
#include "flash/utils/getDefinitionByName.h"

#include "starling/utils/Color.h"

    // To show a Preloader while the SWF is being transferred from the server, 
    // set this class as your 'default application' and add the following 
    // compiler argument: '-frame StartupFrame Demo_Web'

using namespace flash::display;
using namespace flash::events;
using namespace flash::utils;
using namespace starling::utils;



                      

                    
                    

        Demo_Web_Preloader::Demo_Web_Preloader()
        {
            addEventListener(Event::ADDED_TO_STAGE, onAddedToStage);
            stop();
        }

        void Demo_Web_Preloader::onAddedToStage(Event* event)
        {
            stage()->scaleMode ( StageScaleMode::SHOW_ALL);
            stage()->align ( StageAlign::TOP_LEFT);

            addEventListener(Event::ENTER_FRAME, onEnterFrame);
            removeEventListener(Event::ADDED_TO_STAGE, onAddedToStage);
        }

        void Demo_Web_Preloader::onEnterFrame(Event* event)
        {
             int bytesLoaded = root()->loaderInfo()->bytesLoaded;
             int bytesTotal  = root()->loaderInfo()->bytesTotal;

            if (bytesLoaded >= bytesTotal)
            {
                dispose();
                run();
            }
            else
            {
                if (mProgressIndicator == NULL)
                {
                    mProgressIndicator = createProgressIndicator();
                    mProgressIndicator->x ( stage()->stageWidth()  / 2);
                    mProgressIndicator->y ( stage()->stageHeight() / 2);
                    addChild(mProgressIndicator);
                }
                else
                {
                    if (mFrameCount++ % 5 == 0)
                        mProgressIndicator->rotation() += 45;
                }
            }
        }

        Shape* Demo_Web_Preloader::createProgressIndicator(float radius, int elements)
        {
             Shape* shape= new Shape();
             float angleDelta  = Math::PI() * 2 / elements;
             float x , float y ;
             float innerRadius  = radius / 4;
             unsigned int color;

            for ( int i=0; i<elements; ++i)
            {
                x = Math::cos(angleDelta * i) * radius;
                y = Math::sin(angleDelta * i) * radius;
                color = (i+1) / elements * 255;

                shape->graphics()->beginFill(Color::rgb(color, color, color));
                shape->graphics()->drawCircle(x, y, innerRadius);
                shape->graphics()->endFill();
            }

            return shape;
        }

        void Demo_Web_Preloader::dispose()
        {
            removeEventListener(Event::ENTER_FRAME, onEnterFrame);

            if (mProgressIndicator)
                removeChild(mProgressIndicator);

            mProgressIndicator = NULL;
        }

        void Demo_Web_Preloader::run()
        {
            nextFrame();

             Class* startupClass= getDefinitionByName(STARTUP_CLASS) as Class;
            if (startupClass == NULL)
                throw new Error("Invalid Startup class in Preloader: " + STARTUP_CLASS);

             DisplayObject* startupObject= new startupClass() as DisplayObject;
            if (startupObject == NULL)
                throw new Error("Startup class needs to inherit from Sprite or MovieClip.");

            addChildAt(startupObject, 0);
        }

