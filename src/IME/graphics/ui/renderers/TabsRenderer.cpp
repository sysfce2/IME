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

#include <IME/utility/Helpers.h>
#include "IME/graphics/ui/renderers/TabsRenderer.h"
#include "IME/core/managers/ResourceManager.h"
#include <IME/utility/Helpers.h>

namespace IME::UI {
    void TabsRenderer::setBorders(const Borders &borders) {
        tabsRenderer_->setBorders({borders.left, borders.top,
            borders.right, borders.bottom});
    }

    TabsRenderer::Borders TabsRenderer::getBorders() const {
        return {tabsRenderer_->getBorders().getLeft(),
                tabsRenderer_->getBorders().getTop(),
                tabsRenderer_->getBorders().getRight(),
                tabsRenderer_->getBorders().getBottom()};
    }

    void TabsRenderer::setBackgroundColour(Colour colour) {
        tabsRenderer_->setBackgroundColor(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getBackgroundColour() const {
        return Utility::convertFrom3rdPartyColour(tabsRenderer_->getBackgroundColor());
    }

    void TabsRenderer::setBackgroundHoverColour(Colour colour) {
        tabsRenderer_->setBackgroundColorHover(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getBackgroundHoverColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedBackgroundColorHover());
    }

    void TabsRenderer::setDisabledBackgroundColour(Colour colour) {
        tabsRenderer_->setBackgroundColorDisabled(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getDisabledBackgroundColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getBackgroundColorDisabled());
    }

    void TabsRenderer::setSelectedBackgroundColour(Colour backgroundColour) {
        tabsRenderer_->setSelectedBackgroundColor(Utility::convertToTGUIColour(backgroundColour));
    }

    Colour TabsRenderer::getSelectedBackgroundColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedBackgroundColor());
    }

    void TabsRenderer::setSelectedBackgroundHoverColour(Colour backgroundColour) {
        tabsRenderer_->setSelectedBackgroundColorHover(
            Utility::convertToTGUIColour(backgroundColour));
    }

    Colour TabsRenderer::getSelectedBackgroundHoverColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedBackgroundColorHover());
    }

    void TabsRenderer::setTextColour(Colour colour) {
        tabsRenderer_->setTextColor(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getTextColour() const {
        return Utility::convertFrom3rdPartyColour(tabsRenderer_->getTextColor());
    }

    void TabsRenderer::setTextHoverColour(Colour colour) {
        tabsRenderer_->setTextColorHover(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getTextHoverColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getTextColorHover());
    }

    void TabsRenderer::setDisabledTextColour(Colour colour) {
        tabsRenderer_->setTextColorDisabled(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getDisabledTextColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getTextColorDisabled());
    }

    void TabsRenderer::setSelectedTextColour(Colour textColour) {
        tabsRenderer_->setSelectedTextColor(Utility::convertToTGUIColour(textColour));
    }

    Colour TabsRenderer::getSelectedTextColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedTextColor());
    }

    void TabsRenderer::setSelectedTextHoverColour(Colour textColour) {
        tabsRenderer_->setSelectedTextColorHover(
            Utility::convertToTGUIColour(textColour));
    }

    Colour TabsRenderer::getSelectedTextColourHover() const {
        return Utility::convertFrom3rdPartyColour(
                tabsRenderer_->getSelectedTextColorHover());
    }

    void TabsRenderer::setBorderColour(Colour colour) {
        tabsRenderer_->setBorderColor(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getBorderColour() const {
        return Utility::convertFrom3rdPartyColour(tabsRenderer_->getBorderColor());
    }

    void TabsRenderer::setBorderHoverColour(Colour colour) {
        tabsRenderer_->setBorderColorHover(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getBorderHoverColour() const {
        return Utility::convertFrom3rdPartyColour(tabsRenderer_->getBorderColorHover());
    }

    void TabsRenderer::setSelectedBorderColour(Colour colour) {
        tabsRenderer_->setSelectedBorderColor(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getSelectedBorderColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedBorderColor());
    }

    void TabsRenderer::setSelectedBorderHoverColour(Colour colour) {
        tabsRenderer_->setSelectedBorderColorHover(Utility::convertToTGUIColour(colour));
    }

    Colour TabsRenderer::getSelectedBorderHoverColour() const {
        return Utility::convertFrom3rdPartyColour(
            tabsRenderer_->getSelectedBorderColorHover());
    }

    void TabsRenderer::setTabTexture(const std::string &filename) {
        tabsRenderer_->setTextureTab(
            IME::ResourceManager::getInstance()->getTexture(filename));
    }

    void TabsRenderer::setTabHoverTexture(const std::string &filename) {
        tabsRenderer_->setTextureTabHover(
            IME::ResourceManager::getInstance()->getTexture(filename));
    }

    void TabsRenderer::setSelectedTabTexture(const std::string &filename) {
        tabsRenderer_->setTextureSelectedTab(
            IME::ResourceManager::getInstance()->getTexture(filename));
    }

    void TabsRenderer::setSelectedTabHoverTexture(const std::string &filename) {
        tabsRenderer_->setTextureSelectedTabHover(
            IME::ResourceManager::getInstance()->getTexture(filename));
    }

    void TabsRenderer::setDisabledTabTexture(const std::string &filename) {
        tabsRenderer_->setTextureDisabledTab(
            IME::ResourceManager::getInstance()->getTexture(filename));
    }

    void TabsRenderer::setDistanceToSide(float distanceToSide) {
        tabsRenderer_->setDistanceToSide(distanceToSide);
    }

    float TabsRenderer::getDistanceToSide() const {
        return tabsRenderer_->getDistanceToSide();
    }

    void TabsRenderer::setOpacity(float opacity) {
        tabsRenderer_->setOpacity(opacity);
    }

    float TabsRenderer::getOpacity() const {
        return tabsRenderer_->getOpacity();
    }

    void TabsRenderer::setOpacityDisabled(float opacity) {
        tabsRenderer_->setOpacityDisabled(opacity);
    }

    float TabsRenderer::getOpacityDisabled() const {
        return tabsRenderer_->getOpacityDisabled();
    }

    void TabsRenderer::setFont(const std::string &filename) {
        tabsRenderer_->setFont(Utility::get_TGUI_Font(filename));
    }

    void TabsRenderer::ignoreTransparentTexture(bool ignoreTransparentParts) {
        tabsRenderer_->setTransparentTexture(ignoreTransparentParts);
    }

    bool TabsRenderer::isTransparentTextureIgnored() const {
        return tabsRenderer_->getTransparentTexture();
    }

    void TabsRenderer::setInternalPtr(tgui::WidgetRenderer *renderer) {
        tabsRenderer_ = dynamic_cast<tgui::TabsRenderer*>(renderer);
    }

    tgui::WidgetRenderer *TabsRenderer::getInternalPtr() {
        return tabsRenderer_;
    }

    TabsRenderer::~TabsRenderer() {
        // We don't delete the pointee because its lifetime is managed by the
        // third party library
        tabsRenderer_ = nullptr;
    }
}