

#include "TextScene.h"
#include "starling/text/BitmapFont.h"
#include "starling/text/TextField.h"
#include "starling/utils/Color.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

using namespace starling::text;
using namespace starling::utils;

namespace scenes {


        TextScene::TextScene()
        {
            init();
        }

        void TextScene::init()
        {
            // TrueType fonts

            int offset = 10;
            std::string ttFont = "Ubuntu";
            int ttFontSize = 19;

            TextField* colorTF = new TextField(300, 80,
                "TextFields can have a border and a color. They can be aligned in different ways, ...",
                ttFont, ttFontSize);
            colorTF->x ( colorTF->y ( offset));
            colorTF->border ( true);
            colorTF->color ( 0x333399);
            addChild(colorTF);

            TextField* leftTF = new TextField(145, 80,
                "... e.g.\\ntop-left ...", ttFont, ttFontSize);
            leftTF->x ( offset);
            leftTF->y ( colorTF->y() + colorTF->height() + offset);
            leftTF->hAlign ( HAlign::LEFT);
            leftTF->vAlign ( VAlign::TOP);
            leftTF->border ( true);
            leftTF->color ( 0x993333);
            addChild(leftTF);

            TextField* rightTF = new TextField(145, 80,
                "... or\\nbottom right ...", ttFont, ttFontSize);
            rightTF->x ( 2*offset + leftTF->width());
            rightTF->y ( leftTF->y());
            rightTF->hAlign ( HAlign::RIGHT);
            rightTF->vAlign ( VAlign::BOTTOM);
            rightTF->color ( 0x228822);
            rightTF->border ( true);
            addChild(rightTF);

            TextField* fontTF = new TextField(300, 80,
                "... or centered. Embedded fonts are detected automatically.",
                ttFont, ttFontSize, 0x0, true);
            fontTF->x ( offset);
            fontTF->y ( leftTF->y() + leftTF->height() + offset);
            fontTF->border ( true);
            addChild(fontTF);

            // Bitmap fonts!

            // First, you will need to create a bitmap font texture.
            //
            // E.g. with this tool: www.angelcode.com/products/bmfont/ or one that uses the same
            // data format. Export the font data as an XML file, and the texture as a png with white
            // characters on a transparent background (32 bit).
            //
            // Then, you just have to register the font at the TextField class.    
            // Look at the file "Assets.as" to see how this is done.
            // After that, you can use them just like a conventional TrueType font.

            TextField* bmpFontTF = new TextField(300, 150,
                "It is very easy to use Bitmap fonts,\\nas well!", "Desyrel");

            bmpFontTF->fontSize ( BitmapFont::NATIVE_SIZE);// the native bitmap font size, no scaling
            bmpFontTF->color ( Color::WHITE);// use white to use the texture as it is (no tinting)
            bmpFontTF->x ( offset);
            bmpFontTF->y ( fontTF->y() + fontTF->height() + offset);
            addChild(bmpFontTF);

            // A tip: you can add the font-texture to your standard texture atlas and reference 
            // it from there. That way, you save texture space and avoid another texture-switch.
        }
}

