

#include "Constants.h"
#include "starling/errors/AbstractClassError.h"

using namespace starling::errors;



        Constants::Constants() { throw new AbstractClassError(); }

        // We chose this stage size because it is used by many mobile devices; 
        // it's e.g. the resolution of the iPhone (non-retina), which means that your game
        // will be displayed without any black bars on all iPhone models up to 4S.
        // 
        // To use landscape mode, exchange the values of width and height, and 
        // set the "aspectRatio" element in the config XML to "landscape". (You'll also have to
        // update the background, startup- and "Default" graphics accordingly.)

        const int Constants::STAGE_WIDTH=320;
        const int Constants::STAGE_HEIGHT=480;

