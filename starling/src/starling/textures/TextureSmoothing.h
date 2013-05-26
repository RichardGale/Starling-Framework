#if !defined(__STARLING_SRC_STARLING_TEXTURES_TEXTURESMOOTHING_AS)
#define __STARLING_SRC_STARLING_TEXTURES_TEXTURESMOOTHING_AS
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
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A class that provides constant values for the possible smoothing algorithms of a texture. */

using namespace starling::errors;

namespace starling
{
    namespace textures
    {
        class TextureSmoothing
        {
            /** @private */
        public:
            TextureSmoothing();

            /** No smoothing, also called "Nearest Neighbor". Pixels will scale up as big rectangles. */
        public:
            static const std::string NONE;

            /** Bilinear filtering. Creates smooth transitions between pixels. */
        public:
            static const std::string BILINEAR;

            /** Trilinear filtering. Highest quality by taking the next mip map level into account. */
        public:
            static const std::string TRILINEAR;

            /** Determines whether a smoothing value is valid. */
        public:
            static bool  isValid(std::string smoothing);
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXTURES_TEXTURESMOOTHING_AS
#endif // __cplusplus

