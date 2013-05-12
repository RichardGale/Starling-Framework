#if !defined(__TESTS_SRC_TESTS_MOVIECLIPTEST_AS)
#define __TESTS_SRC_TESTS_MOVIECLIPTEST_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace display3D { class Context3DTextureFormat; } }

namespace flexunit { namespace framework { class Assert; } }

namespace org { namespace flexunit { class assertThat; } }
namespace org { namespace flexunit { namespace asserts { class assertEquals; } } }
namespace org { namespace flexunit { namespace asserts { class assertFalse; } } }
namespace org { namespace hamcrest { namespace number { class closeTo; } } }

namespace starling { namespace display { class MovieClip; } }
namespace starling { namespace events { class Event; } }
namespace starling { namespace textures { class ConcreteTexture; } }
namespace starling { namespace textures { class Texture; } }

using namespace flash::display3D;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::textures;

namespace tests {
    class MovieClipTest
    {
        private: const float E;


        public: void 
        ////[Test]      testFrameManipulation();


        public: void 
        ////[Test]      testAdvanceTime();


        public: void 
        ////[Test]      testChangeFps();


        public: void 
        ////[Test]      testCompletedEvent();


        public: void 
        ////[Test]      testChangeCurrentFrameInCompletedEvent();


        public: void 
        ////[Test]      testRemoveAllFrames();


        public: void 
        ////[Test]      testLastTextureInFastPlayback();


        public: void 
        ////[Test]      testAssignedTextureWithCompleteHandler();


        public: void 
        ////[Test]      testStopMovieInCompleteHandler();

        private: std::vector<Texture*>* createFrames(int count);
    };
}

#endif // __TESTS_SRC_TESTS_MOVIECLIPTEST_AS
#endif // __cplusplus

