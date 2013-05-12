// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "BlendMode.h"
#include "flash/display3D/Context3DBlendFactor.h"

#include "starling/errors/AbstractClassError.h"

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


         std::vector<void*> BlendMode::sBlendFactors=[
            // no premultiplied alpha
            {
                "none"     : [ Context3DBlendFactor::ONE,Context3DBlendFactor::ZERO],
                "normal"   : [ Context3DBlendFactor::SOURCE_ALPHA,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA],
                "add"      : [ Context3DBlendFactor::SOURCE_ALPHA,Context3DBlendFactor::DESTINATION_ALPHA],
                "multiply" : [ Context3DBlendFactor::DESTINATION_COLOR,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA],
                "screen"   : [ Context3DBlendFactor::SOURCE_ALPHA,Context3DBlendFactor::ONE],
                "erase"    : [ Context3DBlendFactor::ZERO,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA]
            },
            // premultiplied alpha
            {
                "none"     : [ Context3DBlendFactor::ONE,Context3DBlendFactor::ZERO],
                "normal"   : [ Context3DBlendFactor::ONE,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA],
                "add"      : [ Context3DBlendFactor::ONE,Context3DBlendFactor::ONE],
                "multiply" : [ Context3DBlendFactor::DESTINATION_COLOR,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA],
                "screen"   : [ Context3DBlendFactor::ONE,Context3DBlendFactor::ONE_MINUS_SOURCE_COLOR],
                "erase"    : [ Context3DBlendFactor::ZERO,Context3DBlendFactor::ONE_MINUS_SOURCE_ALPHA]
            }
        ];

        // predifined modes

        /** @private */
        BlendMode::BlendMode() { throw new AbstractClassError(); }

        /** Inherits the blend mode from this display object's parent. */
        const std::string BlendMode::AUTO="auto";

        /** Deactivates blending, i.e. disabling any transparency. */
        const std::string BlendMode::NONE="none";

        /** The display object appears in front of the background. */
        const std::string BlendMode::NORMAL="normal";

        /** Adds the values of the colors of the display object to the colors of its background. */
        const std::string BlendMode::ADD="add";

        /** Multiplies the values of the display object colors with the the background color. */
        const std::string BlendMode::MULTIPLY="multiply";

        /** Multiplies the complement (inverse) of the display object color with the complement of 
          * the background color, resulting in a bleaching effect. */
        const std::string BlendMode::SCREEN="screen";

        /** Erases the background when drawn on a RenderTexture. */
        const std::string BlendMode::ERASE="erase";

        // accessing modes

        /** Returns the blend factors that correspond with a certain mode and premultiplied alpha
         *  value. Throws an ArgumentError if the mode does not exist. */
        std::vector<void*> BlendMode::getBlendFactors(std::string mode, bool premultipliedAlpha)
        {
             Object* modes=sBlendFactors[int(premultipliedAlpha)];
            if (mode in modes) return modes[mode];
            else throw new ArgumentError("Invalid blend mode");
        }

        /** Registeres a blending mode under a certain name and for a certain premultiplied alpha
         *  (pma) value. If the mode for the other pma value was not yet registered, the factors are
         *  used for both pma settings. */
        void BlendMode::REGISTER(std::string name, std::string sourceFactor, std::string destFactor,
                                        bool premultipliedAlpha)
        {
             Object* modes=sBlendFactors[int(premultipliedAlpha)];
            modes[name] = [sourceFactor, destFactor];

             Object* otherModes=sBlendFactors[int(!premultipliedAlpha)];
            if (!(name in otherModes)) otherModes[name] = [sourceFactor, destFactor];
        }
}
}

