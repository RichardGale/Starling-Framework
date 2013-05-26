// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextureSmoothing.h"
#include "starling/errors/AbstractClassError.h"

    /** A class that provides constant values for the possible smoothing algorithms of a texture. */

using namespace starling::errors;

namespace starling {
namespace textures {


        /** @private */
        TextureSmoothing::TextureSmoothing() { throw new AbstractClassError(); }

        /** No smoothing, also called "Nearest Neighbor". Pixels will scale up as big rectangles. */
        const std::string TextureSmoothing::NONE      = "none";

        /** Bilinear filtering. Creates smooth transitions between pixels. */
        const std::string TextureSmoothing::BILINEAR  = "bilinear";

        /** Trilinear filtering. Highest quality by taking the next mip map level into account. */
        const std::string TextureSmoothing::TRILINEAR = "trilinear";

        /** Determines whether a smoothing value is valid. */
        bool TextureSmoothing::isValid(std::string smoothing)
        {
            return smoothing == NONE || smoothing == BILINEAR || smoothing == TRILINEAR;
        }
}
}

