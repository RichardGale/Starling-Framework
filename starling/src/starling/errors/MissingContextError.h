#if !defined(__STARLING_SRC_STARLING_ERRORS_MISSINGCONTEXTERROR_AS)
#define __STARLING_SRC_STARLING_ERRORS_MISSINGCONTEXTERROR_AS
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



/** A MissingContextError is thrown when a Context3D object is required but not (yet)
 *  available. */

namespace starling
{
    namespace errors
    {
        class MissingContextError: public Error
        {
            /** Creates a new MissingContextError object. */
        public:
            MissingContextError(void *message, void *id);
        };
    }
}

#endif // __STARLING_SRC_STARLING_ERRORS_MISSINGCONTEXTERROR_AS
#endif // __cplusplus

