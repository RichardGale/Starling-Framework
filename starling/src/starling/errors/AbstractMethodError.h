#if !defined(__STARLING_SRC_STARLING_ERRORS_ABSTRACTMETHODERROR_AS)
#define __STARLING_SRC_STARLING_ERRORS_ABSTRACTMETHODERROR_AS
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



/** An AbstractMethodError is thrown when you attempt to call an abstract method. */

#include "flex11.6.h"



namespace starling
{
    namespace errors
    {
        class AbstractMethodError : public Error
        {
            /** Creates a new AbstractMethodError object. */
        public:
            AbstractMethodError(void *message=(void *)"", void *id=0);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ERRORS_ABSTRACTMETHODERROR_AS
#endif // __cplusplus

