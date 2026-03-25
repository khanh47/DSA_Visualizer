#include "PlaybackCommand.h"

PlaybackCommand::PlaybackCommand(std::function<void()> callback)
    : callback(std::move(callback)) {}

void PlaybackCommand::execute() {
    if (callback) {
        callback();
    }
}
