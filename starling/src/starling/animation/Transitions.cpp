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



#include "Transitions.h"
#include "flash/utils/Dictionary.h"

#include "starling/errors/AbstractClassError.h"

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


        const std::string Transitions::LINEAR="linear";
        const std::string Transitions::EASE_IN="easeIn";
        const std::string Transitions::EASE_OUT="easeOut";
        const std::string Transitions::EASE_IN_OUT="easeInOut";
        const std::string Transitions::EASE_OUT_IN="easeOutIn";
        const std::string Transitions::EASE_IN_BACK="easeInBack";
        const std::string Transitions::EASE_OUT_BACK="easeOutBack";
        const std::string Transitions::EASE_IN_OUT_BACK="easeInOutBack";
        const std::string Transitions::EASE_OUT_IN_BACK="easeOutInBack";
        const std::string Transitions::EASE_IN_ELASTIC="easeInElastic";
        const std::string Transitions::EASE_OUT_ELASTIC="easeOutElastic";
        const std::string Transitions::EASE_IN_OUT_ELASTIC="easeInOutElastic";
        const std::string Transitions::EASE_OUT_IN_ELASTIC="easeOutInElastic";
        const std::string Transitions::EASE_IN_BOUNCE="easeInBounce";
        const std::string Transitions::EASE_OUT_BOUNCE="easeOutBounce";
        const std::string Transitions::EASE_IN_OUT_BOUNCE="easeInOutBounce";
        const std::string Transitions::EASE_OUT_IN_BOUNCE="easeOutInBounce";

        std::map<std::string, void *> Transitions::sTransitions;

        /** @private */
        Transitions::Transitions()
        {
            throw new AbstractClassError();
        }

        /** Returns the transition function that was registered under a certain name. */
        Function *Transitions::getTransition(std::string name)
        {
            if (sTransitions.empty()) registerDefaults();
            return sTransitions[name];
        }

        /** Registers a new transition function under a certain name. */
        void Transitions::REGISTER(std::string name, Function *func)
        {
            if (sTransitions.empty()) registerDefaults();
            sTransitions[name] = func;
        }

        void Transitions::registerDefaults()
        {
            sTransitions.clear();

            REGISTER(LINEAR, linear);
            REGISTER(EASE_IN, easeIn);
            REGISTER(EASE_OUT, easeOut);
            REGISTER(EASE_IN_OUT, easeInOut);
            REGISTER(EASE_OUT_IN, easeOutIn);
            REGISTER(EASE_IN_BACK, easeInBack);
            REGISTER(EASE_OUT_BACK, easeOutBack);
            REGISTER(EASE_IN_OUT_BACK, easeInOutBack);
            REGISTER(EASE_OUT_IN_BACK, easeOutInBack);
            REGISTER(EASE_IN_ELASTIC, easeInElastic);
            REGISTER(EASE_OUT_ELASTIC, easeOutElastic);
            REGISTER(EASE_IN_OUT_ELASTIC, easeInOutElastic);
            REGISTER(EASE_OUT_IN_ELASTIC, easeOutInElastic);
            REGISTER(EASE_IN_BOUNCE, easeInBounce);
            REGISTER(EASE_OUT_BOUNCE, easeOutBounce);
            REGISTER(EASE_IN_OUT_BOUNCE, easeInOutBounce);
            REGISTER(EASE_OUT_IN_BOUNCE, easeOutInBounce);
        }

        // transition functions

        float Transitions::linear(float ratio)
        {
            return ratio;
        }

        float Transitions::easeIn(float ratio)
        {
            return ratio * ratio * ratio;
        }

        float Transitions::easeOut(float ratio)
        {
            float invRatio = ratio - 1.0;
            return invRatio * invRatio * invRatio + 1;
        }

        float Transitions::easeInOut(float ratio)
        {
            return easeCombined(easeIn, easeOut, ratio);
        }

        float Transitions::easeOutIn(float ratio)
        {
            return easeCombined(easeOut, easeIn, ratio);
        }

        float Transitions::easeInBack(float ratio)
        {
            float s = 1.70158;
            return Math::pow(ratio,2) * ((s + 1.0)*ratio - s);
        }

        float Transitions::easeOutBack(float ratio)
        {
            float invRatio = ratio - 1.0;
            float s = 1.70158;
            return Math::pow(invRatio,2) * ((s + 1.0)*invRatio + s) + 1.0;
        }

        float Transitions::easeInOutBack(float ratio)
        {
            return easeCombined(easeInBack, easeOutBack, ratio);
        }

        float Transitions::easeOutInBack(float ratio)
        {
            return easeCombined(easeOutBack, easeInBack, ratio);
        }

        float Transitions::easeInElastic(float ratio)
        {
            if (ratio == 0 || ratio == 1) return ratio;
            else
            {
                float p = 0.3;
                float s = p/4.0;
                float invRatio = ratio - 1;
                return -1.0 * Math::pow(2.0,10.0*invRatio) * Math::sin((invRatio-s)*(2.0*Math::PI)/p);
            }
        }

        float Transitions::easeOutElastic(float ratio)
        {
            if (ratio == 0 || ratio == 1) return ratio;
            else
            {
                float p = 0.3;
                float s = p/4.0;
                return Math::pow(2.0,-10.0*ratio) * Math::sin((ratio-s)*(2.0*Math::PI)/p)+1;
            }
        }

        float Transitions::easeInOutElastic(float ratio)
        {
            return easeCombined(easeInElastic, easeOutElastic, ratio);
        }

        float Transitions::easeOutInElastic(float ratio)
        {
            return easeCombined(easeOutElastic, easeInElastic, ratio);
        }

        float Transitions::easeInBounce(float ratio)
        {
            return 1.0 - easeOutBounce(1.0 - ratio);
        }

        float Transitions::easeOutBounce(float ratio)
        {
            float s = 7.5625;
            float p = 2.75;
            float l;
            if (ratio < (1.0/p))
            {
                l = s * Math::pow(ratio,2);
            }
            else
            {
                if (ratio < (2.0/p))
                {
                    ratio -= 1.5/p;
                    l = s * Math::pow(ratio,2) + 0.75;
                }
                else
                {
                    if (ratio < 2.5/p)
                    {
                        ratio -= 2.25/p;
                        l = s * Math::pow(ratio,2) + 0.9375;
                    }
                    else
                    {
                        ratio -= 2.625/p;
                        l =  s * Math::pow(ratio,2) + 0.984375;
                    }
                }
            }
            return l;
        }

        float Transitions::easeInOutBounce(float ratio)
        {
            return easeCombined(easeInBounce, easeOutBounce, ratio);
        }

        float Transitions::easeOutInBounce(float ratio)
        {
            return easeCombined(easeOutBounce, easeInBounce, ratio);
        }

        float Transitions::easeCombined(Function *startFunc, Function *endFunc, float ratio)
        {
            if (ratio < 0.5) return 0.5 * startFunc(ratio*2.0);
            else             return 0.5 * endFunc((ratio-0.5)*2.0) + 0.5;
        }
    }
}

