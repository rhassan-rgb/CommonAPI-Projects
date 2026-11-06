#ifndef I_MEDIA_INTERFACE_HPP
#define I_MEDIA_INTERFACE_HPP
#include <inttypes.h>
#include <string>

namespace mediaRemote{
namespace mediaInterface {

struct MediaInfo {
    std::string title; 
    std::string album;
    std::string artist;
    uint8_t volume; 
    bool playing;
};

class IMediaInterface {
public:
    IMediaInterface() = default;
    virtual ~IMediaInterface() = default;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual void volUp() = 0;
    virtual void volDown() = 0;
    virtual void setVolume(uint8_t vol) = 0;
    virtual MediaInfo getMediaInfo() = 0;

protected:
    IMediaInterface(IMediaInterface&) = delete;
    IMediaInterface(IMediaInterface&&) = delete;
    IMediaInterface& operator=(IMediaInterface&) = delete;
    IMediaInterface& operator=(IMediaInterface&&) = delete;
};
}
}

#endif