#include "resources/ResourceManager.h"
#include "MusicPlayer.h"

Audio::MusicPlayer::MusicPlayer(const std::string &musicPath) :
    musicFilePath_(musicPath),
    musicFiles_(ResourceHolder<sf::Music>(musicPath, {})),
    currentMusicFileName_("")
{}

void Audio::MusicPlayer::play(const std::string &song){
    if (currentMusicFileName_ !=song) {
        song_ = musicFiles_.get(song); //Throws exception if song is not loaded
        currentMusicFileName_ = song;
        play();
    }
}

void Audio::MusicPlayer::setLoop(bool isLoop){
    if (song_ && song_->getLoop() != isLoop) {
        song_->setLoop(isLoop);
        emit("loopChanged", isLoop);
    }
}

void Audio::MusicPlayer::pause(){
    if (song_ && song_->getStatus() == sf::Music::Status::Playing) {
        song_->pause();
        emit("paused");
    }
}

void Audio::MusicPlayer::play(){
    if (song_ && (song_->getStatus() == sf::Music::Status::Paused
        || song_->getStatus() == sf::Music::Status::Stopped))
    {
        song_->play();
        emit("playing", currentMusicFileName_);
    }
}

void Audio::MusicPlayer::stop() {
    if (song_ && song_->getStatus() == sf::Music::Status::Playing) {
        song_->stop();
        emit("stopped");
    }
}

void Audio::MusicPlayer::setVolume(float volume) {
    if (song_ && song_->getVolume() != volume
        && (volume >=0 && volume <= 100)) {
        song_->setVolume(volume);
        emit("volumeChanged", volume);
    }
}

Audio::Status Audio::MusicPlayer::getStatus() const {
    if (song_) {
        switch (song_->getStatus()) {
            case sf::Sound::Status::Playing:
                return Status::Playing;
            case sf::Sound::Status::Paused:
                return Status::Paused;
            case sf::Sound::Status::Stopped:
                return Status::Stopped;
        }
    }
    return Audio::Status::Stopped;
}

float Audio::MusicPlayer::getVolume() const {
    if (song_)
        return song_->getVolume();
    return 100.0f; //Default volume is maximum
}

void Audio::MusicPlayer::setPath(const std::string &path) {
    if (!path.empty())
        musicFilePath_ = path;
}

void Audio::MusicPlayer::load(std::initializer_list<std::string> filenames) {
    std::for_each(filenames.begin(), filenames.end(), [this](const auto& filename) {
        musicFiles_.load(filename);
    });
}

bool Audio::MusicPlayer::isLooped() const {
    if (song_)
        return song_->getLoop();
    return false;
}

const std::string &Audio::MusicPlayer::getAudioFilePath() const {
    return musicFilePath_;
}

const std::string &Audio::MusicPlayer::getCurrentAudioFileName() const {
    return currentMusicFileName_;
}

float Audio::MusicPlayer::getDuration() const {
    if (song_)
        return song_->getDuration().asSeconds();
    return 0.0f;
}

void Audio::MusicPlayer::seek(float position) {
    if (song_) {
        song_->setPosition(position, 0.0f, 0.0f);
        emit("playingPositionChanged", position);
    }
}

float Audio::MusicPlayer::getPosition() const {
    if (song_)
        song_->getPosition().x;
    return 0.0f;
}

void Audio::MusicPlayer::next() {
    if (musicFiles_.getSize() > 1){
        //@todo Play next song
    }
}

void Audio::MusicPlayer::prev() {
    if (song_){
        if (musicFiles_.getSize() > 1u && getDuration() <= 1.0f)
            ;//@todo Play previous song
        else{ //Restart
            stop();
            play();
        }
    }
}
