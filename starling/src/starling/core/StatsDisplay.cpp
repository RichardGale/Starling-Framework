// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "StatsDisplay.h"
#include "flash/system/System.h"

#include "starling/display/BlendMode.h"
#include "starling/display/Quad.h"
#include "starling/display/Sprite.h"
#include "starling/events/EnterFrameEvent.h"
#include "starling/events/Event.h"
#include "starling/text/BitmapFont.h"
#include "starling/text/TextField.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

    /** A small, lightweight box that displays the current framerate, memory consumption and
     *  the number of draw calls per frame. The display is updated automatically once per frame. */
using namespace flash::system;
using namespace starling::display;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::text;
using namespace starling::text;
using namespace starling::utils;
using namespace starling::utils;

namespace starling {
namespace core {


        /** Creates a new Statistics Box. */
        StatsDisplay::StatsDisplay()
        {
            mBackground = new Quad(50, 25, 0x0);
            mTextField = new TextField(48, 25, "", BitmapFont::MINI,BitmapFont::NATIVE_SIZE,0xffffff);
            mTextField->x= 2;
            mTextField->hAlign= HAlign::LEFT;
            mTextField->vAlign= VAlign::TOP;

            addChild(mBackground);
            addChild(mTextField);

            blendMode = BlendMode::NONE;

            addEventListener(Event::ADDED_TO_STAGE,onAddedToStage);
            addEventListener(Event::REMOVED_FROM_STAGE,onRemovedFromStage);
        }

        void StatsDisplay::onAddedToStage()
        {
            addEventListener(Event::ENTER_FRAME,onEnterFrame);
            mTotalTime = mFrameCount = 0;
            update();
        }

        void StatsDisplay::onRemovedFromStage()
        {
            removeEventListener(Event::ENTER_FRAME,onEnterFrame);
        }

        void StatsDisplay::onEnterFrame(EnterFrameEvent* event)
        {
            mTotalTime += event->passedTime;
            mFrameCount++;

            if (mTotalTime > 1.0)
            {
                update();
                mFrameCount = mTotalTime = 0;
            }
        }

        /** Updates the displayed values. */
        void StatsDisplay::update()
        {
            mFps = mTotalTime > 0 ? mFrameCount / mTotalTime : 0;
            mMemory = System::totalMemory* 0.000000954; // 1.0 / (1024*1024) to convert to MB

            mTextField->text= "FPS: " + mFps->toFixed(mFps< 100 ? 1 : 0) +
                            "\nMEM: " + mMemory->toFixed(mMemory< 100 ? 1 : 0) +
                            "\nDRW: " + Math::max(0,mDrawCount - 2); // ignore self 
        }

        /** The number of Stage3D draw calls per second. */
        int StatsDisplay::drawCount()     { return mDrawCount; }
        void StatsDisplay::drawCount(int value)      { mDrawCount = value; }

        /** The current frames per second (updated once per second). */
        float StatsDisplay::fps()        { return mFps; }
        void StatsDisplay::fps(float value)      { mFps = value; }

        /** The currently required system memory in MB. */
        float StatsDisplay::memory()        { return mMemory; }
        void StatsDisplay::memory(float value)      { mMemory = value; }
}
}

