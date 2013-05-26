#if !defined(__SAMPLES_DEMO_WEB_SRC_EMBEDDEDASSETS_AS)
#define __SAMPLES_DEMO_WEB_SRC_EMBEDDEDASSETS_AS
#if defined(__cplusplus)



#include "flex11.6.h"



class EmbeddedAssets
{
    /** ATTENTION: Naming conventions!
     *
     *  - Classes for embedded IMAGES should have the exact same name as the file,
     *    without extension. This is required so that references from XMLs (atlas, bitmap font)
     *    won't break.
     *
     *  - Atlas and Font XML files can have an arbitrary name, since they are never
     *    referenced by file name.
     *
     */

    // Texture Atlas


public:
    static const Class *atlas_xml;


public:
    static const Class *atlas;

    // Compressed textures


public:
    static const Class *compressed_texture;

    // Bitmap Fonts


public:
    static const Class *desyrel_fnt;


public:
    static const Class *desyrel;

    // Sounds


public:
    static const Class *wing_flap;
};

#endif // __SAMPLES_DEMO_WEB_SRC_EMBEDDEDASSETS_AS
#endif // __cplusplus

