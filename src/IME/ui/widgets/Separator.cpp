////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "IME/ui/widgets/Separator.h"
#include "WidgetImpl.h"
#include <TGUI/Widgets/SeparatorLine.hpp>

namespace ime::ui {
    Separator::Separator() :
        Widget(std::make_unique<priv::WidgetImpl<tgui::SeparatorLine>>(tgui::SeparatorLine::create()))
    {
        setRenderer(std::make_shared<SeparatorRenderer>());
    }

    Separator::Separator(Separator &&) = default;

    Separator &Separator::operator=(Separator &&) = default;

    Separator::Ptr Separator::create() {
        return Ptr(new Separator());
    }

    Separator::Ptr Separator::copy(Separator::ConstPtr other,
        bool shareRenderer) 
    {
        auto widget = create();

        return widget;
    }

    std::shared_ptr<SeparatorRenderer> Separator::getRenderer() {
        return std::static_pointer_cast<SeparatorRenderer>(Widget::getRenderer());
    }

    const std::shared_ptr<SeparatorRenderer> Separator::getRenderer() const {
        return std::static_pointer_cast<SeparatorRenderer>(Widget::getRenderer());
    }

    std::string Separator::getWidgetType() const {
        return "Separator";
    }

    Separator::~Separator() = default;
}
