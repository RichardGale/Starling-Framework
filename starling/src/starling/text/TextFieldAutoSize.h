#if !defined(__STARLING_SRC_STARLING_TEXT_TEXTFIELDAUTOSIZE_AS)
#define __STARLING_SRC_STARLING_TEXT_TEXTFIELDAUTOSIZE_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2013 Gamua OG. All Rights Reserved.
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

/** This class is an enumeration of constant values used in setting the
 *  autoSize property of the TextField class. */

using namespace starling::errors;

namespace starling
{
    namespace text
    {
        class TextFieldAutoSize
        {
            /** @private */
        public:
            TextFieldAutoSize();

            /** No auto-sizing will happen. */
        public:
            static const std::string NONE;

            /** The text field will grow to the right; no line-breaks will be added.
             *  The height of the text field remains unchanged. */
        public:
            static const std::string HORIZONTAL;

            /** The text field will grow to the bottom, adding line-breaks when necessary.
              * The width of the text field remains unchanged. */
        public:
            static const std::string VERTICAL;

            /** The text field will grow to the right and bottom; no line-breaks will be added. */
        public:
            static const std::string BOTH_DIRECTIONS;
        };
    }
}

#endif // __STARLING_SRC_STARLING_TEXT_TEXTFIELDAUTOSIZE_AS
#endif // __cplusplus

