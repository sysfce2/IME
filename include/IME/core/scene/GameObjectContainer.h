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

#ifndef IME_GAMEOBJECTCONTAINER_H
#define IME_GAMEOBJECTCONTAINER_H

#include "IME/Config.h"
#include "IME/common/ObjectContainer.h"
#include "IME/core/game_object/GameObject.h"
#include "IME/core/scene/RenderLayerContainer.h"

namespace ime {
    /**
     * @brief A container for GameObject instances
     */
    class IME_API GameObjectContainer : public ObjectContainer<GameObject> {
    public:
        /**
         * @brief Constructor
         * @param renderLayers The render layer container for
         */
        explicit GameObjectContainer(RenderLayerContainer& renderLayers);

        /**
         * @brief Add a game object to the container
         * @param gameObject The game object to be added
         * @param renderOrder The render order of the game object
         * @param renderLayer The RenderLayer the game object belongs to
         *
         * Note that the container keeps the pointer so there is no need to
         * keep the your pointer after adding the game objet
         */
        void add(GameObject::Ptr gameObject, unsigned int renderOrder = 0u,
             const std::string& renderLayer = "default");

    private:
        std::reference_wrapper<RenderLayerContainer> renderLayers_;
        using ObjectContainer::addObject;
    };
}

#endif //IME_GAMEOBJECTCONTAINER_H
