#if !defined(__STARLING_SRC_STARLING_ANIMATION_TRANSITIONS_AS)
#define __STARLING_SRC_STARLING_ANIMATION_TRANSITIONS_AS
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
//
// easing functions thankfully taken from http://dojotoolkit.org
//                                    and http://www.robertpenner.com/easing
//



#include "flex11.6.h"
namespace flash
{
    namespace utils
    {
        class Dictionary;
    }
}

namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** The Transitions class contains static methods that define easing functions.
 *  Those functions are used by the Tween class to execute animations.
 *
 *  <p>Here is a visual representation of the available transitions:</p>
 *  <img src="http://gamua.com/img/blog/2010/sparrow-transitions.png"/>
 *
 *  <p>You can define your own transitions through the "registerTransition" function. A
 *  transition function must have the following signature, where <code>ratio</code> is
 *  in the range 0-1:</p>
 *
 *  <pre>function myTransition(ratio:Number):Number</pre>
 */

using namespace flash::utils;
using namespace starling::errors;

namespace starling
{
    namespace animation
    {
        class Transitions
        {
        public:
            static const std::string LINEAR;
        public:
            static const std::string EASE_IN;
        public:
            static const std::string EASE_OUT;
        public:
            static const std::string EASE_IN_OUT;
        public:
            static const std::string EASE_OUT_IN;
        public:
            static const std::string EASE_IN_BACK;
        public:
            static const std::string EASE_OUT_BACK;
        public:
            static const std::string EASE_IN_OUT_BACK;
        public:
            static const std::string EASE_OUT_IN_BACK;
        public:
            static const std::string EASE_IN_ELASTIC;
        public:
            static const std::string EASE_OUT_ELASTIC;
        public:
            static const std::string EASE_IN_OUT_ELASTIC;
        public:
            static const std::string EASE_OUT_IN_ELASTIC;
        public:
            static const std::string EASE_IN_BOUNCE;
        public:
            static const std::string EASE_OUT_BOUNCE;
        public:
            static const std::string EASE_IN_OUT_BOUNCE;
        public:
            static const std::string EASE_OUT_IN_BOUNCE;

        private:
            static std::map<std::string, void *> sTransitions;

            /** @private */
        public:
            Transitions();

            /** Returns the transition function that was registered under a certain name. */
        public:
            static Function *getTransition(std::string name);

            /** Registers a new transition function under a certain name. */
        public:
            static void     REGISTER(std::string name, Function *func);

        private:
            static void     registerDefaults();

            // transition functions

        protected:
            static float   linear(float ratio);

        protected:
            static float   easeIn(float ratio);

        protected:
            static float   easeOut(float ratio);

        protected:
            static float   easeInOut(float ratio);

        protected:
            static float   easeOutIn(float ratio);

        protected:
            static float   easeInBack(float ratio);

        protected:
            static float   easeOutBack(float ratio);

        protected:
            static float   easeInOutBack(float ratio);

        protected:
            static float   easeOutInBack(float ratio);

        protected:
            static float   easeInElastic(float ratio);

        protected:
            static float   easeOutElastic(float ratio);

        protected:
            static float   easeInOutElastic(float ratio);

        protected:
            static float   easeOutInElastic(float ratio);

        protected:
            static float   easeInBounce(float ratio);

        protected:
            static float   easeOutBounce(float ratio);

        protected:
            static float   easeInOutBounce(float ratio);

        protected:
            static float   easeOutInBounce(float ratio);

        protected:
            static float   easeCombined(Function *startFunc, Function *endFunc, float ratio);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ANIMATION_TRANSITIONS_AS
#endif // __cplusplus

