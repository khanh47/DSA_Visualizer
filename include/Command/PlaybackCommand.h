#pragma once

#include <functional>
#include <memory>

#include "ICommand.h"

class PlaybackCommand : public ICommand {
public:
    explicit PlaybackCommand(std::function<void()> callback);

    void execute() override;

private:
    std::function<void()> callback;
};

inline std::unique_ptr<ICommand> createPlaybackCommand(std::function<void()> callback) {
    return std::unique_ptr<ICommand>(new PlaybackCommand(std::move(callback)));
}
