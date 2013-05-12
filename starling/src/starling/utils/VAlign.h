#if !defined(__STARLING_SRC_STARLING_UTILS_VALIGN_AS)
#define __STARLING_SRC_STARLING_UTILS_VALIGN_AS
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



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A class that provides constant values for vertical alignment of objects. */
using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class VAlign
        {
            /** @private */
        public:
            VAlign();

            /** Top alignment. */
        public:
            static const std::string TOP;

            /** Centered alignment. */
        public:
            static const std::string CENTER;

            /** Bottom alignment. */
        public:
            static const std::string BOTTOM;

            /** Indicates whether the given alignment string is valid. */
        public:
            static bool isValid(std::string vAlign);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_VALIGN_AS
#endif // __cplusplus

