

#include "BenchmarkScene.h"
#include "flash/system/System.h"

#include "starling/core/Starling.h"
#include "starling/display/Button.h"
#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/events/EnterFrameEvent.h"
#include "starling/events/Event.h"
#include "starling/text/TextField.h"
#include "starling/utils/formatString.h"

using namespace flash::system;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::utils;

namespace scenes {


                    
                    

                    
                    
                    
                    
                    
                    

        BenchmarkScene::BenchmarkScene()
        {
            Scene();

            // the container will hold all test objects
            mContainer = new Sprite();
            mContainer->touchable ( false);// we do not need touch events on the test objects -- 
                                          // thus, it is more efficient to disable them.
            addChildAt(mContainer, 0);

            mStartButton = new Button(Game()->assets()->getTexture("button_normal"), "Start benchmark");
            mStartButton->addEventListener(Event::TRIGGERED, onStartButtonTriggered);
            mStartButton->x ( Constants()->CenterX() - int(mStartButton->width() / 2));
            mStartButton->y ( 20);
            addChild(mStartButton);

            mStarted = false;
            mElapsed = 0.0;

            addEventListener(Event::ENTER_FRAME, onEnterFrame);
        }

        void BenchmarkScene::dispose()
        {
            removeEventListener(Event::ENTER_FRAME, onEnterFrame);
            mStartButton->removeEventListener(Event::TRIGGERED, onStartButtonTriggered);
            Scene::dispose();
        }

        void BenchmarkScene::onEnterFrame(EnterFrameEvent* event)
        {
            if (!mStarted) return;

            mElapsed += event->passedTime();
            mFrameCount++;

            if (mFrameCount % mWaitFrames == 0)
            {
                float fps = mWaitFrames / mElapsed;
                int targetFps = Starling::current()->nativeStage()->frameRate();

                if (Math::ceil(fps) >= targetFps)
                {
                    mFailCount = 0;
                    addTestObjects();
                }
                else
                {
                    mFailCount++;

                    if (mFailCount > 20)
                        mWaitFrames = 5; // slow down creation process to be more exact
                    if (mFailCount > 30)
                        mWaitFrames = 10;
                    if (mFailCount == 40)
                        benchmarkComplete(); // target fps not reached for a while
                }

                mElapsed = mFrameCount = 0;
            }

            int numObjects = mContainer->numChildren();
            float passedTime = event->passedTime;

            for (int i=0; i<numObjects; ++i)
                mContainer->getChildAt(i)->rotation() += Math::PI() / 2 * passedTime;
        }

        void BenchmarkScene::onStartButtonTriggered()
        {
            trace("Starting benchmark");

            mStartButton->visible ( false);
            mStarted = true;
            mFailCount = 0;
            mWaitFrames = 2;
            mFrameCount = 0;

            if (mResultText)
            {
                mResultText->removeFromParent(true);
                mResultText = NULL;
            }

            addTestObjects();
        }

        void BenchmarkScene::addTestObjects()
        {
            int padding = 15;
            int numObjects = mFailCount > 20 ? 2 : 10;

            for (int i = 0; i<numObjects; ++i)
            {
                Image* egg = new Image(Game()->assets()->getTexture("benchmark_object"));
                egg->x ( padding + Math::random() * (Constants()->GameWidth() - 2 * padding));
                egg->y ( padding + Math::random() * (Constants()->GameHeight() - 2 * padding));
                mContainer->addChild(egg);
            }
        }

        void BenchmarkScene::benchmarkComplete()
        {
            mStarted = false;
            mStartButton->visible ( true);

            int fps = Starling::current()->nativeStage()->frameRate();

            trace("Benchmark complete!");
            trace("FPS: " + fps);
            trace("Number of objects: " + mContainer->numChildren());

            std::string resultString = formatString("Result:\\n{0} objects\\nwith {1} fps",
                                                   mContainer->numChildren(), fps);
            mResultText = new TextField(240, 200, resultString);
            mResultText->fontSize ( 30);
            mResultText->x ( Constants()->CenterX() - mResultText->width() / 2);
            mResultText->y ( Constants()->CenterY() - mResultText->height() / 2);

            addChild(mResultText);

            mContainer->removeChildren();
            System::pauseForGCIfCollectionImminent();
        }
}

