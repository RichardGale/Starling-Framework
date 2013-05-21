// =================================================================================================
//
//  Starling Framework
//  Copyright 2013 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "TextFieldAutoSize.h"
#include "starling/errors/AbstractClassError.h"

    /** This class is an enumeration of constant values used in setting the 
     *  autoSize property of the TextField class. */

using namespace starling::errors;

namespace starling {
namespace text {


        /** @private */
        TextFieldAutoSize::TextFieldAutoSize() { throw new AbstractClassError(); }

        /** No auto-sizing will happen. */
        const std::string TextFieldAutoSize::NONE="none";

        /** The text field will grow to the right; no line-breaks will be added.
         *  The height of the text field remains unchanged. */
        const std::string TextFieldAutoSize::HORIZONTAL="horizontal";

        /** The text field will grow to the bottom, adding line-breaks when necessary.
          * The width of the text field remains unchanged. */
        const std::string TextFieldAutoSize::VERTICAL="vertical";

        /** The text field will grow to the right and bottom; no line-breaks will be added. */
        const std::string TextFieldAutoSize::BOTH_DIRECTIONS="bothDirections";
}
}

