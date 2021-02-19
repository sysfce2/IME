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

template <typename T>
inline void Container<T>::add(Item item) {
    items_.push_back(std::move(item));
}

template <typename T>
inline bool Container<T>::remove(Item item) {
    if (auto found = std::find(items_.begin(), items_.end(), item); found != items_.end()) {
        items_.erase(found);
        return true;
    }

    return false;
}

template <typename T>
inline void Container<T>::removeAll() {
    items_.clear();
}

template <typename T>
inline std::size_t Container<T>::getCount() const {
    return items_.size();
}

template <typename T>
inline void Container<T>::render(Window& window, Callback<Item> preRenderCallback) {
    forEach([&window, &preRenderCallback](Item item) {
        if (preRenderCallback)
            preRenderCallback(item);
        item->draw(window);;
    });
}

template <>
inline void Container<GameObject>::render(Window& window, Callback<GameObject::sharedPtr> preRenderCallback) {
    forEach([&window, &preRenderCallback](GameObject::sharedPtr gameObject) {
        if (preRenderCallback)
            preRenderCallback(gameObject);
        gameObject->getSprite().draw(window);;
    });
}

template <typename T>
inline void Container<T>::forEach(Callback<Item> callback) {
    std::for_each(items_.begin(), items_.end(), callback);
}

template <typename T>
inline typename Container<T>::constIterator Container<T>::cbegin() const {
    return items_.cbegin();
}

template <typename T>
inline typename Container<T>::constIterator Container<T>::cend() const {
    return items_.cend();
}