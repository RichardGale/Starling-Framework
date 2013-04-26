#if !defined(__STARLING_SRC_STARLING_UTILS_SCALEMODE_AS)
#define __STARLING_SRC_STARLING_UTILS_SCALEMODE_AS
#if defined(__cplusplus)


namespace starling
{
    namespace errors
    {
        class AbstractClassError;
    }
}

/** A class that provides constant values for the 'RectangleUtil.fit' method. */
using namespace starling::errors;

namespace starling
{
    namespace utils
    {
        class ScaleMode
        {
            /** @private */
        public:
            ScaleMode();

            /** Specifies that the rectangle is not scaled, but simply centered within the
             *  specified area. */
        public:
            static const std::string NONE;

            /** Specifies that the rectangle fills the specified area without distortion
             *  but possibly with some cropping, while maintaining the original aspect ratio. */
        public:
            static const std::string NO_BORDER;

            /** Specifies that the entire rectangle will be scaled to fit into the specified
             *  area, while maintaining the original aspect ratio. This might leave empty bars at
             *  either the top and bottom, or left and right. */
        public:
            static const std::string SHOW_ALL;

            /** Indicates whether the given scale mode string is valid. */
        public:
            static bool     isValid(std::string scaleMode);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_SCALEMODE_AS
#endif // __cplusplus

