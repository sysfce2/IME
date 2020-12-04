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

#include "IME/graphics/ui/renderers/ProgressBarRenderer.h"
#include "IME/core/managers/ResourceManager.h"
#include <TGUI/Backends/SFML/BackendFontSFML.hpp>
#include "IME/utility/Helpers.h"

namespace IME::UI {
    void ProgressBarRenderer::setBorders(const Borders &borders) {
        progressBarRenderer_->setBorders({borders.left, borders.top,
            borders.right, borders.bottom});
    }

    Borders ProgressBarRenderer::getBorders() const {
        return {progressBarRenderer_->getBorders().getLeft(),
                progressBarRenderer_->getBorders().getTop(),
                progressBarRenderer_->getBorders().getRight(),
                progressBarRenderer_->getBorders().getBottom()};
    }

    void ProgressBarRenderer::setTextColour(Colour color) {
        progressBarRenderer_->setTextColor(Utility::convertToTGUIColour(color));
    }

    Colour ProgressBarRenderer::getTextColour() const {
        return Utility::convertFrom3rdPartyColour(progressBarRenderer_->getTextColor());
    }

    void ProgressBarRenderer::setTextColourFilled(Colour color) {
        progressBarRenderer_->setTextColorFilled(Utility::convertToTGUIColour(color));
    }

    Colour ProgressBarRenderer::getTextColourFilled() const {
        return Utility::convertFrom3rdPartyColour(progressBarRenderer_->getTextColorFilled());
    }

    void ProgressBarRenderer::setBackgroundColour(Colour color) {
        progressBarRenderer_->setBackgroundColor(Utility::convertToTGUIColour(color));
    }

    Colour ProgressBarRenderer::getBackgroundColour() const {
        return Utility::convertFrom3rdPartyColour(progressBarRenderer_->getBackgroundColor());
    }

    void ProgressBarRenderer::setFillColour(Colour color) {
        progressBarRenderer_->setFillColor(Utility::convertToTGUIColour(color));
    }

    Colour ProgressBarRenderer::getFillColour() const {
        return Utility::convertFrom3rdPartyColour(progressBarRenderer_->getFillColor());
    }

    void ProgressBarRenderer::setBorderColour(Colour color) {
        progressBarRenderer_->setBorderColor(Utility::convertToTGUIColour(color));
    }

    Colour ProgressBarRenderer::getBorderColour() const {
        return Utility::convertFrom3rdPartyColour(progressBarRenderer_->getBorderColor());
    }

    void ProgressBarRenderer::setBackgroundTexture(const std::string &texture) {
        progressBarRenderer_->setTextureBackground(
            IME::ResourceManager::getInstance()->getTexture(texture));
    }

    void ProgressBarRenderer::setFillTexture(const std::string &texture) {
        progressBarRenderer_->setTextureFill(
            IME::ResourceManager::getInstance()->getTexture(texture));
    }

    void ProgressBarRenderer::setTextStyle(TextStyle style) {
        progressBarRenderer_->setTextStyle(static_cast<tgui::TextStyle>(style));
    }

    TextStyle ProgressBarRenderer::getTextStyle() const {
        return static_cast<TextStyle>(static_cast<unsigned int>(
                progressBarRenderer_->getTextStyle()));
    }

    void ProgressBarRenderer::setOpacity(float opacity) {
        progressBarRenderer_->setOpacity(opacity);
    }

    float ProgressBarRenderer::getOpacity() const {
        return progressBarRenderer_->getOpacity();
    }

    void ProgressBarRenderer::setOpacityDisabled(float opacity) {
        progressBarRenderer_->setOpacityDisabled(opacity);
    }

    float ProgressBarRenderer::getOpacityDisabled() const {
        return progressBarRenderer_->getOpacity();
    }

    void ProgressBarRenderer::setFont(const std::string &filename) {
        progressBarRenderer_->setFont(Utility::get_TGUI_Font(filename));
    }

    void ProgressBarRenderer::ignoreTransparentTexture(bool ignoreTransparentParts) {
        progressBarRenderer_->setTransparentTexture(ignoreTransparentParts);
    }

    bool ProgressBarRenderer::isTransparentTextureIgnored() const {
        return progressBarRenderer_->getTransparentTexture();
    }

    void ProgressBarRenderer::unsubscribe(int ) {

    }

    void ProgressBarRenderer::setInternalPtr(tgui::WidgetRenderer *renderer) {
        progressBarRenderer_ = dynamic_cast<tgui::ProgressBarRenderer*>(renderer);
    }

    tgui::WidgetRenderer *ProgressBarRenderer::getInternalPtr() {
        return progressBarRenderer_;
    }

    int ProgressBarRenderer::onPropertyChange(const std::string &,
        Callback<const std::string &>) {
        return 0;
    }

    ProgressBarRenderer::~ProgressBarRenderer() {
        // We don't delete the pointee because its lifetime is managed by the
        // third party library
        progressBarRenderer_ = nullptr;
    }
}
