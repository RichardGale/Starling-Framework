

#include "FlexUnitApplication.h"
#include "Array.h"

#include "flash/display/Sprite.h"

#include "flexunit/flexui/FlexUnitTestRunnerUIAS.h"

#include "tests/BlendModeTest.h"
#include "tests/ColorTest.h"
#include "tests/DelayedCallTest.h"
#include "tests/DisplayObjectContainerTest.h"
#include "tests/DisplayObjectTest.h"
#include "tests/EventTest.h"
#include "tests/JugglerTest.h"
#include "tests/MovieClipTest.h"
#include "tests/QuadTest.h"
#include "tests/RectangleUtilTest.h"
#include "tests/TextureAtlasTest.h"
#include "tests/TextureTest.h"
#include "tests/TweenTest.h"
#include "tests/UtilsTest.h"
#include "tests/VertexDataTest.h"

using namespace flash::display;
using namespace flexunit::flexui;
using namespace tests;



        FlexUnitApplication::FlexUnitApplication()
        {
            onCreationComplete();
        }

        void FlexUnitApplication::onCreationComplete()
        {
            FlexUnitTestRunnerUIAS* testRunner=new FlexUnitTestRunnerUIAS();
            testRunner->portNumber(8765);
            this->addChild(testRunner);
            testRunner->runWithFlexUnit4Runner(currentRunTestSuite(), "Starling-Tests");
        }

        std::vector<void*> FlexUnitApplication::currentRunTestSuite()
        {
            std::vector<void*> testsToRun = std::vector<void*>()           ;
            testsToRun.push_back(tests()->ColorTest);
            testsToRun.push_back(tests()->TextureAtlasTest);
            testsToRun.push_back(tests()->JugglerTest);
            testsToRun.push_back(tests()->QuadTest);
            testsToRun.push_back(tests()->DisplayObjectContainerTest);
            testsToRun.push_back(tests()->UtilsTest);
            testsToRun.push_back(tests()->DisplayObjectTest);
            testsToRun.push_back(tests()->BlendModeTest);
            testsToRun.push_back(tests()->MovieClipTest);
            testsToRun.push_back(tests()->RectangleUtilTest);
            testsToRun.push_back(tests()->VertexDataTest);
            testsToRun.push_back(tests()->EventTest);
            testsToRun.push_back(tests()->DelayedCallTest);
            testsToRun.push_back(tests()->TweenTest);
            testsToRun.push_back(tests()->TextureTest);
            return testsToRun;
        }

