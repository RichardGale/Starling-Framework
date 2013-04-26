#if !defined(__SAMPLES_SCAFFOLD_MOBILE_SRC_CONSTANTS_AS)
#define __SAMPLES_SCAFFOLD_MOBILE_SRC_CONSTANTS_AS
#if defined(__cplusplus)


namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

using namespace starling::errors;

class Constants
{
public:
    Constants();

    // We chose this stage size because it is used by many mobile devices;
    // it's e.g. the resolution of the iPhone (non-retina), which means that your game
    // will be displayed without any black bars on all iPhone models up to 4S.
    //
    // To use landscape mode, exchange the values of width and height, and
    // set the "aspectRatio" element in the config XML to "landscape". (You'll also have to
    // update the background, startup- and "Default" graphics accordingly.)

public:
    static const int STAGE_WIDTH;
public:
    static const int STAGE_HEIGHT;
};

#endif // __SAMPLES_SCAFFOLD_MOBILE_SRC_CONSTANTS_AS
#endif // __cplusplus

