#if !defined(__TESTS_SRC_TESTS_TEXTURETEST_AS)
#define __TESTS_SRC_TESTS_TEXTURETEST_AS
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



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash
{
    namespace geom
    {
        class Point;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}

namespace org
{
    namespace flexunit
    {
        class assertThat;
    }
}
namespace org
{
    namespace flexunit
    {
        namespace asserts
        {
            class assertEquals;
        }
    }
}
namespace org
{
    namespace hamcrest
    {
        namespace number
        {
            class closeTo;
        }
    }
}

namespace starling
{
    namespace textures
    {
        class ConcreteTexture;
    }
}
namespace starling
{
    namespace textures
    {
        class SubTexture;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace utils
    {
        class VertexData;
    }
}

using namespace flash::geom;
using namespace flash::geom;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::textures;
using namespace starling::textures;
using namespace starling::textures;
using namespace starling::utils;

namespace tests
{
    class TextureTest
    {
    private:
        static const float E;


    public:
        void
        ////[Test(expects="starling.errors.AbstractClassError")]testCreateTexture();


    public:
        void
        ////[Test]      testTextureCoordinates();

    private:
        VertexData *createStandardVertexData();

    private:
        VertexData *createVertexDataWithMovedTexCoords();


    public:
        void
        ////[Test]      testGetRoot();


    public:
        void
        ////[Test]      testGetSize();
    };
}

#endif // __TESTS_SRC_TESTS_TEXTURETEST_AS
#endif // __cplusplus

