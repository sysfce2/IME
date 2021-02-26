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

#ifndef IME_TABSCONTAINER_H
#define IME_TABSCONTAINER_H

#include "IME/Config.h"
#include "IME/ui/widgets/Widget.h"
#include "IME/ui/widgets/Panel.h"
#include "IME/ui/renderers/TabsRenderer.h"
#include <memory>

namespace ime {
    namespace ui {
        /**
         * @brief A widget which acts as a container for Tabs widget
         *
         * This widget can hold multiple panels which are displayed
         * one panel at a time. The widget uses Tabs to switch
         * between panels
         *
         * If you are looking for Tabs without a panel, check out the
         * Tabs class
         */
        class IME_API TabsContainer : public Widget {
        public:
            using Ptr = std::shared_ptr<TabsContainer>; //!< Shared widget pointer
            using ConstPtr = std::shared_ptr<const TabsContainer>; //!< const shared widget pointer

            /**
             * @brief Move constructor
             */
            TabsContainer(TabsContainer&&);

            /**
             * @brief Move assignment operator
             */
            TabsContainer& operator=(TabsContainer&&);

            /**
             * @brief Create a new TabsContainer widget
             * @param width Width of the panel relative to the size of its 
             *        parent
             * @param height Height of the panel relative to the size of its
             *        parent
             * @return The new TabsContainer
             * 
             * * The relative size is specified in percentages as shown:
             * 
             * @code
             * TabsContainer::create({"50%", "20%"});
             * @endcode
             * 
             * By default, the new panel is the same size as its parent
             */
            static TabsContainer::Ptr create(const std::string& width = "100%",
                const std::string& height = "100%");

            /**
             * @brief Create a copy of another container
             * @param other The container to copy
             * @param shareRenderer True if the new container should have the
             *          same renderer as the copied container
             * @return The new container widget
             *
             * When the containers share a renderer, changes in a render
             * property of one of the containers automatically reflect on
             * the other container, otherwise each container has its own renderer
             * and changes in render properties are isolated to the specific
             * container.
             *
             * @note when the containers don't share a render, the renderer of
             * the new container widget will initially have the properties of
             * the copied container such that the two look the same after this
             * function returns
             *
             * By default, the containers share a renderer
             *
             * @warning Once a renderer is shared, it cannot be unshared at
             * a later time
             */
            static TabsContainer::Ptr copy(TabsContainer::ConstPtr other,
                bool shareRenderer = true);

            /**
             * @brief Get the tabs renderer
             * @return The tabs renderer
             *
             * The renderer gives access to functions that determine how the
             * container is displayed. It allows you to manipulate things such
             * as the background colour, border colour etc...
             *
             * @see setRenderer
             */
            TabsRenderer::Ptr getRenderer();
            const TabsRenderer::Ptr getRenderer() const;

            /**
             * @brief Set the height of the tabs part of the widget
             * @param height New height of the tabs part of the widget
             */
            void setTabsHeight(float height);

            /**
             * @brief Add a panel
             * @param panel Panel to be added
             * @param text Text for the container
             * @param select True to immediately select the panel, otherwise
             *        false
             *
             * The panel is added after the panel that was added during
             * the last call to this function
             *
             * By default, the panel is selected after being added
             */
            void addPanel(Panel::Ptr panel, const std::string& text, bool select = true);

            /**
             * @brief Insert a panel between other panels
             * @param panel Panel to be added
             * @param text Text for the container
             * @param index Index where the panel should be inserted
             * @param select True to immediately select the panel, otherwise
             *               false
             * @return True if the panel was inserted or false if the given
             *         index is out of bounds
             *
             * By default, the panel is selected after successful insertion
             * 
             * @warning This function os experimental
             */
            bool insertPanel(Panel::Ptr panel, const std::string& text,
                std::size_t index, bool select = true);

            /**
             * @brief Remove a panel from the container
             * @param panel Panel to remove
             */
            void removePanel(Panel::Ptr panel);

            /**
             * @brief Select a panel
             * @param index Index of the panel to select
             */
            void select(std::size_t index);

            /**
             * @brief Get the number of panel in the container
             * @return The current number of panels in the container
             */
            std::size_t getPanelCount() const;

            /**
             * @brief Get the index of a given panel
             * @param panel Panel to get the index for
             * @return The index of the panel or -1 if the panel does not
             *         exist in the container
             */
            int getIndex(Panel::Ptr panel);

            /**
             * @brief Get the currently selected panel
             * @return The panel that is selected or a nullptr if no panel
             *         is selected
             */
            Panel::Ptr getSelected();

            /**
             * @brief Get the index of the currently selected panel
             * @return The index of the selected panel or -1 if not panel
             *         is selected
             */
            int getSelectedIndex() const;

            /**
             * @brief Get a panel with a given index
             * @param index The index of the panel to retrieve
             * @return Panel with the given index or a nullptr if the index
             *         is out of bounds
             */
            Panel::Ptr getPanel(int index);

            /**
             * @brief Get the text of a container
             * @param index Index of the container to get the index of
             * @return The text on the container or an empty string if the index
             *         is out of bounds
             *
             * The index of the first container is 0
             */
            std::string getTabText(std::size_t index) const;

            /**
             * @brief Set the text of a container
             * @param index The index of the container whose text is to be ste
             * @param text The new text of the container
             * @return True if the text was successfully changed or false
             *         if the index is out of bounds
             *
             * The index of the first container is 0
             */
            bool changeTabText(std::size_t index, const std::string& text);

            /**
             * @brief Get the type of the container
             * @return The type of the container
             */
            std::string getWidgetType() const override;

            /**
             * @brief Destructor
             */
            ~TabsContainer();

        private:
            /**
             * @brief Constructor
             * @param width Width of the container relative to the size of its
             *        parent
             * @param height Height of the container relative to the size of its
             *        parent
             *
             * The relative size is specified in percentages as shown:
             *
             * @code
             * TabsContainer({"50%", "20%"});
             * @endcode
             *
             * By default, the container is the same size as its parent
             */
            explicit TabsContainer(const std::string& width = "100%",
                const std::string& height = "100%");

        private:
            class Impl;
            std::unique_ptr<Impl> pimpl_;
        };
    }
}

#endif //IME_TABSCONTAINER_H
