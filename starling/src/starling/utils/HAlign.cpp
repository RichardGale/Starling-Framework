// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "HAlign.h"
#include "starling/errors/AbstractClassError.h"

    /** A class that provides constant values for horizontal alignment of objects. */

using namespace starling::errors;

namespace starling {
namespace utils {


        /** @private */
        HAlign::HAlign() { throw new AbstractClassError(); }

        /** Left alignment. */
        const std::string HAlign::LEFT   = "left";

        /** Centered alignement. */
        const std::string HAlign::CENTER = "center";

        /** Right alignment. */
        const std::string HAlign::RIGHT  = "right";

        /** Indicates whether the given alignment string is valid. */
        bool HAlign::isValid(std::string hAlign)
        {
            return hAlign == LEFT || hAlign == CENTER || hAlign == RIGHT;
        }
}
}

