// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "VAlign.h"
#include "starling/errors/AbstractClassError.h"

/** A class that provides constant values for vertical alignment of objects. */
using namespace starling::errors;

namespace starling
{
    namespace utils
    {


        /** @private */
        VAlign::VAlign()
        {
            throw new AbstractClassError();
        }

        /** Top alignment. */
        const std::string VAlign::TOP="top";

        /** Centered alignment. */
        const std::string VAlign::CENTER="center";

        /** Bottom alignment. */
        const std::string VAlign::BOTTOM="bottom";

        /** Indicates whether the given alignment string is valid. */
        bool VAlign::isValid(std::string vAlign)
        {
            return vAlign == TOP || vAlign == CENTER || vAlign == BOTTOM;
        }
    }
}

