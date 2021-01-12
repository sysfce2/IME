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

#include "IME/core/loop/Engine.h"
#include "IME/core/time/Clock.h"
#include "IME/utility/ConfigFileParser.h"

template <class T>
void setDefaultValueIfNotSet(ime::PropertyContainer& settings,
     const std::string& setting, const std::string& type, T&& defaultValue)
{
    if (settings.hasProperty(setting) && settings.propertyHasValue(setting))
        return;
    else if (!settings.hasProperty(setting))
        settings.addProperty({setting, type, std::forward<T>(defaultValue)});
    else
        settings.setValueFor<T>(setting, std::forward<T>(defaultValue));
    IME_PRINT_WARNING(R"(Missing or valueless ")" + setting + R"(" entry in settings, using default value)");
}

namespace ime {
    Engine::Engine(const std::string &gameName, const PropertyContainer& settings) :
        Engine(gameName, "")
    {
        //@TODO - Fix engine throwing std::bad_cast during initialization
        //        when constructed with this constructor
        settings_ = std::move(settings);
    }

    Engine::Engine(const std::string &gameName, const std::string &settingsFile) :
        appName_{gameName},
        settingFile_{settingsFile},
        isSettingsLoadedFromFile_(!settingsFile.empty()),
        isInitialized_{false},
        isRunning_{false},
        elapsedTime_{0.0f},
        shouldPop_{false}
    {}

    void Engine::initialize() {
        if (isSettingsLoadedFromFile_)
            loadSettings();

        processSettings();
        initResourceManager();
        initRenderTarget();

        audioManager_ = std::make_unique<audio::AudioManager>();
        eventDispatcher_ = EventDispatcher::instance();

        windowCloseHandler_ = [this]{quit();};
        isInitialized_ = true;
    }

    void Engine::loadSettings() {
        settings_ = utility::ConfigFileParser().parse(settingFile_);
    }

    void Engine::processSettings() {
        setDefaultValueIfNotSet(settings_, "WINDOW_TITLE", "STRING", std::string("Untitled"));
        setDefaultValueIfNotSet(settings_, "WINDOW_WIDTH", "INT",  600);
        setDefaultValueIfNotSet(settings_, "WINDOW_HEIGHT", "INT", 600);
        setDefaultValueIfNotSet(settings_, "FPS_LIMIT", "INT", 60);
        setDefaultValueIfNotSet(settings_, "FULLSCREEN", "BOOL", false);
        setDefaultValueIfNotSet(settings_, "VSYNC", "BOOL",  false);
        setDefaultValueIfNotSet(settings_, "FONTS_DIR", "STRING", std::string("")); // Same directory as the executable
        setDefaultValueIfNotSet(settings_, "TEXTURES_DIR", "STRING", std::string(""));
        setDefaultValueIfNotSet(settings_, "IMAGES_DIR", "STRING", std::string(""));
        setDefaultValueIfNotSet(settings_, "SOUND_EFFECTS_DIR", "STRING", std::string(""));
        setDefaultValueIfNotSet(settings_, "MUSIC_DIR", "STRING", std::string(""));
    }

    void Engine::initRenderTarget() {
        auto desktopWidth = static_cast<int>(sf::VideoMode::getDesktopMode().width);
        auto desktopHeight = static_cast<int>(sf::VideoMode::getDesktopMode().height);
        auto title = settings_.getValueFor<std::string>("WINDOW_TITLE");
        auto width = settings_.getValueFor<int>("WINDOW_WIDTH");
        auto height = settings_.getValueFor<int>("WINDOW_HEIGHT");
        auto isFullscreen = settings_.getValueFor<bool>("FULLSCREEN");
        if (isFullscreen || (width >= desktopWidth && height >= desktopHeight)){
            window_.create(title, desktopWidth, desktopHeight, Window::Style::Fullscreen);
        } else {
            if (width > desktopWidth)
                width = desktopWidth;
            if (height > desktopHeight)
                height = desktopHeight;
            window_.create(title, width, height, Window::Style::Close);
        }

        window_.setFramerateLimit(settings_.getValueFor<int>("FPS_LIMIT"));
        window_.setVsyncEnabled(settings_.getValueFor<bool>("VSYNC"));
        window_.setIcon("icon.png");
    }

    void Engine::initResourceManager() {
        resourceManager_ = ResourceManager::getInstance();
        resourceManager_->setPathFor(ResourceType::Font, settings_.getValueFor<std::string>("FONTS_DIR"));
        resourceManager_->setPathFor(ResourceType::Texture, settings_.getValueFor<std::string>("TEXTURES_DIR"));
        resourceManager_->setPathFor(ResourceType::Image, settings_.getValueFor<std::string>("IMAGES_DIR"));
        resourceManager_->setPathFor(ResourceType::SoundBuffer, settings_.getValueFor<std::string>("SOUND_EFFECTS_DIR"));
        resourceManager_->setPathFor(ResourceType::Music, settings_.getValueFor<std::string>("MUSIC_DIR"));
    }

    void Engine::processEvents() {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed && windowCloseHandler_) {
                windowCloseHandler_();
                return;
            }
            statesManager_.getActiveState()->handleEvent(event);
            globalInputManager_.handleEvent(event);
            inputManager_.handleEvent(event);
        }
    }

    void Engine::run() {
        IME_ASSERT(isInitialized_, "Failed to start engine because its not initialized");
        IME_ASSERT(!statesManager_.isEmpty(), "Failed to start engine because it has no states");

        statesManager_.getActiveState()->onEnter();
        isRunning_ = true;
        auto const frameTime = 1.0f / settings_.getValueFor<int>("FPS_LIMIT");
        auto deltaTime = 0.0f, now = 0.0f, accumulator = 0.0f;
        auto clock = Clock();
        elapsedTime_ = 0.0f;
        auto prevTime = static_cast<float>(clock.getElapsedTimeInSeconds());
        while (window_.isOpen() && isRunning_ && !statesManager_.isEmpty()) {
            if (onFrameStart_)
                onFrameStart_();
            now = clock.getElapsedTimeInSeconds();
            deltaTime = now - prevTime;
            prevTime = now;
            accumulator += deltaTime;
            processEvents();
            while (accumulator >= frameTime) {
                statesManager_.getActiveState()->fixedUpdate(frameTime);
                accumulator -= frameTime;
            }
            update(deltaTime);
            clear();
            render();
            display();
            postFrameUpdate();
            elapsedTime_ += deltaTime;
            if (onFrameEnd_)
                onFrameEnd_();
        }
        shutdown();
    }

    void Engine::quit() {
        isRunning_ = false;
    }

    void Engine::update(float deltaTime) {
        statesManager_.getActiveState()->update(deltaTime);
    }

    void Engine::clear() {
        window_.clear();
    }

    void Engine::render() {
        statesManager_.getActiveState()->render(window_);
    }

    void Engine::display() {
        window_.display();
    }

    void Engine::pushState(std::shared_ptr<State> state, Callback<> callback) {
        if (!isRunning_) {
            prevStateInputManager_.push(input::InputManager());
            statesManager_.pushState(std::move(state));
        } else
            statesToPush_.push({std::move(state), callback});
    }

    void Engine::popState() {
        if (!isRunning_ && !statesManager_.isEmpty()) {
            statesManager_.popState();
            inputManager_ = prevStateInputManager_.top();
            prevStateInputManager_.pop();
        } else
            shouldPop_ = true;
    }

    void Engine::postFrameUpdate() {
        // Always pop first
        if (shouldPop_) {
            shouldPop_ = false;
            statesManager_.popState();
            if (!statesManager_.isEmpty()) {
                //Restore input manager
                inputManager_ = prevStateInputManager_.top();
                prevStateInputManager_.pop();
                if (!statesManager_.getActiveState()->isEntered())
                    statesManager_.getActiveState()->onEnter();
            }
        }

        while (!statesToPush_.empty()) {
            prevStateInputManager_.push(inputManager_);
            inputManager_ = input::InputManager(); //Clear prev state input handlers
            auto [state, callback] = statesToPush_.front();
            statesManager_.pushState(std::move(state));
            statesToPush_.pop();
            if (statesToPush_.empty()) {
                statesManager_.getActiveState()->onEnter();
                if (callback)
                    callback();
            }
        }
    }

    void Engine::shutdown() {
        while (!statesToPush_.empty())
            statesToPush_.pop();
        statesManager_.clear();
        window_.close();
        audioManager_->stopAllAudio();
        inputManager_ = input::InputManager();
        globalInputManager_ = input::InputManager();
        eventDispatcher_.reset();
        resourceManager_.reset();
        isRunning_ = false;
        isInitialized_ = false;
        elapsedTime_ = 0.0f;
        shouldPop_ = false;
    }

    bool Engine::isRunning() const {
        return isRunning_;
    }

    float Engine::getElapsedTime() const {
        return elapsedTime_;
    }

    ResourceManager &Engine::getResourceManager() {
        return *resourceManager_;
    }

    const PropertyContainer &Engine::getSettings() const {
        //@TODO - Fix this function throwing std::bad_cast when called outside
        //        the class
        return settings_;
    }

    const std::string &Engine::getGameName() const {
        return appName_;
    }

    PropertyContainer &Engine::getPersistentData() {
        return dataSaver_;
    }

    audio::AudioManager &Engine::getAudioManager() {
        return *audioManager_;
    }

    input::InputManager &Engine::getInputManager() {
        return inputManager_;
    }

    input::InputManager &Engine::getGlobalInputManager() {
        return globalInputManager_;
    }

    Window &Engine::getRenderTarget() {
        return window_;
    }

    void Engine::onWindowClose(Callback<> callback) {
        windowCloseHandler_ = std::move(callback);
    }

    void Engine::onFrameStart(Callback<> callback) {
        onFrameStart_ = std::move(callback);
    }

    void Engine::onFrameEnd(Callback<> callback) {
        onFrameEnd_ = std::move(callback);
    }
}
