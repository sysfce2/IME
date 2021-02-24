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

#include "IME/core/animation/Animator.h"
#include "IME/core/resources/ResourceManager.h"
#include "IME/graphics/Sprite.h"
#include <algorithm>

namespace ime {
    Animator::Animator(Sprite& target) :
        currentFrameIndex_{0},
        timescale_{1.0f},
        isPlaying_{false},
        isPaused_{false},
        hasStarted_{false},
        target_{std::make_unique<std::reference_wrapper<Sprite>>(target)},
        cycleDirection_{Direction::Unknown},
        completedFirstAlternateCycle_{false}
    {}

    Animator::Animator(const Animator& other) :
        currentFrameIndex_{other.currentFrameIndex_},
        totalTime_{other.totalTime_},
        timescale_{other.timescale_},
        isPlaying_{other.isPlaying_},
        isPaused_{other.isPaused_},
        hasStarted_{other.hasStarted_},
        eventEmitter_{other.eventEmitter_},
        currentAnimation_{other.currentAnimation_},
        chains_{other.chains_},
        animations_{other.animations_}
    {}

    Animator &Animator::operator=(Animator other) {
        swap(other);
        return *this;
    }

    void Animator::swap(Animator &other) {
        std::swap(currentFrameIndex_, other.currentFrameIndex_);
        std::swap(totalTime_, other.totalTime_);
        std::swap(timescale_, other.timescale_);
        std::swap(isPlaying_, other.isPlaying_);
        std::swap(isPaused_, other.isPaused_);
        std::swap(hasStarted_, other.hasStarted_);
        std::swap(eventEmitter_, other.eventEmitter_);
        std::swap(currentAnimation_, other.currentAnimation_);
        std::swap(chains_, other.chains_);
        std::swap(animations_, other.animations_);
    }

    Animation::sharedPtr Animator::createAnimation(const std::string &name,
        const SpriteSheet& spriteSheet, Time duration)
    {
        auto animation = Animation::create(name, spriteSheet, duration);
        if (addAnimation(animation))
            return animation;
        else
            return nullptr;
    }

    void Animator::setTarget(Sprite &target) {
        target_.reset(new std::reference_wrapper<Sprite>(target));
    }

    void Animator::setTimescale(float timescale) {
        if (timescale < 0.0f)
            timescale_ = 1.0f;
        else
            timescale_ = timescale;
    }

    float Animator::getTimescale() const {
        return timescale_;
    }

    bool Animator::addAnimation(Animation::sharedPtr animation) {
        IME_ASSERT(animation, "Cannot add nullptr to animator");
        return animations_.insert({animation->getName(), std::move(animation)}).second;
    }

    void Animator::addAnimation(Animator::Animations animations) {
        std::for_each(animations.begin(), animations.end(),
            [this](const auto& animation) {
                addAnimation(animation);
            }
        );
    }

    Animation::sharedPtr Animator::getAnimation(const std::string &name) const {
        if (hasAnimation(name))
            return animations_.at(name);
        return nullptr;
    }

    Animation::sharedPtr Animator::getCurrentAnimation() const {
        return currentAnimation_;
    }

    bool Animator::removeAnimation(const std::string &name) {
        if (hasAnimation(name)) {
            animations_.erase(name);
            return true;
        }
        return false;
    }

    bool Animator::removeAnimation(Animation::sharedPtr animation) {
        if (animation)
            removeAnimation(animation->getName());
        return false;
    }

    void Animator::removeAll() {
        animations_.clear();
    }

    bool Animator::hasAnimation(const Animation::sharedPtr &animation) const {
        if (animation)
            return hasAnimation(animation->getName());
        return false;
    }

    bool Animator::hasAnimation(const std::string &name) const {
        return animations_.find(name) != animations_.end();
    }

    void Animator::chainAnimation(Animation::sharedPtr animation) {
        if (!animation || (currentAnimation_ && currentAnimation_ == animation))
            return;

        addAnimation(animation);

        if (!currentAnimation_) {
            currentAnimation_ = animation;
            play();
        } else
            chains_.push(std::move(animation));
    }

    void Animator::chainAnimation(const std::string &name) {
        if (hasAnimation(name))
            chainAnimation(animations_.at(name));
    }

    bool Animator::removeChain(const std::string &name) {
        if (chains_.empty())
            return false;

        auto sizeBeforeOp = chains_.size();
        auto newChains = std::queue<Animation::sharedPtr>{};
        while (!chains_.empty()) {
            if (chains_.front()->getName() != name)
                newChains.push(std::move(chains_.front()));
            chains_.pop();
        }
        chains_.swap(newChains);
        return chains_.size() < sizeBeforeOp;
    }

    void Animator::clearAllChains() {
        for (auto i = 0u; i < chains_.size(); ++i)
            chains_.pop();
    }

    bool Animator::switchAnimation(const std::string &animation, bool ignoreIfPlaying) {
        if (!hasAnimation(animation))
            return false;

        if (!currentAnimation_) {
            currentAnimation_ = animations_.at(animation);
            setCycleDirection();
        } else if ((isPlaying_ || isPaused_) && !ignoreIfPlaying) {
            stop();
            currentAnimation_ = animations_.at(animation);
            setCycleDirection();
            fireEvent(Event::AnimationSwitch, currentAnimation_);
        } else
            return false;

        return true;
    }

    void Animator::startAnimation(const std::string &name, bool unchain) {
        if (!hasAnimation(name))
            return;

        if (isPlaying_ || isPaused_)
            stop();

        if (unchain)
            clearAllChains();

        currentAnimation_ = animations_.at(name);
        setCycleDirection();

        play();
    }

    void Animator::play() {
        if (currentAnimation_ && !isPlaying_ && !isPaused_) {
            isPlaying_ = true;
            fireEvent(Event::AnimationPlay, currentAnimation_);
        }
    }

    void Animator::pause() {
        if (isPlaying_) {
            isPlaying_ = false;
            isPaused_ = true;
            fireEvent(Event::AnimationPause, currentAnimation_);
        }
    }

    void Animator::resume() {
        if (isPaused_) {
            isPlaying_ = true;
            isPaused_ = false;
            fireEvent(Event::AnimationResume, currentAnimation_);
        }
    }

    void Animator::stop() {
        if (isPlaying_ || isPaused_) {
            isPlaying_ = hasStarted_ = isPaused_ = false;
            totalTime_ = Time::Zero;
            currentFrameIndex_ = 0;
            resetCurrentFrame();
            fireEvent(Event::AnimationStop, currentAnimation_);
        }
    }

    void Animator::restart() {
        stop();
        play();
    }

    void Animator::complete() {
        if (currentAnimation_) {
            if (currentAnimation_->getDirection() == Animation::Direction::Forward
                || currentAnimation_->getDirection() == Animation::Direction::Alternate_Reverse)
            {
                currentFrameIndex_ = currentAnimation_->getFrameCount() - 1;
            } else if (currentAnimation_->getDirection() == Animation::Direction::Reverse
                || currentAnimation_->getDirection() == Animation::Direction::Alternate_Forward)
            {
                currentFrameIndex_ = 0;
            }

            setCurrentFrame(*currentAnimation_->getFrameAt(currentFrameIndex_));
            onComplete();
        }
    }

    bool Animator::isAnimationPlaying() const {
        return isPlaying_;
    }

    bool Animator::isAnimationPaused() const {
        return isPaused_;
    }

    bool Animator::hasAnimationStarted() const {
        return hasStarted_;
    }

    void Animator::update(Time deltaTime) {
        IME_ASSERT(target_, "Cannot start Animator without a target to animate");
        if (!currentAnimation_ || !isPlaying_ || isPaused_)
            return;

        totalTime_ += deltaTime * timescale_;

        // Handle delayed start
        if (!hasStarted_) {
            if (totalTime_ >= currentAnimation_->getStartDelay()) {
                totalTime_ -= currentAnimation_->getStartDelay();
                onStart();
            }
        } else if (totalTime_ >= currentAnimation_->getFrameTime()) {
            totalTime_ = Time::Zero;
            if (currentAnimation_->getDirection() == Animation::Direction::Forward
                || currentAnimation_->getDirection() == Animation::Direction::Reverse)
            {
                cycle(false);
            } else
                cycle(true);
        }
    }

    int Animator::on(Animator::Event event, Callback<Animation::sharedPtr> callback) {
        return eventEmitter_.on(std::to_string(static_cast<int>(event)), std::move(callback));
    }

    int Animator::on(Animator::Event event, Callback<> callback) {
        return eventEmitter_.on(std::to_string(static_cast<int>(event)), std::move(callback));
    }

    bool Animator::unsubscribe(Animator::Event event, int id) {
        return eventEmitter_.removeEventListener(std::to_string(static_cast<int>(event)), id);
    }

    int Animator::on(Animator::Event event, const std::string &name,
        Callback<Animation::sharedPtr> callback)
    {
        return eventEmitter_.on(std::to_string(static_cast<int>(event)) + name, std::move(callback));
    }

    int Animator::on(Animator::Event event, const std::string &name,
        Callback<> callback)
    {
        return eventEmitter_.on(std::to_string(static_cast<int>(event)) + name, std::move(callback));
    }

    bool Animator::unsubscribe(Animator::Event event, const std::string &name, int id) {
        return eventEmitter_.removeEventListener(std::to_string(static_cast<int>(event)) + name, id);
    }

    void Animator::fireEvent(Animator::Event event, Animation::sharedPtr animation) {
        /**
         * Event handlers are separated into specific and general handlers
         *
         * Specific handlers are only invoked when the event is generated
         * by a specific animation object whilst general handlers are always
         * invoked when an animation is fired regardless of the animation
         * object that triggered the event. This separation of general and
         * specific handlers alleviates the issue of having one monolithic
         * handler in the client code with if-else-if branches that checks
         * the animation name before performing an action
         *
         * We first notify specific handlers (which may not be invoked) about
         * the event, then notify general handlers. Note that the callback
         * argument is optional so we fire the event twice, first without the
         * argument and secondly with the argument.
         */

        // Specific handlers
        eventEmitter_.emit(std::to_string(static_cast<int>(event)) + animation->getName());
        eventEmitter_.emit(std::to_string(static_cast<int>(event)) + animation->getName(), animation);

        //General handlers
        eventEmitter_.emit(std::to_string(static_cast<int>(event)));
        eventEmitter_.emit(std::to_string(static_cast<int>(event)), animation);
    }

    void Animator::setCycleDirection() {
        if (currentAnimation_->getDirection() == Animation::Direction::Forward
            || currentAnimation_->getDirection() == Animation::Direction::Alternate_Forward)
        {
            cycleDirection_ = Direction::Forward;
        } else
            cycleDirection_ = Direction::Backward;
    }

    void Animator::cycle(bool isAlternating) {
        static auto completeFirstAlternateCycle = [this] {
            completedFirstAlternateCycle_ = true;
            if (cycleDirection_ == Direction::Forward) {
                currentFrameIndex_--;
                cycleDirection_ = Direction::Backward;
            } else {
                currentFrameIndex_++;
                cycleDirection_ = Direction::Forward;
            }
        };

        if ((cycleDirection_ == Direction::Backward && currentFrameIndex_ == 0)
            || (cycleDirection_ == Direction::Forward && currentFrameIndex_ == currentAnimation_->getFrameCount() - 1))
        {
            if (!currentAnimation_->isRepeating()) {
                if (isAlternating) {
                    if (completedFirstAlternateCycle_) {
                        completedFirstAlternateCycle_ = false;
                        onComplete();
                        return;
                    } else {
                        completeFirstAlternateCycle();
                    }
                } else {
                    onComplete();
                    return;
                }
            } else {
                // Update repeat counter
                if (currentAnimation_->getRepeatCount() != -1) { // -1 = repeat forever
                    if (isAlternating) {
                        if (completedFirstAlternateCycle_) {
                            completedFirstAlternateCycle_ = false;
                            currentAnimation_->setRepeatCount(currentAnimation_->getRepeatCount() - 1);
                            fireEvent(Event::AnimationRepeat, currentAnimation_);
                        }
                    } else {
                        currentAnimation_->setRepeatCount(currentAnimation_->getRepeatCount() - 1);
                        fireEvent(Event::AnimationRepeat, currentAnimation_);
                    }
                }

                // Repeat animation
                if (!isAlternating) {
                    if (currentFrameIndex_ == 0)
                        currentFrameIndex_ = currentAnimation_->getFrameCount() - 1;
                    else
                        currentFrameIndex_ = 0;
                } else {
                    completeFirstAlternateCycle();
                }
            }
        } else {
            if (cycleDirection_ == Direction::Forward)
                currentFrameIndex_++;
            else
                currentFrameIndex_--;
        }

        setCurrentFrame(*currentAnimation_->getFrameAt(currentFrameIndex_));
    }

    void Animator::onStart() {
        hasStarted_ = true;
        if (currentAnimation_->isTargetShownOnStart())
            (*target_).get().setVisible(true);

        resetCurrentFrame();
        fireEvent(Event::AnimationStart, currentAnimation_);
    }

    void Animator::onComplete() {
        setCurrentFrame(*currentAnimation_->getFrameAt(currentAnimation_->getCompletionFrameIndex()));

        if (currentAnimation_->isTargetHiddenOnCompletion())
            (*target_).get().setVisible(false);

        isPlaying_ = isPaused_ = hasStarted_ = false;
        totalTime_ = Time::Zero;
        fireEvent(Event::AnimationComplete, currentAnimation_);

        if (!chains_.empty()) {
            currentAnimation_ = chains_.front();
            chains_.pop();
            play();
        }
    }

    void Animator::advanceFrame() {

    }

    void Animator::reverseFrame() {

    }

    void Animator::setCurrentFrame(Animation::Frame frame) {
        (*target_).get().setTexture(currentAnimation_->getSpriteSheet().getTexture());
        (*target_).get().setTextureRect(frame.left, frame.top, frame.width, frame.height);
    }

    void Animator::resetCurrentFrame() {
        if (cycleDirection_ == Direction::Forward)
            currentFrameIndex_ = 0;
        else if (cycleDirection_ == Direction::Backward)
            currentFrameIndex_ = currentAnimation_->getFrameCount() - 1;
        else
            return;

        setCurrentFrame(*currentAnimation_->getFrameAt(currentFrameIndex_));
    }
}
