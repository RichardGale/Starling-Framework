#if !defined(__STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTERMODE_AS)
#define __STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTERMODE_AS
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
namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A class that provides constant values for filter modes. The values are used in the
 *  FragmentFilter.mode property and define how a filter result is combined with the
 *  original object. */
using namespace starling::errors;

namespace starling
{
    namespace filters
    {
        class FragmentFilterMode
        {
            /** @private */
        public:
            FragmentFilterMode();

            /** The filter is displayed below the filtered object. */
        public:
            static const std::string BELOW;

            /** The filter is replacing the filtered object. */
        public:
            static const std::string REPLACE;

            /** The filter is displayed above the filtered object. */
        public:
            static const std::string ABOVE;
        };
    }
}

#endif // __STARLING_SRC_STARLING_FILTERS_FRAGMENTFILTERMODE_AS
#endif // __cplusplus

