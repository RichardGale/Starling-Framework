// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "AtfData.h"
#include "flash/display3D/Context3DTextureFormat.h"
#include "flash/utils/ByteArray.h"

    /** A parser for the ATF data format. */

using namespace flash::display3D;
using namespace flash::utils;

namespace starling {
namespace textures {


                    
                    
                    
                    
                    

        /** Create a new instance by parsing the given byte array. */
        AtfData::AtfData(ByteArray* data)
        {
            std::string signature = String()->fromCharCode(data[0], data[1], data[2]);
            if (signature != "ATF") throw new ArgumentError("Invalid ATF data");

            switch (data[6])
            {
                case 0:
                case 1: mFormat = Context3DTextureFormat::BGRA; break;
                case 2:
                case 3: mFormat = Context3DTextureFormat::COMPRESSED; break;
                case 4:
                case 5: mFormat = "compressedAlpha"; break; // explicit string to stay compatible 
                                                            // with older versions
                default: throw new Error("Invalid ATF format");
            }

            mWidth = Math::pow(2, data[7]);
            mHeight = Math::pow(2, data[8]);
            mNumTextures = data[9];
            mData = data;
        }

        std::string AtfData::format()        { return mFormat; }
        int AtfData::width()     { return mWidth; }
        int AtfData::height()     { return mHeight; }
        int AtfData::numTextures()     { return mNumTextures; }
        ByteArray* AtfData::data()           { return mData; }
}
}

