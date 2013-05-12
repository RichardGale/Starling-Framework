#if !defined(__STARLING_SRC_STARLING_TEXTURES_ATFDATA_AS)
#define __STARLING_SRC_STARLING_TEXTURES_ATFDATA_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace display3D { class Context3DTextureFormat; } }
namespace flash { namespace utils { class ByteArray; } }

    /** A parser for the ATF data format. */
using namespace flash::display3D;
using namespace flash::utils;

namespace starling {
namespace textures {
    class AtfData
    {
        private:  std::string mFormat;
        private:  int mWidth;
        private:  int mHeight;
        private:  int mNumTextures;
        private:  ByteArray* mData;

        /** Create a new instance by parsing the given byte array. */
        public:          AtfData(ByteArray* data);

        public: std::string  format();
        public: int          width();
        public: int          height();
        public: int          numTextures();
        public: ByteArray*   data();
    };
}
}

#endif // __STARLING_SRC_STARLING_TEXTURES_ATFDATA_AS
#endif // __cplusplus

