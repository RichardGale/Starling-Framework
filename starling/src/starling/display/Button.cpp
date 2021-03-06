// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Button.h"
#include "flash/geom/Rectangle.h"
#include "flash/ui/Mouse.h"
#include "flash/ui/MouseCursor.h"

#include "starling/events/Event.h"
#include "starling/events/Touch.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/text/TextField.h"
#include "starling/textures/Texture.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

#include "starling/display/DisplayObjectContainer.h"
#include "starling/display/Sprite.h"
#include "starling/display/Image.h"

    /** Dispatched when the user triggers the button. Bubbles. */

using namespace flash::geom;
using namespace flash::ui;
using namespace starling::display;
using namespace starling::events;
using namespace starling::text;
using namespace starling::textures;
using namespace starling::utils;

namespace starling {
namespace display {


        const float Button::MAX_DRAG_DIST = 50;

                    
                    

                    
                    
                    
                    

                    
                    
                    
                    
                    

        /** Creates a button with textures for up- and down-state or text. */
        Button::Button(Texture* upState, std::string text, Texture* downState)
        {
            if (upState == NULL) throw new ArgumentError("Texture cannot be null");

            mUpState = upState;
            mDownState = downState ? downState : upState;
            mBackground = new Image(upState);
            mScaleWhenDown = downState ? 1.0 : 0.9;
            mAlphaWhenDisabled = 0.5;
            mEnabled = true;
            mIsDown = false;
            mUseHandCursor = true;
            mTextBounds = new Rectangle(0, 0, upState->width(), upState->height());

            mContents = new Sprite();
            mContents->addChild(mBackground);
            addChild(mContents);
            addEventListener(TouchEvent::TOUCH, onTouch);

            if (text.length() != 0) this->text = text;
        }

        void Button::resetContents()
        {
            mIsDown = false;
            mBackground->texture ( mUpState);
            mContents->x ( mContents->y ( 0));
            mContents->scaleX ( mContents->scaleY ( 1.0));
        }

        void Button::createTextField()
        {
            if (mTextField == NULL)
            {
                mTextField = new TextField(mTextBounds->width(), mTextBounds->height(), "");
                mTextField->vAlign ( VAlign::CENTER);
                mTextField->hAlign ( HAlign::CENTER);
                mTextField->touchable ( false);
                mTextField->autoScale ( true);
                mContents->addChild(mTextField);
            }

            mTextField->width  ( mTextBounds->width());
            mTextField->height ( mTextBounds->height());
            mTextField->x ( mTextBounds->x());
            mTextField->y ( mTextBounds->y());
        }

        void Button::onTouch(TouchEvent* event)
        {
            Mouse::cursor ( (mUseHandCursor && mEnabled && event->interactsWith(this)) ?
                MouseCursor::BUTTON : MouseCursor::AUTO);

            Touch* touch = event->getTouch(this);
            if (!mEnabled || touch == NULL) return;

            if (touch->phase() == TouchPhase::BEGAN() && !mIsDown)
            {
                mBackground->texture ( mDownState);
                mContents->scaleX ( mContents->scaleY ( mScaleWhenDown));
                mContents->x ( (1.0 - mScaleWhenDown) / 2.0 * mBackground->width());
                mContents->y ( (1.0 - mScaleWhenDown) / 2.0 * mBackground->height());
                mIsDown = true;
            }
            else if (touch->phase() == TouchPhase::MOVED() && mIsDown)
            {
                // reset button when user dragged too far away after pushing
                Rectangle* buttonRect = getBounds(stage);
                if (touch->globalX() < buttonRect->x - MAX_DRAG_DIST ||
                    touch->globalY() < buttonRect->y - MAX_DRAG_DIST ||
                    touch->globalX() > buttonRect->x() + buttonRect->width + MAX_DRAG_DIST ||
                    touch->globalY() > buttonRect->y() + buttonRect->height() + MAX_DRAG_DIST)
                {
                    resetContents();
                }
            }
            else if (touch->phase() == TouchPhase::ENDED() && mIsDown)
            {
                resetContents();
                dispatchEventWith(Event::TRIGGERED, true);
            }
        }

        /** The scale factor of the button on touch. Per default, a button with a down state 
          * texture won't scale. */
        float Button::scaleWhenDown()        { return mScaleWhenDown; }
        void Button::scaleWhenDown(float value)      { mScaleWhenDown = value; }

        /** The alpha value of the button when it is disabled. @default 0.5 */
        float Button::alphaWhenDisabled()        { return mAlphaWhenDisabled; }
        void Button::alphaWhenDisabled(float value)      { mAlphaWhenDisabled = value; }

        /** Indicates if the button can be triggered. */
        bool Button::enabled()         { return mEnabled; }
        void Button::enabled(bool value)
        {
            if (mEnabled != value)
            {
                mEnabled = value;
                mContents->alpha ( value ? 1.0 : mAlphaWhenDisabled);
                resetContents();
            }
        }

        /** The text that is displayed on the button. */
        std::string Button::text()        { return mTextField ? mTextField->text() : ""; }
        void Button::text(std::string value)
        {
            createTextField();
            mTextField->text ( value);
        }

        /** The name of the font displayed on the button. May be a system font or a registered 
          * bitmap font. */
        std::string Button::fontName()        { return mTextField ? mTextField->fontName() : "Verdana"; }
        void Button::fontName(std::string value)
        {
            createTextField();
            mTextField->fontName ( value);
        }

        /** The size of the font. */
        float Button::fontSize()        { return mTextField ? mTextField->fontSize() : 12; }
        void Button::fontSize(float value)
        {
            createTextField();
            mTextField->fontSize ( value);
        }

        /** The color of the font. */
        unsigned int Button::fontColor()      { return mTextField ? mTextField->color() : 0x0; }
        void Button::fontColor(unsigned int value)
        {
            createTextField();
            mTextField->color ( value);
        }

        /** Indicates if the font should be bold. */
        bool Button::fontBold()         { return mTextField ? mTextField->bold() : false; }
        void Button::fontBold(bool value)
        {
            createTextField();
            mTextField->bold ( value);
        }

        /** The texture that is displayed when the button is not being touched. */
        Texture* Button::upState()         { return mUpState; }
        void Button::upState(Texture* value)
        {
            if (mUpState != value)
            {
                mUpState = value;
                if (!mIsDown) mBackground->texture ( value);
            }
        }

        /** The texture that is displayed while the button is touched. */
        Texture* Button::downState()         { return mDownState; }
        void Button::downState(Texture* value)
        {
            if (mDownState != value)
            {
                mDownState = value;
                if (mIsDown) mBackground->texture ( value);
            }
        }

        /** The vertical alignment of the text on the button. */
        std::string Button::textVAlign()        { return mTextField->vAlign(); }
        void Button::textVAlign(std::string value)
        {
            createTextField();
            mTextField->vAlign ( value);
        }

        /** The horizontal alignment of the text on the button. */
        std::string Button::textHAlign()        { return mTextField->hAlign(); }
        void Button::textHAlign(std::string value)
        {
            createTextField();
            mTextField->hAlign ( value);
        }

        /** The bounds of the textfield on the button. Allows moving the text to a custom position. */
        Rectangle* Button::textBounds()           { return mTextBounds->clone(); }
        void Button::textBounds(Rectangle* value)
        {
            mTextBounds = value->clone();
            createTextField();
        }

        /** Indicates if the mouse cursor should transform into a hand while it's over the button. 
         *  @default true */
        bool Button::useHandCursor()         { return mUseHandCursor; }
        void Button::useHandCursor(bool value)      { mUseHandCursor = value; }
}
}

