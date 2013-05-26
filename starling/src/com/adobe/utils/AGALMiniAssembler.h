#if !defined(__STARLING_SRC_COM_ADOBE_UTILS_AGALMINIASSEMBLER_AS)
#define __STARLING_SRC_COM_ADOBE_UTILS_AGALMINIASSEMBLER_AS
#if defined(__cplusplus)
/*
Copyright (c) 2011, Adobe Systems Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

* Neither the name of Adobe Systems Incorporated nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// ===========================================================================
//  Imports
// ---------------------------------------------------------------------------
#include "flex11.6.h"
namespace flash
{
    namespace display3D
    {
        class Context3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class Program3D;
    }
}
namespace flash
{
    namespace utils
    {
        class ByteArray;
    }
}
namespace flash
{
    namespace utils
    {
        class Endian;
    }
}
namespace flash
{
    namespace utils
    {
        class getTimer;
    }
}

// ===========================================================================
//  Class
// ---------------------------------------------------------------------------

using namespace flash::display3D;
using namespace flash::utils;

namespace com
{
    namespace adobe
    {
        namespace utils
        {
            class AGALMiniAssembler
            {
                // ======================================================================
                //  Constants
                // ----------------------------------------------------------------------
            protected:
                static const RegExp *REGEXP_OUTER_SPACES;

                // ======================================================================
                //  Properties
                // ----------------------------------------------------------------------
                // AGAL bytes and error buffer
            private:
                ByteArray *_agalcode;
            private:
                std::string _error;

            private:
                bool debugEnabled;

            private:
                static bool initialized;
            public:
                bool verbose;

                // ======================================================================
                //  Getters
                // ----------------------------------------------------------------------
            public:
                std::string       error();
            public:
                ByteArray    *agalcode();

                // ======================================================================
                //  Constructor
                // ----------------------------------------------------------------------
            public:
                AGALMiniAssembler( bool debugging = false);
                // ======================================================================
                //  Methods
                // ----------------------------------------------------------------------

            public:
                Program3D *assemble2( Context3D *ctx3d, unsigned int version, std::string vertexsrc, std::string fragmentsrc);

            public:
                ByteArray *assemble( std::string mode, std::string source, unsigned int version=1, bool ignorelimits=false);

            private:
                void     initregmap( unsigned int version, bool ignorelimits);

            private:
                static         void     init();

                // ======================================================================
                //  Constants
                // ----------------------------------------------------------------------
            private:
                static const std::map<std::string, void *> OPMAP;
            private:
                static const std::map<std::string, void *> REGMAP;
            private:
                static const std::map<std::string, void *> SAMPLEMAP;

            private:
                static const int MAX_NESTING;
            private:
                static const int MAX_OPCODES;

            private:
                static const std::string FRAGMENT;
            private:
                static const std::string VERTEX;

                // masks and shifts
            private:
                static const unsigned int SAMPLER_TYPE_SHIFT;
            private:
                static const unsigned int SAMPLER_DIM_SHIFT;
            private:
                static const unsigned int SAMPLER_SPECIAL_SHIFT;
            private:
                static const unsigned int SAMPLER_REPEAT_SHIFT;
            private:
                static const unsigned int SAMPLER_MIPMAP_SHIFT;
            private:
                static const unsigned int SAMPLER_FILTER_SHIFT;

                // regmap flags
            private:
                static const unsigned int REG_WRITE;
            private:
                static const unsigned int REG_READ;
            private:
                static const unsigned int REG_FRAG;
            private:
                static const unsigned int REG_VERT;

                // opmap flags
            private:
                static const unsigned int OP_SCALAR;
            private:
                static const unsigned int OP_SPECIAL_TEX;
            private:
                static const unsigned int OP_SPECIAL_MATRIX;
            private:
                static const unsigned int OP_FRAG_ONLY;
            private:
                static const unsigned int OP_VERT_ONLY;
            private:
                static const unsigned int OP_NO_DEST;
            private:
                static const unsigned int OP_VERSION2;
            private:
                static const unsigned int OP_INCNEST;
            private:
                static const unsigned int OP_DECNEST;

                // opcodes
            private:
                static const std::string MOV;
            private:
                static const std::string ADD;
            private:
                static const std::string SUB;
            private:
                static const std::string MUL;
            private:
                static const std::string DIV;
            private:
                static const std::string RCP;
            private:
                static const std::string MIN;
            private:
                static const std::string MAX;
            private:
                static const std::string FRC;
            private:
                static const std::string SQT;
            private:
                static const std::string RSQ;
            private:
                static const std::string POW;
            private:
                static const std::string LOG;
            private:
                static const std::string EXP;
            private:
                static const std::string NRM;
            private:
                static const std::string SIN;
            private:
                static const std::string COS;
            private:
                static const std::string CRS;
            private:
                static const std::string DP3;
            private:
                static const std::string DP4;
            private:
                static const std::string ABS;
            private:
                static const std::string NEG;
            private:
                static const std::string SAT;
            private:
                static const std::string M33;
            private:
                static const std::string M44;
            private:
                static const std::string M34;
            private:
                static const std::string DDX;
            private:
                static const std::string DDY;
            private:
                static const std::string IFE;
            private:
                static const std::string INE;
            private:
                static const std::string IFG;
            private:
                static const std::string IFL;
            private:
                static const std::string ELS;
            private:
                static const std::string EIF;
            private:
                static const std::string TED;
            private:
                static const std::string KIL;
            private:
                static const std::string TEX;
            private:
                static const std::string SGE;
            private:
                static const std::string SLT;
            private:
                static const std::string SGN;
            private:
                static const std::string SEQ;
            private:
                static const std::string SNE;

                // registers
            private:
                static const std::string VA;
            private:
                static const std::string VC;
            private:
                static const std::string VT;
            private:
                static const std::string VO;
            private:
                static const std::string VI;
            private:
                static const std::string FC;
            private:
                static const std::string FT;
            private:
                static const std::string FS;
            private:
                static const std::string FO;
            private:
                static const std::string FD;

                // samplers
            private:
                static const std::string D2;
            private:
                static const std::string D3;
            private:
                static const std::string CUBE;
            private:
                static const std::string MIPNEAREST;
            private:
                static const std::string MIPLINEAR;
            private:
                static const std::string MIPNONE;
            private:
                static const std::string NOMIP;
            private:
                static const std::string NEAREST;
            private:
                static const std::string LINEAR;
            private:
                static const std::string CENTROID;
            private:
                static const std::string SINGLE;
            private:
                static const std::string IGNORESAMPLER;
            private:
                static const std::string REPEAT;
            private:
                static const std::string WRAP;
            private:
                static const std::string CLAMP;
            private:
                static const std::string RGBA;
            private:
                static const std::string DXT1;
            private:
                static const std::string DXT5;
            private:
                static const std::string VIDEO;
            };
        }
    }
}
// ================================================================================//  Helper Classes
// --------------------------------------------------------------------------------

// ===========================================================================
//  Class
// ---------------------------------------------------------------------------
class OpCode
{
    // ======================================================================
    //  Properties
    // ----------------------------------------------------------------------
private:
    unsigned int _emitCode;
private:
    unsigned int _flags;
private:
    std::string _name;
private:
    unsigned int _numRegister;

    // ======================================================================
    //  Getters
    // ----------------------------------------------------------------------
public:
    unsigned int         emitCode();
public:
    unsigned int         flags();
public:
    std::string       name();
public:
    unsigned int         numRegister();

    // ======================================================================
    //  Constructor
    // ----------------------------------------------------------------------
public:
    OpCode( std::string name, unsigned int numRegister, unsigned int emitCode, unsigned int flags);

    // ======================================================================
    //  Methods
    // ----------------------------------------------------------------------
public:
    std::string   toString();
};

// ===========================================================================
//  Class
// ---------------------------------------------------------------------------
class Register
{
    // ======================================================================
    //  Properties
    // ----------------------------------------------------------------------
private:
    unsigned int _emitCode;
private:
    std::string _name;
private:
    std::string _longName;
private:
    unsigned int _flags;
private:
    unsigned int _range;

    // ======================================================================
    //  Getters
    // ----------------------------------------------------------------------
public:
    unsigned int         emitCode();
public:
    std::string       longName();
public:
    std::string       name();
public:
    unsigned int         flags();
public:
    unsigned int         range();

    // ======================================================================
    //  Constructor
    // ----------------------------------------------------------------------
public:
    Register( std::string name, std::string longName, unsigned int emitCode, unsigned int range, unsigned int flags);

    // ======================================================================
    //  Methods
    // ----------------------------------------------------------------------
public:
    std::string   toString();
};

// ===========================================================================
//  Class
// ---------------------------------------------------------------------------
class Sampler
{
    // ======================================================================
    //  Properties
    // ----------------------------------------------------------------------
private:
    unsigned int _flag;
private:
    unsigned int _mask;
private:
    std::string _name;

    // ======================================================================
    //  Getters
    // ----------------------------------------------------------------------
public:
    unsigned int         flag();
public:
    unsigned int         mask();
public:
    std::string       name();

    // ======================================================================
    //  Constructor
    // ----------------------------------------------------------------------
public:
    Sampler( std::string name, unsigned int flag, unsigned int mask);

    // ======================================================================
    //  Methods
    // ----------------------------------------------------------------------
public:
    std::string   toString();
};
#endif // __STARLING_SRC_COM_ADOBE_UTILS_AGALMINIASSEMBLER_AS
#endif // __cplusplus

