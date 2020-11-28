////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020 Kwena Mashamaite (kmash.ime@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "IME/graphics/Colour.h"

namespace IME {
    const Colour Colour::Black{0, 0, 0, 255};
    const Colour Colour::White{255, 255, 255, 255};
    const Colour Colour::Red{255, 0, 0, 255};
    const Colour Colour::Green{0, 255, 0, 255};
    const Colour Colour::Blue{0, 0, 255, 255};
    const Colour Colour::Yellow{255, 255, 0, 255};
    const Colour Colour::Orange{255, 165, 0, 255};
    const Colour Colour::Cyan{0, 255, 255, 255};
    const Colour Colour::Purple{128, 0, 128, 255};
    const Colour Colour::Violet{238, 130, 238, 255};
    const Colour Colour::Magenta{255, 0, 255, 255};
    const Colour Colour::Pink{255, 192, 203, 255};
    const Colour Colour::Grey{128, 128, 128, 255};
    const Colour Colour::Turquoise{64, 224, 208, 255};
    const Colour Colour::Gold{255, 215, 0, 255};
    const Colour Colour::Transparent{0, 0, 0, 0};

    Colour::Colour() : red(0u), green (0u), blue(0u), opacity(255u)
    {}

    Colour::Colour(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha){
        red = r;
        green = g;
        blue = b;
        opacity = alpha;
    }

    bool Colour::operator==(const Colour &rhs) {
        return red == rhs.red
            && green == rhs.green
            && blue == rhs.blue
            && opacity == rhs.opacity;
    }

    bool Colour::operator!=(const Colour &rhs) {
        return !(*this == rhs);
    }
}
