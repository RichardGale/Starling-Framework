

#include "ScaleMode.h"
#include "starling/errors/AbstractClassError.h"

    /** A class that provides constant values for the 'RectangleUtil.fit' method. */

using namespace starling::errors;

namespace starling {
namespace utils {


        /** @private */
        ScaleMode::ScaleMode() { throw new AbstractClassError(); }

        /** Specifies that the rectangle is not scaled, but simply centered within the 
         *  specified area. */
        const std::string ScaleMode::NONE = "none";

        /** Specifies that the rectangle fills the specified area without distortion 
         *  but possibly with some cropping, while maintaining the original aspect ratio. */
        const std::string ScaleMode::NO_BORDER = "noBorder";

        /** Specifies that the entire rectangle will be scaled to fit into the specified 
         *  area, while maintaining the original aspect ratio. This might leave empty bars at
         *  either the top and bottom, or left and right. */
        const std::string ScaleMode::SHOW_ALL = "showAll";

        /** Indicates whether the given scale mode string is valid. */
        bool ScaleMode::isValid(std::string scaleMode)
        {
            return scaleMode == NONE || scaleMode == NO_BORDER || scaleMode == SHOW_ALL;
        }
}
}

