#if !defined(__STARLING_SRC_STARLING_CORE_TOUCHPROCESSOR_AS)
#define __STARLING_SRC_STARLING_CORE_TOUCHPROCESSOR_AS
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



#include "flex11.6.h"
namespace flash
{
    namespace geom
    {
        class Point;
    }
}
namespace flash
{
    namespace utils
    {
        class getDefinitionByName;
    }
}

namespace starling
{
    namespace display
    {
        class Stage;
    }
}
namespace starling
{
    namespace events
    {
        class KeyboardEvent;
    }
}
namespace starling
{
    namespace events
    {
        class Touch;
    }
}
namespace starling
{
    namespace events
    {
        class TouchEvent;
    }
}
namespace starling
{
    namespace events
    {
        class TouchPhase;
    }
}

namespace starling
{
    namespace core
    {
        class TouchMarker;
    }
}

//use namespace starling_internal;

/** @private
 *  The TouchProcessor is used internally to convert mouse and touch events of the conventional
 *  Flash stage to Starling's TouchEvents. */

using namespace flash::geom;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;

namespace starling
{
    namespace core
    {
        class TouchProcessor
        {
        private:
            static const float MULTITAP_TIME;
        private:
            static const float MULTITAP_DISTANCE;

        private:
            Stage *mStage;
        private:
            float mElapsedTime;
        private:
            TouchMarker *mTouchMarker;

        private:
            std::vector<Touch *> mCurrentTouches;
        private:
            std::vector<std::vector<void *>> mQueue;
        private:
            std::vector<Touch *> mLastTaps;

        private:
            bool mShiftDown;
        private:
            bool mCtrlDown;

            /** Helper objects. */
        private:
            static std::vector<int> sProcessedTouchIDs;
        private:
            static std::vector<Object *> sHoveringTouchData;

        public:
            TouchProcessor(Stage *stage);

        public:
            void     dispose();

        public:
            void     advanceTime(float passedTime);

        public:
            void     enqueue(int touchID, std::string phase, float globalX, float globalY,
                             float pressure=1.0, float width=1.0, float height=1.0);

        public:
            void     enqueueMouseLeftStage();    // On OS X, we get mouse events from outside the stage; on Windows, we do not.// This method enqueues an artifial hover point that is just outside the stage.// That way, objects listening for HOVERs over them will get notified everywhere.

        private:
            void     processTouch(int touchID, std::string phase, float globalX, float globalY,
                                  float pressure=1.0, float width=1.0, float height=1.0);

        private:
            void     onKey(KeyboardEvent *event);                                  // shift key

        private:
            void     processTap(Touch *touch);

        private:
            void     addCurrentTouch(Touch *touch);

        private:
            Touch    *getCurrentTouch(int touchID);

        public:
            bool      simulateMultitouch();
        public:
            void         simulateMultitouch(bool value);

            // interruption handling                     // no change

        private:
            void     monitorInterruptions(bool enable);

        private:
            void     onInterruption(Object *event);
        };
    }
}

#endif // __STARLING_SRC_STARLING_CORE_TOUCHPROCESSOR_AS
#endif // __cplusplus

