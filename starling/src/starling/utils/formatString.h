#if !defined(__STARLING_SRC_STARLING_UTILS_FORMATSTRING_AS)
#define __STARLING_SRC_STARLING_UTILS_FORMATSTRING_AS
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



// TODO: add number formatting options

/** Formats a String in .Net-style, with curly braces ("{0}"). Does not support any
 *  number formatting options yet. */
public:
std::string formatString(std::string format, ...);
#endif // __STARLING_SRC_STARLING_UTILS_FORMATSTRING_AS
#endif // __cplusplus

