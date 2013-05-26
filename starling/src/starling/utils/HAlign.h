#if !defined(__STARLING_SRC_STARLING_UTILS_HALIGN_AS)
#define __STARLING_SRC_STARLING_UTILS_HALIGN_AS
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

/** A class that provides constant values for horizontal alignment of objects. */

using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class HAlign
        {
            /** @private */
        public:
            HAlign();

            /** Left alignment. */
        public:
            static const std::string LEFT;

            /** Centered alignement. */
        public:
            static const std::string CENTER;

            /** Right alignment. */
        public:
            static const std::string RIGHT;

            /** Indicates whether the given alignment string is valid. */
        public:
            static bool  isValid(std::string hAlign);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_HALIGN_AS
#endif // __cplusplus

