// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



    /** Returns the next power of two that is equal to or bigger than the specified number. */
    int ::getNextPowerOfTwo(int number)
    {
        if (number > 0 && (number & (number - 1)) == 0)
            return number;
        else
        {
             int result = 1;
            while (result < number) result <<= 1;
            return result;
        }
    }                                                   // see: http://goo.gl/D9kPj
