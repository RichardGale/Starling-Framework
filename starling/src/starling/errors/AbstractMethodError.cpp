// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================





namespace starling
{
    namespace errors
    {
        /** An AbstractMethodError is thrown when you attempt to call an abstract method. */

        /** Creates a new AbstractMethodError object. */
        AbstractMethodError::AbstractMethodError(void *message, void *id)
        {
            super(message, id);
        }
    }
}

