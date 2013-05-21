#if !defined(__STARLING_SRC_STARLING_CORE_STATSDISPLAY_AS)
#define __STARLING_SRC_STARLING_CORE_STATSDISPLAY_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "flex11.6.h"
namespace flash
{
    namespace system
    {
        class System;
    }
}

namespace starling
{
    namespace display
    {
        class BlendMode;
    }
}
namespace starling
{
    namespace display
    {
        class Quad;
    }
}
#include "starling/display/Sprite.h"
namespace starling
{
    namespace events
    {
        class EnterFrameEvent;
    }
}
namespace starling
{
    namespace events
    {
        class Event;
    }
}
namespace starling
{
    namespace text
    {
        class BitmapFont;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}
namespace starling
{
    namespace utils
    {
        class HAlign;
    }
}
namespace starling
{
    namespace utils
    {
        class VAlign;
    }
}

/** A small, lightweight box that displays the current framerate, memory consumption and
 *  the number of draw calls per frame. The display is updated automatically once per frame. */

using namespace flash::system;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::utils;

namespace starling
{
    namespace core
    {
        class StatsDisplay : public starling::display::Sprite
        {
        private:
            Quad *mBackground;
        private:
            TextField *mTextField;

        private:
            int mFrameCount;
        private:
            float mTotalTime;

        private:
            float mFps;
        private:
            float mMemory;
        private:
            int mDrawCount;

            /** Creates a new Statistics Box. */
        public:
            StatsDisplay();

        private:
            void     onAddedToStage();

        private:
            void     onRemovedFromStage();

        private:
            void     onEnterFrame(EnterFrameEvent *event);

            /** Updates the displayed values. */
        public:
            void     update();

            /** The number of Stage3D draw calls per second. */
        public:
            int          drawCount();
        public:
            void         drawCount(int value);

            /** The current frames per second (updated once per second). */
        public:
            float        fps();
        public:
            void         fps(float value);

            /** The currently required system memory in MB. */
        public:
            float        memory();
        public:
            void         memory(float value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_CORE_STATSDISPLAY_AS
#endif // __cplusplus

