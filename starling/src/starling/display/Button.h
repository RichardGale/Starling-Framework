#if !defined(__STARLING_SRC_STARLING_DISPLAY_BUTTON_AS)
#define __STARLING_SRC_STARLING_DISPLAY_BUTTON_AS
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
        class Rectangle;
    }
}
namespace flash
{
    namespace ui
    {
        class Mouse;
    }
}
namespace flash
{
    namespace ui
    {
        class MouseCursor;
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
    namespace text
    {
        class TextField;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
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

#include "starling/display/DisplayObjectContainer.h"
namespace starling
{
    namespace display
    {
        class Sprite;
    }
}
namespace starling
{
    namespace display
    {
        class Image;
    }
}

/** Dispatched when the user triggers the button. Bubbles. */
//[Event(name="triggered",type="starling.events.Event")]

using namespace flash::geom;
using namespace flash::ui;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

namespace starling
{
    namespace display
    {
        /** A simple button composed of an image and, optionally, text.
         *
         *  <p>You can pass a texture for up- and downstate of the button. If you do not provide a down
         *  state, the button is simply scaled a little when it is touched.
         *  In addition, you can overlay a text on the button. To customize the text, almost the
         *  same options as those of text fields are provided. In addition, you can move the text to a
         *  certain position with the help of the <code>textBounds</code> property.</p>
         *
         *  <p>To react on touches on a button, there is special <code>triggered</code>-event type. Use
         *  this event instead of normal touch events - that way, users can cancel button activation
         *  by moving the mouse/finger away from the button before releasing.</p>
         */
        class Button : public starling::display::DisplayObjectContainer
        {
        private:
            static const float MAX_DRAG_DIST;

        private:
            Texture *mUpState;
        private:
            Texture *mDownState;

        private:
            Sprite *mContents;
        private:
            Image *mBackground;
        private:
            TextField *mTextField;
        private:
            Rectangle *mTextBounds;

        private:
            float mScaleWhenDown;
        private:
            float mAlphaWhenDisabled;
        private:
            bool mEnabled;
        private:
            bool mIsDown;
        private:
            bool mUseHandCursor;

            /** Creates a button with textures for up- and down-state or text. */
        public:
            Button(Texture *upState, std::string text="", Texture *downState=NULL);

        private:
            void     resetContents();

        private:
            void     createTextField();

        private:
            void     onTouch(TouchEvent *event);

            /** The scale factor of the button on touch. Per default, a button with a down state
              * texture won't scale. */
        public:
            float       scaleWhenDown();
        public:
            void         scaleWhenDown(float value);

            /** The alpha value of the button when it is disabled. @default 0.5 */
        public:
            float       alphaWhenDisabled();
        public:
            void         alphaWhenDisabled(float value);

            /** Indicates if the button can be triggered. */
        public:
            bool      enabled();
        public:
            void         enabled(bool value);

            /** The text that is displayed on the button. */
        public:
            std::string       text();
        public:
            void         text(std::string value);

            /** The name of the font displayed on the button. May be a system font or a registered
              * bitmap font. */
        public:
            std::string       fontName();
        public:
            void         fontName(std::string value);

            /** The size of the font. */
        public:
            float       fontSize();
        public:
            void         fontSize(float value);

            /** The color of the font. */
        public:
            unsigned int         fontColor();
        public:
            void         fontColor(unsigned int value);

            /** Indicates if the font should be bold. */
        public:
            bool      fontBold();
        public:
            void         fontBold(bool value);

            /** The texture that is displayed when the button is not being touched. */
        public:
            Texture      *upState();
        public:
            void         upState(Texture *value);

            /** The texture that is displayed while the button is touched. */
        public:
            Texture      *downState();
        public:
            void         downState(Texture *value);

            /** The vertical alignment of the text on the button. */
        public:
            std::string       textVAlign();
        public:
            void         textVAlign(std::string value);

            /** The horizontal alignment of the text on the button. */
        public:
            std::string       textHAlign();
        public:
            void         textHAlign(std::string value);

            /** The bounds of the textfield on the button. Allows moving the text to a custom position. */
        public:
            Rectangle    *textBounds();
        public:
            void         textBounds(Rectangle *value);

            /** Indicates if the mouse cursor should transform into a hand while it's over the button.
             *  @default true */
        public:
            virtual bool      useHandCursor();
        public:
            virtual void         useHandCursor(bool value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_DISPLAY_BUTTON_AS
#endif // __cplusplus

