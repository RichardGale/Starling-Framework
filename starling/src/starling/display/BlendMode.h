#if !defined(__STARLING_SRC_STARLING_DISPLAY_BLENDMODE_AS)
#define __STARLING_SRC_STARLING_DISPLAY_BLENDMODE_AS
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



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace display3D { class Context3DBlendFactor; } }

namespace starling { namespace errors { class AbstractClassError; } }

    /** A class that provides constant values for visual blend mode effects. 
     *   
     *  <p>A blend mode is always defined by two 'Context3DBlendFactor' values. A blend factor 
     *  represents a particular four-value vector that is multiplied with the source or destination
     *  color in the blending formula. The blending formula is:</p>
     * 
     *  <pre>result = source    sourceFactor + destination    destinationFactor</pre>
     * 
     *  <p>In the formula, the source color is the output color of the pixel shader program. The 
     *  destination color is the color that currently exists in the color buffer, as set by 
     *  previous clear and draw operations.</p>
     *  
     *  <p>Beware that blending factors produce different output depending on the texture type.
     *  Textures may contain 'premultiplied alpha' (pma), which means that their RGB values were 
     *  multiplied with their alpha value (to save processing time). Textures based on 'BitmapData'
     *  objects have premultiplied alpha values, while ATF textures haven't. For this reason, 
     *  a blending mode may have different factors depending on the pma value.</p>
     *  
     *  @see flash.display3D.Context3DBlendFactor
     */
using namespace flash::display3D;
using namespace starling::errors;

namespace starling {
namespace display {
    class BlendMode
    {
        private: static  std::vector<void*> sBlendFactors;

        // predifined modes

        /** @private */
        public:          BlendMode();

        /** Inherits the blend mode from this display object's parent. */
        public: static const std::string AUTO;

        /** Deactivates blending, i.e. disabling any transparency. */
        public: static const std::string NONE;

        /** The display object appears in front of the background. */
        public: static const std::string NORMAL;

        /** Adds the values of the colors of the display object to the colors of its background. */
        public: static const std::string ADD;

        /** Multiplies the values of the display object colors with the the background color. */
        public: static const std::string MULTIPLY;

        /** Multiplies the complement (inverse) of the display object color with the complement of 
          * the background color, resulting in a bleaching effect. */
        public: static const std::string SCREEN;

        /** Erases the background when drawn on a RenderTexture. */
        public: static const std::string ERASE;

        // accessing modes

        /** Returns the blend factors that correspond with a certain mode and premultiplied alpha
         *  value. Throws an ArgumentError if the mode does not exist. */
        public: static std::vector<void*> getBlendFactors(std::string mode, bool premultipliedAlpha   =true);

        /** Registeres a blending mode under a certain name and for a certain premultiplied alpha
         *  (pma) value. If the mode for the other pma value was not yet registered, the factors are
         *  used for both pma settings. */
        public: static void REGISTER(std::string name, std::string sourceFactor, std::string destFactor,
                                        bool premultipliedAlpha   =true);
    };
}
}

#endif // __STARLING_SRC_STARLING_DISPLAY_BLENDMODE_AS
#endif // __cplusplus

