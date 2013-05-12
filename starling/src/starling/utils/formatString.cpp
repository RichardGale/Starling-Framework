// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



// TODO: add number formatting options

/** Formats a String in .Net-style, with curly braces ("{0}"). Does not support any
 *  number formatting options yet. */
std::string ::formatString(std::string format, ...)
{
    for ( int i=0; i<args->length; ++i)
        format = format->replace(newRegExp("\\{"+i+"\\}", "g"), args[i]);

    return format;
}
