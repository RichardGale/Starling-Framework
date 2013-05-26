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
#include "AGALMiniAssembler.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Program3D.h"
#include "flash/utils/ByteArray.h"
#include "flash/utils/Endian.h"
#include "flash/utils/getTimer.h"

    // ===========================================================================
    //  Class
    // ---------------------------------------------------------------------------

using namespace flash::display3D;
using namespace flash::utils;

namespace com {
namespace adobe {
namespace utils {

            // ======================================================================
        //  Constants
        // ----------------------------------------------------------------------               
        const RegExp* AGALMiniAssembler::REGEXP_OUTER_SPACES       = new RegExp("//^\\s+|\\s+$//g");

        // ======================================================================
        //  Properties
        // ----------------------------------------------------------------------
        // AGAL bytes and error buffer 
                    
                    

                    

        bool AGALMiniAssembler::initialized                  = false;
                   

        // ======================================================================
        //  Getters
        // ----------------------------------------------------------------------
        std::string AGALMiniAssembler::error()                             { return _error; }
        ByteArray* AGALMiniAssembler::agalcode()                          { return _agalcode; }

        // ======================================================================
        //  Constructor
        // ----------------------------------------------------------------------
        AGALMiniAssembler::AGALMiniAssembler( bool debugging)
        {
            debugEnabled = debugging;
            if ( !initialized )
                init();
        }
        // ======================================================================
        //  Methods
        // ----------------------------------------------------------------------

        Program3D* AGALMiniAssembler::assemble2( Context3D* ctx3d, unsigned int version, std::string vertexsrc, std::string fragmentsrc)
        {
            ByteArray* agalvertex   = assemble ( VERTEX, vertexsrc, version );
            ByteArray* agalfragment   = assemble ( FRAGMENT, fragmentsrc, version );
            Program3D* prog   = ctx3d->createProgram();
            prog->upload(agalvertex,agalfragment);
            return prog;
        }

        ByteArray* AGALMiniAssembler::assemble( std::string mode, std::string source, unsigned int version, bool ignorelimits)
        {
            unsigned int start = getTimer();

            _agalcode                           = new ByteArray();
            _error = "";

            bool isFrag = false;

            if ( mode == FRAGMENT )
                isFrag = true;
            else if ( mode != VERTEX )
                _error = "ERROR: mode needs to be /"" + FRAGMENT + "/" or /"" + VERTEX + "/" but is /"" + mode + "/".";

            agalcode()->endian ( Endian::LITTLE_ENDIAN);
            agalcode()->writeByte( 0xa0 );             // tag version
            agalcode()->writeUnsignedInt( version );       // AGAL version, big endian, bit pattern will be 0x01000000
            agalcode()->writeByte( 0xa1 );             // tag program id
            agalcode()->writeByte( isFrag ? 1 : 0 );   // vertex or fragment

            initregmap(version, ignorelimits);

            std::vector<void*> lines = source.replace( "//[\\f\\n\\r\\v]+//g", "\\n" )->split( "\\n" );
            int nest = 0;
            int nops = 0;
            int i;
            int lng = lines.size();

            for ( i = 0; i < lng && _error == ""; i++ )
            {
                std::string line = ""                      ;
                line = line.replace( REGEXP_OUTER_SPACES, "" );

                // remove comments
                int startcomment = line.search( "////" );
                if ( startcomment != -1 )
                    line = line.slice( 0, startcomment );

                // grab options
                int optsi = line.search( "//<.*>//g" );
                std::vector<void*> opts;
                if ( optsi != -1 )
                {
                    opts = line.slice( optsi )->match( "//([\\w\\.\\-\\+]+)//gi" );
                    line = line.slice( 0, optsi );
                }

                // find opcode
                std::vector<void*> opCode = line.match( "//^\\w{3}//ig" );
                if ( !!opCode.empty() )
                {
                    if ( line.length() >= 3 )
                        trace( "warning: bad line "+i+": "+lines[i] );
                    continue;
                }
                OpCode* opFound = OPMAP[ opCode[0] ];

                // if debug is enabled, output the opcodes
                if ( debugEnabled )
                    trace( opFound );

                if ( opFound == NULL )
                {
                    if ( line.length() >= 3 )
                        trace( "warning: bad line "+i+": "+lines[i] );
                    continue;
                }

                line = line.slice( line.search( opFound->name() ) + opFound->name()->length() );

                if ( ( opFound->flags() & OP_VERSION2 ) && version<2 )
                {
                    _error = "error: opcode requires version 2.";
                    break;
                }

                if ( ( opFound->flags() & OP_VERT_ONLY ) && isFrag )
                {
                    _error = "error: opcode is only allowed in vertex programs.";
                    break;
                }

                if ( ( opFound->flags() & OP_FRAG_ONLY ) && !isFrag )
                {
                    _error = "error: opcode is only allowed in fragment programs.";
                    break;
                }
                if ( verbose )
                    trace( "emit opcode=" + opFound );

                agalcode()->writeUnsignedInt( opFound->emitCode() );
                nops++;

                if ( nops > MAX_OPCODES )
                {
                    _error = "error: too many opcodes. maximum is "+MAX_OPCODES+".";
                    break;
                }

                // get operands, use regexp
                std::vector<void*> regs;

                // will match both syntax
                regs = line.match( "//vc\\[([vof][acostdip]?)(\\d*)?(\\.[xyzw](\\+\\d{1,3})?)?\\](\\.[xyzw]{1,4})?|([vof][acostdip]?)(\\d*)?(\\.[xyzw]{1,4})?//gi" );

                if ( !regs || regs.size() != opFound->numRegister() )
                {
                    _error = "error: wrong number of operands. found "+regs.size()+" but expected "+opFound->numRegister()+".";
                    break;
                }

                bool badreg  = false;
                unsigned int pad        = 64 + 64 + 32;
                unsigned int regLength  = regs.size();

                for ( int j = 0; j < regLength; j++ )
                {
                    bool isRelative = false;
                    std::vector<void*> relreg = regs[ j ]->match( "//\\[.*\\]//ig" );
                    if ( relreg && relreg.size() > 0 )
                    {
                        regs[ j ] = regs[ j ]->replace( relreg[ 0 ], "0" );

                        if ( verbose )
                            trace( "IS REL" );
                        isRelative = true;
                    }

                    std::vector<void*> res = regs[j]->match( "//^\\b[A-Za-z]{1,2}//ig" );
                    if ( !!res.empty() )
                    {
                        _error = "error: could not parse operand "+j+" ("+regs[j]+").";
                        badreg = true;
                        break;
                    }
                    Register* regFound = REGMAP[ res[ 0 ] ];

                    // if debug is enabled, output the registers
                    if ( debugEnabled )
                        trace( regFound );

                    if ( regFound == NULL )
                    {
                        _error = "error: could not find register name for operand "+j+" ("+regs[j]+").";
                        badreg = true;
                        break;
                    }

                    if ( isFrag )
                    {
                        if ( !( regFound->flags() & REG_FRAG ) )
                        {
                            _error = "error: register operand "+j+" ("+regs[j]+") only allowed in vertex programs.";
                            badreg = true;
                            break;
                        }
                        if ( isRelative )
                        {
                            _error = "error: register operand "+j+" ("+regs[j]+") relative adressing not allowed in fragment programs.";
                            badreg = true;
                            break;
                        }
                    }
                    else
                    {
                        if ( !( regFound->flags() & REG_VERT ) )
                        {
                            _error = "error: register operand "+j+" ("+regs[j]+") only allowed in fragment programs.";
                            badreg = true;
                            break;
                        }
                    }

                    regs[j] = regs[j]->slice( regs[j]->search( regFound->name() ) + regFound->name()->length() );
                    //trace( "REGNUM: " +regs[j] );
                    std::vector<void*> idxmatch = isRelative ? relreg[0]->match( "//\\d+//" ) : regs[j]->match( "//\\d+//" );
                    unsigned int regidx = 0;

                    if ( idxmatch )
                        regidx = unsigned int( idxmatch[0] );

                    if ( regFound->range() < regidx )
                    {
                        _error = "error: register operand "+j+" ("+regs[j]+") index exceeds limit of "+(regFound->range()+1)+".";
                        badreg = true;
                        break;
                    }

                    unsigned int regmask        = 0;
                    std::vector<void*> maskmatch     = regs[j]->match( "//(\\.[xyzw]{1,4})//" );
                    bool isDest      = ( j == 0 && !( opFound->flags() & OP_NO_DEST ) );
                    bool isSampler   = ( j == 2 && ( opFound->flags() & OP_SPECIAL_TEX ) );
                    unsigned int reltype        = 0;
                    unsigned int relsel         = 0;
                    int reloffset       = 0;

                    if ( isDest && isRelative )
                    {
                        _error = "error: relative can not be destination";
                        badreg = true;
                        break;
                    }

                    if ( maskmatch )
                    {
                        regmask = 0;
                        unsigned int cv;
                        unsigned int maskLength = maskmatch[0]->length();
                        for ( int k = 1; k < maskLength; k++ )
                        {
                            cv = maskmatch[0]->charCodeAt(k) - "x"()->charCodeAt(0);
                            if ( cv > 2 )
                                cv = 3;
                            if ( isDest )
                                regmask |= 1 << cv;
                            else
                                regmask |= cv << ( ( k - 1 ) << 1 );
                        }
                        if ( !isDest )
                            for ( ; k <= 4; k++ )
                                regmask |= cv << ( ( k - 1 ) << 1 ); // repeat last                             
                    }
                    else
                    {
                        regmask = isDest ? 0xf : 0xe4; // id swizzle or mask                        
                    }

                    if ( isRelative )
                    {
                        std::vector<void*> relname = relreg[0]->match( "//[A-Za-z]{1,2}//ig" );
                        Register* regFoundRel = REGMAP[ relname[0]];
                        if ( regFoundRel == NULL )
                        {
                            _error = "error: bad index register";
                            badreg = true;
                            break;
                        }
                        reltype = regFoundRel->emitCode();
                        std::vector<void*> selmatch = relreg[0]->match( "//(\\.[xyzw]{1,1})//" );
                        if ( selmatch.size()==0 )
                        {
                            _error = "error: bad index register select";
                            badreg = true;
                            break;
                        }
                        relsel = selmatch[0]->charCodeAt(1) - "x"()->charCodeAt(0);
                        if ( relsel > 2 )
                            relsel = 3;
                        std::vector<void*> relofs = relreg[0]->match( "//\\+\\d{1,3}//ig" );
                        if ( relofs.size() > 0 )
                            reloffset = relofs[0];
                        if ( reloffset < 0 || reloffset > 255 )
                        {
                            _error = "error: index offset "+reloffset+" out of bounds. [0..255]";
                            badreg = true;
                            break;
                        }
                        if ( verbose )
                            trace( "RELATIVE: type="+reltype+"=="+relname[0]+" sel="+relsel+"=="+selmatch[0]+" idx="+regidx+" offset="+reloffset );
                    }

                    if ( verbose )
                        trace( "  emit argcode="+regFound+"["+regidx+"]["+regmask+"]" );
                    if ( isDest )
                    {
                        agalcode()->writeShort( regidx );
                        agalcode()->writeByte( regmask );
                        agalcode()->writeByte( regFound->emitCode() );
                        pad -= 32;
                    } else
                    {
                        if ( isSampler )
                        {
                            if ( verbose )
                                trace( "  emit sampler" );
                            unsigned int samplerbits = 5; // type 5 
                            unsigned int optsLength = opts.empty() ? 0 : opts.size();
                            float bias = 0;
                            for ( k = 0; k<optsLength; k++ )
                            {
                                if ( verbose )
                                    trace( "    opt: "+opts[k] );
                                Sampler* optfound = SAMPLEMAP [opts[k]];
                                if ( optfound == NULL )
                                {
                                    // todo check that it's a number...
                                    //trace( "Warning, unknown sampler option: "+opts[k] );
                                    bias = float(opts[k]);
                                    if ( verbose )
                                        trace( "    bias: " + bias );
                                }
                                else
                                {
                                    if ( optfound->flag() != SAMPLER_SPECIAL_SHIFT )
                                        samplerbits &= ~( 0xf << optfound->flag() );
                                    samplerbits |= unsigned int( optfound->mask() ) << unsigned int( optfound->flag() );
                                }
                            }
                            agalcode()->writeShort( regidx );
                            agalcode()->writeByte(int(bias*8.0));
                            agalcode()->writeByte(0);
                            agalcode()->writeUnsignedInt( samplerbits );

                            if ( verbose )
                                trace( "    bits: " + ( samplerbits - 5 ) );
                            pad -= 64;
                        }
                        else
                        {
                            if ( j == 0 )
                            {
                                agalcode()->writeUnsignedInt( 0 );
                                pad -= 32;
                            }
                            agalcode()->writeShort( regidx );
                            agalcode()->writeByte( reloffset );
                            agalcode()->writeByte( regmask );
                            agalcode()->writeByte( regFound->emitCode() );
                            agalcode()->writeByte( reltype );
                            agalcode()->writeShort( isRelative ? ( relsel | ( 1 << 15 ) ) : 0 );

                            pad -= 64;
                        }
                    }
                }

                // pad unused regs
                for ( j = 0; j < pad; j += 8 )
                    agalcode()->writeByte( 0 );

                if ( badreg )
                    break;
            }

            if ( _error != "" )
            {
                _error += "\\n  at line " + i + " " + lines[i];
                agalcode()->clear()    ;
                trace( _error );
            }

            // trace the bytecode bytes if debugging is enabled
            if ( debugEnabled )
            {
                std::string dbgLine = "generated bytecode:";
                unsigned int agalLength = agalcode()->length();
                for ( unsigned int index = 0; index < agalLength; index++ )
                {
                    if ( !( index % 16 ) )
                        dbgLine += "\\n";
                    if ( !( index % 4 ) )
                        dbgLine += " ";

                    std::string byteStr = agalcode[ index ]->toString( 16 );
                    if ( byteStr.length() < 2 )
                        byteStr = "0" + byteStr;

                    dbgLine += byteStr;
                }
                trace( dbgLine );
            }

            if ( verbose )
                trace( "AGALMiniAssembler.assemble time: " + ( ( getTimer() - start ) / 1000 ) + "s" );

            return agalcode;
        }

        void AGALMiniAssembler::initregmap( unsigned int version, bool ignorelimits)        {
            // version changes limits               
            REGMAP[ VA ]    = new Register( VA, "vertex attribute",     0x0,    ignorelimits?1024:7,                        REG_VERT | REG_READ );
            REGMAP[ VC ]    = new Register( VC, "vertex constant",      0x1,    ignorelimits?1024:(version==1?127:250),     REG_VERT | REG_READ );
            REGMAP[ VT ]    = new Register( VT, "vertex temporary",     0x2,    ignorelimits?1024:(version==1?7:27),        REG_VERT | REG_WRITE | REG_READ );
            REGMAP[ VO ]    = new Register( VO, "vertex output",        0x3,    ignorelimits?1024:0,                        REG_VERT | REG_WRITE );
            REGMAP[ VI ]    = new Register( VI, "varying",              0x4,    ignorelimits?1024:(version==1?7:11),        REG_VERT | REG_FRAG | REG_READ | REG_WRITE );
            REGMAP[ FC ]    = new Register( FC, "fragment constant",    0x1,    ignorelimits?1024:(version==1?27:63),       REG_FRAG | REG_READ );
            REGMAP[ FT ]    = new Register( FT, "fragment temporary",   0x2,    ignorelimits?1024:(version==1?7:27),        REG_FRAG | REG_WRITE | REG_READ );
            REGMAP[ FS ]    = new Register( FS, "texture sampler",      0x5,    ignorelimits?1024:7,                        REG_FRAG | REG_READ );
            REGMAP[ FO ]    = new Register( FO, "fragment output",      0x3,    ignorelimits?1024:(version==1?0:3),         REG_FRAG | REG_WRITE );
            REGMAP[ FD ]    = new Register( FD, "fragment depth output",0x6,    ignorelimits?1024:(version==1?-1:0),        REG_FRAG | REG_WRITE );

            // aliases
            REGMAP[ "op" ]  = REGMAP[ VO ];
            REGMAP[ "i" ]   = REGMAP[ VI ];
            REGMAP[ "v" ]   = REGMAP[ VI ];
            REGMAP[ "oc" ]  = REGMAP[ FO ];
            REGMAP[ "od" ]  = REGMAP[ FD ];
            REGMAP[ "fi" ]  = REGMAP[ VI ];
        }

        void AGALMiniAssembler::init()
        {
            initialized = true;

            // Fill the dictionaries with opcodes and registers
            OPMAP[ MOV ] = new OpCode( MOV, 2, 0x00, 0 );
            OPMAP[ ADD ] = new OpCode( ADD, 3, 0x01, 0 );
            OPMAP[ SUB ] = new OpCode( SUB, 3, 0x02, 0 );
            OPMAP[ MUL ] = new OpCode( MUL, 3, 0x03, 0 );
            OPMAP[ DIV ] = new OpCode( DIV, 3, 0x04, 0 );
            OPMAP[ RCP ] = new OpCode( RCP, 2, 0x05, 0 );
            OPMAP[ MIN ] = new OpCode( MIN, 3, 0x06, 0 );
            OPMAP[ MAX ] = new OpCode( MAX, 3, 0x07, 0 );
            OPMAP[ FRC ] = new OpCode( FRC, 2, 0x08, 0 );
            OPMAP[ SQT ] = new OpCode( SQT, 2, 0x09, 0 );
            OPMAP[ RSQ ] = new OpCode( RSQ, 2, 0x0a, 0 );
            OPMAP[ POW ] = new OpCode( POW, 3, 0x0b, 0 );
            OPMAP[ LOG ] = new OpCode( LOG, 2, 0x0c, 0 );
            OPMAP[ EXP ] = new OpCode( EXP, 2, 0x0d, 0 );
            OPMAP[ NRM ] = new OpCode( NRM, 2, 0x0e, 0 );
            OPMAP[ SIN ] = new OpCode( SIN, 2, 0x0f, 0 );
            OPMAP[ COS ] = new OpCode( COS, 2, 0x10, 0 );
            OPMAP[ CRS ] = new OpCode( CRS, 3, 0x11, 0 );
            OPMAP[ DP3 ] = new OpCode( DP3, 3, 0x12, 0 );
            OPMAP[ DP4 ] = new OpCode( DP4, 3, 0x13, 0 );
            OPMAP[ ABS ] = new OpCode( ABS, 2, 0x14, 0 );
            OPMAP[ NEG ] = new OpCode( NEG, 2, 0x15, 0 );
            OPMAP[ SAT ] = new OpCode( SAT, 2, 0x16, 0 );
            OPMAP[ M33 ] = new OpCode( M33, 3, 0x17, OP_SPECIAL_MATRIX );
            OPMAP[ M44 ] = new OpCode( M44, 3, 0x18, OP_SPECIAL_MATRIX );
            OPMAP[ M34 ] = new OpCode( M34, 3, 0x19, OP_SPECIAL_MATRIX );
            OPMAP[ DDX ] = new OpCode( DDX, 2, 0x1a, OP_VERSION2 | OP_FRAG_ONLY );
            OPMAP[ DDY ] = new OpCode( DDY, 2, 0x1b, OP_VERSION2 | OP_FRAG_ONLY );
            OPMAP[ IFE ] = new OpCode( IFE, 2, 0x1c, OP_NO_DEST | OP_VERSION2 | OP_INCNEST | OP_SCALAR );
            OPMAP[ INE ] = new OpCode( INE, 2, 0x1d, OP_NO_DEST | OP_VERSION2 | OP_INCNEST | OP_SCALAR );
            OPMAP[ IFG ] = new OpCode( IFG, 2, 0x1e, OP_NO_DEST | OP_VERSION2 | OP_INCNEST | OP_SCALAR );
            OPMAP[ IFL ] = new OpCode( IFL, 2, 0x1f, OP_NO_DEST | OP_VERSION2 | OP_INCNEST | OP_SCALAR );
            OPMAP[ ELS ] = new OpCode( ELS, 0, 0x20, OP_NO_DEST | OP_VERSION2 | OP_INCNEST | OP_DECNEST | OP_SCALAR );
            OPMAP[ EIF ] = new OpCode( EIF, 0, 0x21, OP_NO_DEST | OP_VERSION2 | OP_DECNEST | OP_SCALAR );
            // space            
            OPMAP[ TED ] = new OpCode( TED, 3, 0x26, OP_FRAG_ONLY | OP_SPECIAL_TEX | OP_VERSION2);
            OPMAP[ KIL ] = new OpCode( KIL, 1, 0x27, OP_NO_DEST | OP_FRAG_ONLY );
            OPMAP[ TEX ] = new OpCode( TEX, 3, 0x28, OP_FRAG_ONLY | OP_SPECIAL_TEX );
            OPMAP[ SGE ] = new OpCode( SGE, 3, 0x29, 0 );
            OPMAP[ SLT ] = new OpCode( SLT, 3, 0x2a, 0 );
            OPMAP[ SGN ] = new OpCode( SGN, 2, 0x2b, 0 );
            OPMAP[ SEQ ] = new OpCode( SEQ, 3, 0x2c, 0 );
            OPMAP[ SNE ] = new OpCode( SNE, 3, 0x2d, 0 );


            SAMPLEMAP[ RGBA ]       = new Sampler( RGBA,        SAMPLER_TYPE_SHIFT,         0 );
            SAMPLEMAP[ DXT1 ]       = new Sampler( DXT1,        SAMPLER_TYPE_SHIFT,         1 );
            SAMPLEMAP[ DXT5 ]       = new Sampler( DXT5,        SAMPLER_TYPE_SHIFT,         2 );
            SAMPLEMAP[ VIDEO ]      = new Sampler( VIDEO,       SAMPLER_TYPE_SHIFT,         3 );
            SAMPLEMAP[ D2 ]         = new Sampler( D2,          SAMPLER_DIM_SHIFT,          0 );
            SAMPLEMAP[ D3 ]         = new Sampler( D3,          SAMPLER_DIM_SHIFT,          2 );
            SAMPLEMAP[ CUBE ]       = new Sampler( CUBE,        SAMPLER_DIM_SHIFT,          1 );
            SAMPLEMAP[ MIPNEAREST ] = new Sampler( MIPNEAREST,  SAMPLER_MIPMAP_SHIFT,       1 );
            SAMPLEMAP[ MIPLINEAR ]  = new Sampler( MIPLINEAR,   SAMPLER_MIPMAP_SHIFT,       2 );
            SAMPLEMAP[ MIPNONE ]    = new Sampler( MIPNONE,     SAMPLER_MIPMAP_SHIFT,       0 );
            SAMPLEMAP[ NOMIP ]      = new Sampler( NOMIP,       SAMPLER_MIPMAP_SHIFT,       0 );
            SAMPLEMAP[ NEAREST ]    = new Sampler( NEAREST,     SAMPLER_FILTER_SHIFT,       0 );
            SAMPLEMAP[ LINEAR ]     = new Sampler( LINEAR,      SAMPLER_FILTER_SHIFT,       1 );
            SAMPLEMAP[ CENTROID ]   = new Sampler( CENTROID,    SAMPLER_SPECIAL_SHIFT,      1 << 0 );
            SAMPLEMAP[ SINGLE ]     = new Sampler( SINGLE,      SAMPLER_SPECIAL_SHIFT,      1 << 1 );
            SAMPLEMAP[ IGNORESAMPLER ]  = new Sampler( IGNORESAMPLER,       SAMPLER_SPECIAL_SHIFT,      1 << 2 );
            SAMPLEMAP[ REPEAT ]     = new Sampler( REPEAT,      SAMPLER_REPEAT_SHIFT,       1 );
            SAMPLEMAP[ WRAP ]       = new Sampler( WRAP,        SAMPLER_REPEAT_SHIFT,       1 );
            SAMPLEMAP[ CLAMP ]      = new Sampler( CLAMP,       SAMPLER_REPEAT_SHIFT,       0 );
        }

        // ======================================================================
        //  Constants
        // ----------------------------------------------------------------------
        const std::map<std::string, void*> AGALMiniAssembler::OPMAP                     std::map<std::string, void*>()                ;
        const std::map<std::string, void*> AGALMiniAssembler::REGMAP                    std::map<std::string, void*>()                ;
        const std::map<std::string, void*> AGALMiniAssembler::SAMPLEMAP                 std::map<std::string, void*>()                ;

        const int AGALMiniAssembler::MAX_NESTING                    = 4;
        const int AGALMiniAssembler::MAX_OPCODES                    = 2048;

        const std::string AGALMiniAssembler::FRAGMENT                    = "fragment";
        const std::string AGALMiniAssembler::VERTEX                      = "vertex";

        // masks and shifts
        const unsigned int AGALMiniAssembler::SAMPLER_TYPE_SHIFT            = 8;
        const unsigned int AGALMiniAssembler::SAMPLER_DIM_SHIFT             = 12;
        const unsigned int AGALMiniAssembler::SAMPLER_SPECIAL_SHIFT         = 16;
        const unsigned int AGALMiniAssembler::SAMPLER_REPEAT_SHIFT          = 20;
        const unsigned int AGALMiniAssembler::SAMPLER_MIPMAP_SHIFT          = 24;
        const unsigned int AGALMiniAssembler::SAMPLER_FILTER_SHIFT          = 28;

        // regmap flags
        const unsigned int AGALMiniAssembler::REG_WRITE                     = 0x1;
        const unsigned int AGALMiniAssembler::REG_READ                      = 0x2;
        const unsigned int AGALMiniAssembler::REG_FRAG                      = 0x20;
        const unsigned int AGALMiniAssembler::REG_VERT                      = 0x40;

        // opmap flags
        const unsigned int AGALMiniAssembler::OP_SCALAR                     = 0x1;
        const unsigned int AGALMiniAssembler::OP_SPECIAL_TEX                = 0x8;
        const unsigned int AGALMiniAssembler::OP_SPECIAL_MATRIX             = 0x10;
        const unsigned int AGALMiniAssembler::OP_FRAG_ONLY                  = 0x20;
        const unsigned int AGALMiniAssembler::OP_VERT_ONLY                  = 0x40;
        const unsigned int AGALMiniAssembler::OP_NO_DEST                    = 0x80;
        const unsigned int AGALMiniAssembler::OP_VERSION2                   = 0x100;
        const unsigned int AGALMiniAssembler::OP_INCNEST                    = 0x200;
        const unsigned int AGALMiniAssembler::OP_DECNEST                    = 0x400;

        // opcodes
        const std::string AGALMiniAssembler::MOV                         = "mov";
        const std::string AGALMiniAssembler::ADD                         = "add";
        const std::string AGALMiniAssembler::SUB                         = "sub";
        const std::string AGALMiniAssembler::MUL                         = "mul";
        const std::string AGALMiniAssembler::DIV                         = "div";
        const std::string AGALMiniAssembler::RCP                         = "rcp";
        const std::string AGALMiniAssembler::MIN                         = "min";
        const std::string AGALMiniAssembler::MAX                         = "max";
        const std::string AGALMiniAssembler::FRC                         = "frc";
        const std::string AGALMiniAssembler::SQT                         = "sqt";
        const std::string AGALMiniAssembler::RSQ                         = "rsq";
        const std::string AGALMiniAssembler::POW                         = "pow";
        const std::string AGALMiniAssembler::LOG                         = "log";
        const std::string AGALMiniAssembler::EXP                         = "exp";
        const std::string AGALMiniAssembler::NRM                         = "nrm";
        const std::string AGALMiniAssembler::SIN                         = "sin";
        const std::string AGALMiniAssembler::COS                         = "cos";
        const std::string AGALMiniAssembler::CRS                         = "crs";
        const std::string AGALMiniAssembler::DP3                         = "dp3";
        const std::string AGALMiniAssembler::DP4                         = "dp4";
        const std::string AGALMiniAssembler::ABS                         = "abs";
        const std::string AGALMiniAssembler::NEG                         = "neg";
        const std::string AGALMiniAssembler::SAT                         = "sat";
        const std::string AGALMiniAssembler::M33                         = "m33";
        const std::string AGALMiniAssembler::M44                         = "m44";
        const std::string AGALMiniAssembler::M34                         = "m34";
        const std::string AGALMiniAssembler::DDX                         = "ddx";
        const std::string AGALMiniAssembler::DDY                         = "ddy";
        const std::string AGALMiniAssembler::IFE                         = "ife";
        const std::string AGALMiniAssembler::INE                         = "ine";
        const std::string AGALMiniAssembler::IFG                         = "ifg";
        const std::string AGALMiniAssembler::IFL                         = "ifl";
        const std::string AGALMiniAssembler::ELS                         = "els";
        const std::string AGALMiniAssembler::EIF                         = "eif";
        const std::string AGALMiniAssembler::TED                         = "ted";
        const std::string AGALMiniAssembler::KIL                         = "kil";
        const std::string AGALMiniAssembler::TEX                         = "tex";
        const std::string AGALMiniAssembler::SGE                         = "sge";
        const std::string AGALMiniAssembler::SLT                         = "slt";
        const std::string AGALMiniAssembler::SGN                         = "sgn";
        const std::string AGALMiniAssembler::SEQ                         = "seq";
        const std::string AGALMiniAssembler::SNE                         = "sne";

        // registers
        const std::string AGALMiniAssembler::VA                          = "va";
        const std::string AGALMiniAssembler::VC                          = "vc";
        const std::string AGALMiniAssembler::VT                          = "vt";
        const std::string AGALMiniAssembler::VO                          = "vo";
        const std::string AGALMiniAssembler::VI                          = "vi";
        const std::string AGALMiniAssembler::FC                          = "fc";
        const std::string AGALMiniAssembler::FT                          = "ft";
        const std::string AGALMiniAssembler::FS                          = "fs";
        const std::string AGALMiniAssembler::FO                          = "fo";
        const std::string AGALMiniAssembler::FD                          = "fd";

        // samplers
        const std::string AGALMiniAssembler::D2                          = "2d";
        const std::string AGALMiniAssembler::D3                          = "3d";
        const std::string AGALMiniAssembler::CUBE                        = "cube";
        const std::string AGALMiniAssembler::MIPNEAREST                  = "mipnearest";
        const std::string AGALMiniAssembler::MIPLINEAR                   = "miplinear";
        const std::string AGALMiniAssembler::MIPNONE                     = "mipnone";
        const std::string AGALMiniAssembler::NOMIP                       = "nomip";
        const std::string AGALMiniAssembler::NEAREST                     = "nearest";
        const std::string AGALMiniAssembler::LINEAR                      = "linear";
        const std::string AGALMiniAssembler::CENTROID                    = "centroid";
        const std::string AGALMiniAssembler::SINGLE                      = "single";
        const std::string AGALMiniAssembler::IGNORESAMPLER               = "ignoresampler";
        const std::string AGALMiniAssembler::REPEAT                      = "repeat";
        const std::string AGALMiniAssembler::WRAP                        = "wrap";
        const std::string AGALMiniAssembler::CLAMP                       = "clamp";
        const std::string AGALMiniAssembler::RGBA                        = "rgba";
        const std::string AGALMiniAssembler::DXT1                        = "dxt1";
        const std::string AGALMiniAssembler::DXT5                        = "dxt5";
        const std::string AGALMiniAssembler::VIDEO                       = "video";
}
}
}
// ================================================================================
//  Helper Classes
// --------------------------------------------------------------------------------

    // ===========================================================================
    //  Class
    // ---------------------------------------------------------------------------


        // ======================================================================
        //  Properties
        // ----------------------------------------------------------------------
                    
                    
                    
                    

        // ======================================================================
        //  Getters
        // ----------------------------------------------------------------------
        unsigned int OpCode::emitCode()          { return _emitCode; }
        unsigned int OpCode::flags()             { return _flags; }
        std::string OpCode::name()              { return _name; }
        unsigned int OpCode::numRegister()       { return _numRegister; }

        // ======================================================================
        //  Constructor
        // ----------------------------------------------------------------------
        OpCode::OpCode( std::string name, unsigned int numRegister, unsigned int emitCode, unsigned int flags)
        {
            _name = name;
            _numRegister = numRegister;
            _emitCode = emitCode;
            _flags = flags;
        }

        // ======================================================================
        //  Methods
        // ----------------------------------------------------------------------
        std::string OpCode::toString()
        {
            return "[OpCode name=\\/"+_name+"\\/, numRegister="+_numRegister+", emitCode="+_emitCode+", flags="+_flags+"]";
        }


    // ===========================================================================
    //  Class
    // ---------------------------------------------------------------------------


        // ======================================================================
        //  Properties
        // ----------------------------------------------------------------------
                    
                    
                    
                    
                    

        // ======================================================================
        //  Getters
        // ----------------------------------------------------------------------
        unsigned int Register::emitCode()          { return _emitCode; }
        std::string Register::longName()          { return _longName; }
        std::string Register::name()              { return _name; }
        unsigned int Register::flags()             { return _flags; }
        unsigned int Register::range()             { return _range; }

        // ======================================================================
        //  Constructor
        // ----------------------------------------------------------------------
        Register::Register( std::string name, std::string longName, unsigned int emitCode, unsigned int range, unsigned int flags)
        {
            _name = name;
            _longName = longName;
            _emitCode = emitCode;
            _range = range;
            _flags = flags;
        }

        // ======================================================================
        //  Methods
        // ----------------------------------------------------------------------
        std::string Register::toString()
        {
            return "[Register name=\\/"+_name+"\\/, longName=\\/"+_longName+"\\/, emitCode="+_emitCode+", range="+_range+", flags="+ _flags+"]";
        }


    // ===========================================================================
    //  Class
    // ---------------------------------------------------------------------------


        // ======================================================================
        //  Properties
        // ----------------------------------------------------------------------
                    
                    
                    

        // ======================================================================
        //  Getters
        // ----------------------------------------------------------------------
        unsigned int Sampler::flag()          { return _flag; }
        unsigned int Sampler::mask()          { return _mask; }
        std::string Sampler::name()          { return _name; }

        // ======================================================================
        //  Constructor
        // ----------------------------------------------------------------------
        Sampler::Sampler( std::string name, unsigned int flag, unsigned int mask)
        {
            _name = name;
            _flag = flag;
            _mask = mask;
        }

        // ======================================================================
        //  Methods
        // ----------------------------------------------------------------------
        std::string Sampler::toString()
        {
            return "[Sampler name=\\/"+_name+"\\/, flag=\\/"+_flag+"\\/, mask="+mask+"]";
        }
