// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "KeyboardEvent.h"
#include "starling/events/Event.h"

    /** A KeyboardEvent is dispatched in response to user input through a keyboard.
     * 
     *  <p>This is Starling's version of the Flash KeyboardEvent class. It contains the same 
     *  properties as the Flash equivalent.</p> 
     * 
     *  <p>To be notified of keyboard events, add an event listener to the Starling stage. Children
     *  of the stage won't be notified of keybaord input. Starling has no concept of a "Focus"
     *  like native Flash.</p>
     *  
     *  @see starling.display.Stage
     */

using namespace starling::events;

namespace starling {
namespace events {


        /** Event type for a key that was released. */
        const std::string KeyboardEvent::KEY_UP = "keyUp";

        /** Event type for a key that was pressed. */
        const std::string KeyboardEvent::KEY_DOWN = "keyDown";

                    
                    
                    
                    
                    
                    

        /** Creates a new KeyboardEvent. */
        KeyboardEvent::KeyboardEvent(std::string type, unsigned int charCode, unsigned int keyCode,
                                      unsigned int keyLocation, bool ctrlKey,
                                      bool altKey, bool shiftKey)
        {
            Event(type, false, keyCode);
            mCharCode = charCode;
            mKeyCode = keyCode;
            mKeyLocation = keyLocation;
            mCtrlKey = ctrlKey;
            mAltKey = altKey;
            mShiftKey = shiftKey;
        }

        /** Contains the character code of the key. */
        unsigned int KeyboardEvent::charCode()      { return mCharCode; }

        /** The key code of the key. */
        unsigned int KeyboardEvent::keyCode()      { return mKeyCode; }

        /** Indicates the location of the key on the keyboard. This is useful for differentiating 
         *  keys that appear more than once on a keyboard. @see Keylocation */
        unsigned int KeyboardEvent::keyLocation()      { return mKeyLocation; }

        /** Indicates whether the Alt key is active on Windows or Linux; 
         *  indicates whether the Option key is active on Mac OS. */
        bool KeyboardEvent::altKey()         { return mAltKey; }

        /** Indicates whether the Ctrl key is active on Windows or Linux; 
         *  indicates whether either the Ctrl or the Command key is active on Mac OS. */
        bool KeyboardEvent::ctrlKey()         { return mCtrlKey; }

        /** Indicates whether the Shift key modifier is active (true) or inactive (false). */
        bool KeyboardEvent::shiftKey()         { return mShiftKey; }
}
}

