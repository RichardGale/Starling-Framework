

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
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;
using namespace tests;



        FlexUnitApplication::FlexUnitApplication()
        {
            onCreationComplete();
        }

        void FlexUnitApplication::onCreationComplete()
        {
             FlexUnitTestRunnerUIAS* testRunner=newFlexUnitTestRunnerUIAS();
            testRunner->portNumber=8765;
            this->addChild(testRunner);
            testRunner->runWithFlexUnit4Runner(currentRunTestSuite(),"Starling-Tests");
        }

        std::vector<void*> FlexUnitApplication::currentRunTestSuite()
        {
             std::vector<void*> testsToRun=newArray();
            testsToRun->push(tests->ColorTest);
            testsToRun->push(tests->TextureAtlasTest);
            testsToRun->push(tests->JugglerTest);
            testsToRun->push(tests->QuadTest);
            testsToRun->push(tests->DisplayObjectContainerTest);
            testsToRun->push(tests->UtilsTest);
            testsToRun->push(tests->DisplayObjectTest);
            testsToRun->push(tests->BlendModeTest);
            testsToRun->push(tests->MovieClipTest);
            testsToRun->push(tests->RectangleUtilTest);
            testsToRun->push(tests->VertexDataTest);
            testsToRun->push(tests->EventTest);
            testsToRun->push(tests->DelayedCallTest);
            testsToRun->push(tests->TweenTest);
            testsToRun->push(tests->TextureTest);
            return testsToRun;
        }

