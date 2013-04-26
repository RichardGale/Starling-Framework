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

//use starling_internal        ;

/** @private
 *  The TouchProcessor is used internally to convert mouse and touch events of the conventional
 *  Flash stage to Starling's TouchEvents. */
using namespace flash::geom;
using namespace flash::utils;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;
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
            std::vector<Touch *> *mCurrentTouches;
        private:
            std::vector<std::vector<void *>> *mQueue;
        private:
            std::vector<Touch *> *mLastTaps;

        private:
            bool mShiftDown;
        private:
            bool mCtrlDown;

            /** Helper objects. */
        private:
            static std::vector<int> *sProcessedTouchIDs;
        private:
            static std::vector<Object *> *sHoveringTouchData;

        public:
            TouchProcessor(Stage *stage);

        public:
            void     dispose();

        public:
            void     advanceTime(float passedTime);

        public:
            void     enqueue(int touchID, std::string phase, float globalX, float globalY,
                             float pressure, float width, float height);

        public:
            void     enqueueMouseLeftStage();    // That way, objects listening for HOVERs over them will get notified everywhere.

        private:
            void     processTouch(int touchID, std::string phase, float globalX, float globalY,
                                  float pressure, float width, float height);

        private:
            void     onKey(KeyboardEvent *event);                                  // shift key

        private:
            void     processTap(Touch *touch);

        private:
            void     addCurrentTouch(Touch *touch);

        private:
            Touch   *getCurrentTouch(int touchID);

        public:
            bool         simulateMultitouch();
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

