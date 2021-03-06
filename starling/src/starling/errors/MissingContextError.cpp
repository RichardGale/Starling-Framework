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


namespace starling {
namespace errors {


        /** Creates a new MissingContextError object. */
        MissingContextError::MissingContextError(void* message, void* id)
        {
            Error(message, id);
        }
}
}

