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

#include "IME/core/audio/AudioManager.h"
#include "IME/core/audio/Music.h"
#include "IME/core/audio/SoundEffect.h"
#include "IME/utility/Helpers.h"

namespace ime::audio {
    AudioManager::AudioManager() :
        masterVolume_{100.0f},
        sfxVolume_{100.0f},
        musicVolume_{100.0f},
        isMuted_(false)
    {}

    Audio* AudioManager::play(Type audioType, const std::string &filename) {
        std::unique_ptr<Audio> audio;
        if (audioType == Type::Music) {
            audio = std::make_unique<Music>();
            audio->setVolume(musicVolume_);
        } else if (audioType == Type::Sfx) {
            audio = std::make_unique<SoundEffect>();
            audio->setVolume(sfxVolume_);
        }

        audio->setSource(filename);
        audio->play();
        playingAudio_.push_back(std::move(audio));
        return playingAudio_.back().get();
    }

    void AudioManager::setVolumeFor(Type audioType, float volume) {
        if (volume > masterVolume_)
            volume = masterVolume_;

        if (audioType == Type::Music)
            musicVolume_ = volume;
        else if (audioType == Type::Sfx)
            sfxVolume_ = volume;
    }

    void AudioManager::playAllAudio() {
        for (auto& audio : playingAudio_)
            audio->play();
    }

    void AudioManager::pauseAllAudio() {
        for (auto& audio : playingAudio_)
            audio->pause();
    }

    void AudioManager::stopAllAudio() {
        for (auto& audio : playingAudio_)
            audio->stop();
    }

    void AudioManager::setMute(bool isMuted) {
        if (isMuted_ == isMuted)
            return;
        isMuted_ = isMuted;
        for (auto& audio : playingAudio_)
            audio->setMute(isMuted);
        eventEmitter_.emit("muteChanged", isMuted_);
    }

    float AudioManager::getVolumeFor(Type audioType) const {
        switch (audioType) {
            case Type::Sfx:
                return sfxVolume_;
            case Type::Music:
                return musicVolume_;
        }
        return masterVolume_;
    }

    void AudioManager::setMasterVolume(float volume) {
        if (masterVolume_ != volume) {
            auto wasMusicVolumeSameAsMaster = musicVolume_ == masterVolume_;
            auto wasSfxVolumeSameAsMaster = sfxVolume_ == masterVolume_;
            if (volume > 100.0f)
                masterVolume_ = 100.0f;
            else if (volume < 0.0f)
                masterVolume_ = 0.0f;
            else
                masterVolume_ = volume;

            if (musicVolume_ > masterVolume_ || wasMusicVolumeSameAsMaster)
                musicVolume_ = masterVolume_;

            if (sfxVolume_ > masterVolume_ || wasSfxVolumeSameAsMaster)
                sfxVolume_ = masterVolume_;

            std::for_each(playingAudio_.begin(), playingAudio_.end(),
                [this](auto& audio) {
                    if (audio->getType() == "Music")
                        audio->setVolume(musicVolume_);
                    else
                        audio->setVolume(sfxVolume_);
            });

            eventEmitter_.emit("volumeChanged", masterVolume_);
        }
    }

    void AudioManager::adjustMasterVolume(float offset) {
        setMasterVolume(getMasterVolume() + offset);
    }

    float AudioManager::getMasterVolume() const {
        return masterVolume_;
    }

    void AudioManager::onMute(Callback<bool> callback) {
        eventEmitter_.addEventListener("muteChanged", std::move(callback));
    }

    void AudioManager::onVolumeChanged(Callback<float> callback) {
        eventEmitter_.addEventListener("volumeChanged", std::move(callback));
    }

    void AudioManager::removePlayedAudio() {
        playingAudio_.erase(std::remove_if(playingAudio_.begin(), playingAudio_.end(),
            [] (std::unique_ptr<Audio>& audio) {
                return audio->getStatus() == Status::Stopped;
        }), playingAudio_.end());
    }
}
