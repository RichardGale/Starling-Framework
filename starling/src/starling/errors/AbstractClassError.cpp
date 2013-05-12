// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "AbstractClassError.h"
#include "Error.h"

    /** An AbstractClassError is thrown when you attempt to create an instance of an abstract 
     *  class. */

namespace starling {
namespace errors {


        /** Creates a new AbstractClassError object. */
        AbstractClassError::AbstractClassError(std::string message, int id)
        {
            super(message, id);
        }
}
}

