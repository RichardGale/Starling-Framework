#if !defined(__STARLING_SRC_STARLING_ERRORS_ABSTRACTCLASSERROR_AS)
#define __STARLING_SRC_STARLING_ERRORS_ABSTRACTCLASSERROR_AS
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



/** An AbstractClassError is thrown when you attempt to create an instance of an abstract
 *  class. */

namespace starling
{
    namespace errors
    {
        class AbstractClassError: public Error
        {
            /** Creates a new AbstractClassError object. */
        public:
            AbstractClassError(void *message, void *id);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ERRORS_ABSTRACTCLASSERROR_AS
#endif // __cplusplus

