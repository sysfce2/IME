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

#ifndef IME_CHECKBOX_H
#define IME_CHECKBOX_H

#include "IME/Config.h"
#include "IME/ui/widgets/ClickableWidget.h"
#include "IME/ui/renderers/CheckBoxRenderer.h"
#include <memory>

namespace ime {
    namespace ui {
        /**
         * @brief Checkbox widget
         */
        class IME_API CheckBox : public ClickableWidget {
        public:
            using Ptr = std::shared_ptr<CheckBox>; //!< Shared widget pointer
            using ConstPtr = std::shared_ptr<const CheckBox>; //!< const shared widget pointer

            /**
             * @brief Constructor
             */
            CheckBox(CheckBox&&);

            /**
             * @brief Move assignment operator
             */
            CheckBox& operator=(CheckBox&&);

            /**
             * @brief Create a new checkbox widget
             * @param text Text to display next to the checkbox
             * @return The new checkbox
             */
            static CheckBox::Ptr create(const std::string& text = "");

            /**
             * @brief Create a copy of another checkbox
             * @param other The checkbox to copy
             * @param shareRenderer True if the new checkbox should have the
             *          same renderer as the copied checkbox
             * @return The new checkbox widget
             *
             * When the checkboxes share a renderer, changes in a render
             * property of one of the checkboxes automatically reflect on
             * the other checkbox, otherwise each checkbox has its own renderer
             * and changes in render properties are isolated to the specific
             * checkbox.
             *
             * @note when the checkboxes don't share a render, the renderer of
             * the new checkbox widget will initially have the properties of
             * the copied checkbox such that the two look the same after this
             * function returns
             *
             * By default, the checkboxes share a renderer
             *
             * @warning Once a renderer is shared, it cannot be unshared at
             * a later time
             */
            static CheckBox::Ptr copy(CheckBox::ConstPtr other, bool shareRenderer = true);

            /**
             * @brief Get the checkboxes renderer
             * @return The checkboxes renderer
             *
             * The renderer gives access to functions that determine how the
             * checkbox is displayed. It allows you to manipulate things such
             * as the background colour, text colour, border colour etc...
             *
             * @see setRenderer
             */
            CheckBoxRenderer::Ptr getRenderer();
            const CheckBoxRenderer::Ptr getRenderer() const;

            /**
             * @brief Allow or disallow the checkbox to be checked by clicking
             *        on the text next to it
             * @param acceptTextClick True to allow, or false to disallow
             */
            void setTextClickable(bool acceptTextClick);

            /**
             * @brief Check if the the radio checkbox can be checked by clicking
             *         on the text next to it
             * @return True if the checkbox is clickable by clicking on the
             *         text next to it, otherwise false
             */
            bool isTextClickable() const;

            /**
             * @brief Check or uncheck the checkbox
             * @param isChecked True to check the checkbox, otherwise false
             */
            void setChecked(bool isChecked);

            /**
             * @brief Check whether the checkbox is checked or not
             * @return True if it is checked, otherwise false
             */
            bool isChecked() const;

            /**
             * @brief Set the text to display next to the checkbox
             * @param text Text to display
             *
             * This function will overwrite any text that was previously
             * set
             */
            void setText(const std::string &text);

            /**
             * @brief Get the text displayed next to the checkbox
             * @return The text displayed next to the checkbox
             */
            std::string getText() const;

            /**
             * @brief Get the type of the check box
             * @return The type of the check box
             */
            std::string getWidgetType() const override;

            /**
             * @brief Destructor
             */
            ~CheckBox();

        private:
            /**
             * @brief Create the checkbox
             * @param text Text to display next to the checkbox
             */
            explicit CheckBox(const std::string &text = "");

        private:
            class Impl;
            std::unique_ptr<Impl> pimpl_;
        };
    }
}

#endif // IME_CHECKBOX_H
