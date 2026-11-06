#include "someIp/RemoteMediaStubImpl.hpp"
#include "logger/logger.hpp"

namespace mediaRemote {
namespace someIp {
namespace service {

void MediaRemoteServiceImpl::doOperation(const std::shared_ptr<CommonAPI::ClientId> client,
                                        someIpOpType op, 
                                        uint64_t freshnessValue,
                                        someIpMactype mac,
                                        doOperationReply_t reply){
    if(!op.validate()) {
        LOG("[REJECTED] doOperation: OP:", op.toString(), ", freshnessValue: ", freshnessValue);
        reply(false);
        return;
    }
    if (!_secOcServer || !_mediaInterface) {
        LOG("[REJECTED] NULL POINTERS");
        reply(false);
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    bool isValidOp = _secOcServer->verify("doOperation", static_cast<uint8_t>(op), freshnessValue, mac);
    if(isValidOp) {
        LOG("[ACCEPTED] doOperation: OP:", op.toString(), ", freshnessValue: ", freshnessValue);
        switch(static_cast<someIpOpType::Literal>(op.value_)) {
            case someIpOpType::PLAY:
            {
                _mediaInterface->play();
                break;
            }
            case someIpOpType::PAUSE:
            {
                _mediaInterface->pause();
                break;
            }
            case someIpOpType::NEXT:
            {
                _mediaInterface->next();
                break;
            }
            case someIpOpType::PREVIOUS:
            {
                _mediaInterface->previous();
                break;
            }
            case someIpOpType::VOLUME_UP:
            {
                _mediaInterface->volUp();
                break;
            }
            case someIpOpType::VOLUME_DOWN:
            {
                _mediaInterface->volDown();
                break;
            }
            default:
                /*do nothing*/
                break;
        }
        reply(true);
        updateMediaInfo(_mediaInterface->getMediaInfo());
    } else {
        LOG("[REJECTED] doOperation: OP:", op.toString(), ", freshnessValue: ", freshnessValue);
        reply(false);
    }
}

void MediaRemoteServiceImpl::setVolume(const std::shared_ptr<CommonAPI::ClientId> client, 
                   uint8_t volumeLevel,
                   uint64_t freshnessValue,
                   someIpMactype mac,
                   setVolumeReply_t reply) {
    constexpr uint8_t MAX_VOLUME{100};
    if(volumeLevel > MAX_VOLUME) {
        LOG("[REJECTED] setVolume: Vol:", static_cast<int>(volumeLevel), ", freshnessValue: ", freshnessValue);
        reply(false);
        return;
    }
    if (!_secOcServer || !_mediaInterface) {
        LOG("[REJECTED] NULL POINTERS");
        reply(false);
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    bool isValidOp = _secOcServer->verify("setVolume", static_cast<uint8_t>(volumeLevel), freshnessValue, mac);
    if(isValidOp) {
        LOG("[ACCEPTED] setVolume: Vol:", static_cast<int>(volumeLevel), ", freshnessValue: ", freshnessValue);
        _mediaInterface->setVolume(volumeLevel);
        reply(true);
        updateMediaInfo(_mediaInterface->getMediaInfo());
    } else {
        LOG("[REJECTED] setVolume: Vol:", static_cast<int>(volumeLevel), ", freshnessValue: ", freshnessValue);
        reply(false);
    }
}

void MediaRemoteServiceImpl::updateMediaInfo(const mediaInterface::MediaInfo& src) {
    _currentMedia.setTitle(src.title);
    _currentMedia.setAlbum(src.album);
    _currentMedia.setArtist(src.artist);
    _currentMedia.setCurrentVolume(src.volume);
    _currentMedia.setIsPlaying(src.playing);
    setCurrentMediaInfoAttribute(_currentMedia);
}

}
}
}