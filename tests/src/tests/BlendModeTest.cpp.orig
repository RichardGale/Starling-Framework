// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "BlendModeTest.h"
#include "flash/display3D/Context3DBlendFactor.h"

#include "flexunit/framework/Assert.h"

#include "starling/display/BlendMode.h"

using namespace flash::display3D;
using namespace flexunit::framework;
using namespace starling::display;

namespace tests {



        void BlendModeTest::                testRegisterBlendMode()
        {
             std::string name="test";

            // register for pma = true; should set factors for both pma possibilities.

            BlendMode::REGISTER(name,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA,
                                     Context3DBlendFactor::DESTINATION_COLOR,true);

             std::vector<void*> modesPma=BlendMode::getBlendFactors(name,true);
             std::vector<void*> modesNoPma=BlendMode::getBlendFactors(name,false);

            Assert::assertEquals(Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA,modesPma[0]);
            Assert::assertEquals(Context3DBlendFactor::DESTINATION_COLOR,modesPma[1]);

            Assert::assertEquals(Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA,modesNoPma[0]);
            Assert::assertEquals(Context3DBlendFactor::DESTINATION_COLOR,modesNoPma[1]);

            // now overwrite for pma = false; should not change pma = true factors.

            BlendMode::REGISTER(name,Context3DBlendFactor::ONE,Context3DBlendFactor::ZERO,
                               false);

            modesPma = BlendMode::getBlendFactors(name,true);
            modesNoPma = BlendMode::getBlendFactors(name,false);

            Assert::assertEquals(Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA,modesPma[0]);
            Assert::assertEquals(Context3DBlendFactor::DESTINATION_COLOR,modesPma[1]);

            Assert::assertEquals(Context3DBlendFactor::ONE,modesNoPma[0]);
            Assert::assertEquals(Context3DBlendFactor::ZERO,modesNoPma[1]);
        }
}

