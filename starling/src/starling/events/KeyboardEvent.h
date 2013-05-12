#if !defined(__STARLING_SRC_STARLING_EVENTS_KEYBOARDEVENT_AS)
#define __STARLING_SRC_STARLING_EVENTS_KEYBOARDEVENT_AS
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















namespace starling
{
    namespace events
    {
        class/** A KeyboardEvent is dispatched in response to user input through a keyboard.
     *
     *  <p>This is Starling's version of the Flash KeyboardEvent class. It contains the same
     *  properties as the Flash equivalent.</p>
     *
     *  <p>To be notified of keyboard events, add an event listener to the Starling stage. Children
     *  of the stage won't be notified of keybaord input. Starling has no concept of a "Focus"
     *  like native Flash.</p>
     *
     *  @see starling.display.Stage
     */          KeyboardEvent: public Event
        {
        /** Event type for a key that was released. */public:
            static const std::string KEY_UP;

            /** Event type for a key that was pressed. */
        public:
            static const std::string KEY_DOWN;

        private:
            unsigned int mCharCode;
        private:
            unsigned int mKeyCode;
        private:
            unsigned int mKeyLocation;
        private:
            bool mAltKey;
        private:
            bool mCtrlKey;
        private:
            bool mShiftKey;

            /** Creates a new KeyboardEvent. */
        public:
            KeyboardEvent(std::string type, unsigned int charCode=0, unsigned int keyCode=0,
                          unsigned int keyLocation=0, bool ctrlKey   =false,
                          bool altKey   =false, bool shiftKey   =false);

            /** Contains the character code of the key. */
        public:
            unsigned int charCode();

            /** The key code of the key. */
        public:
            unsigned int keyCode();

            /** Indicates the location of the key on the keyboard. This is useful for differentiating
             *  keys that appear more than once on a keyboard. @see Keylocation */
        public:
            unsigned int keyLocation();

            /** Indicates whether the Alt key is active on Windows or Linux;
             *  indicates whether the Option key is active on Mac OS. */
        public:
            bool         altKey();

            /** Indicates whether the Ctrl key is active on Windows or Linux;
             *  indicates whether either the Ctrl or the Command key is active on Mac OS. */
        public:
            bool         ctrlKey();

            /** Indicates whether the Shift key modifier is active (true) or inactive (false). */
        public:
            bool         shiftKey();
        };
    }
}

#endif // __STARLING_SRC_STARLING_EVENTS_KEYBOARDEVENT_AS
#endif // __cplusplus

