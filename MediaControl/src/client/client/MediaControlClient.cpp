#include <client/MediaControlClient.hpp>
#include <secoc/PskConfig.hpp>
#include <logger/logger.hpp>

namespace mediaRemote {
namespace client {

bool MediaControlClient::init() {
    _runtime = CommonAPI::Runtime::get();
    _proxy = _runtime->buildProxy<v1::commonapi::media::RemoteMediaProxy>(_domain, _instance, "MediaControlClient");
    if(!_proxy) return false;
    auto secOc = std::make_unique<security::SecOcClinet>(DEFAULT_PSK, 0ULL);
    _client = std::make_unique<mediaProxyImpl>(_proxy, std::move(secOc));
    std::weak_ptr<MediaControlClient> weakSelf = weak_from_this();
    _client->subscribeToMediaInfo([weakSelf](const auto& mediaInfo) {
        if (auto weakThis = weakSelf.lock()){
            weakThis->onUpdateCallback(mediaInfo);
        }
    });
    return true;
}

bool MediaControlClient::sendOperation(someIpOpType op){
    return _client->sendOperation(op);
}

bool MediaControlClient::isAvailable() const{
    return _proxy && _proxy->isAvailable();
}

bool MediaControlClient::sendSetVolume(uint8_t vol) {
    return _client->sendSetVolume(vol);
}
void MediaControlClient::onUpdateCallback(const someIpMediInfo& info) {
    LOG("[BROADCAST] with title: ", info.getTitle(), 
        " Artist: ", info.getArtist(), 
        " Album: ", info.getAlbum(), 
        " Vol: ", static_cast<int>(info.getCurrentVolume()), 
        " playing: ", info.getIsPlaying());
    _mediaInfo = info;
}

void MediaControlClient::loop() {
    if (!isAvailable()) {
        LOG("[CLIENT] Waiting for service availability...");
        return;
    }
    if (_isCommandPending){
        _isCommandPending = false;
        someIpOpType op = maapOperationToSomeIP(_pendingCommand.command);
        bool ok = false;
        if (op == someIpOpType::INVALID){
            ok = sendSetVolume(_pendingCommand.argument);
            LOG("[CLIENT] setVolume(", static_cast<int>(_pendingCommand.argument), ") -> ", (ok ? "ACCEPTED" : "REJECTED"));
        } else {
            ok = sendOperation(op);
            LOG("[CLIENT] sendOperation(", op.toString(), ") -> ", (ok ? "ACCEPTED" : "REJECTED"));
        }
    }
}

void MediaControlClient::scheduleCommand(const commandType& command){
    _pendingCommand = command;
    _isCommandPending = true;
}

someIpOpType MediaControlClient::maapOperationToSomeIP(CONTROL_COMMANDS op){
    switch(op) {
        case CONTROL_COMMANDS::PLAY:
        {
            return someIpOpType::PLAY;
        }
        case CONTROL_COMMANDS::PAUSE:
        {
            return someIpOpType::PAUSE;
        }
        case CONTROL_COMMANDS::NEXT:
        {
            return someIpOpType::NEXT;
        }
        case CONTROL_COMMANDS::PREVIOUS:
        {
            return someIpOpType::PREVIOUS;
        }
        case CONTROL_COMMANDS::VOL_UP:
        {
            return someIpOpType::VOLUME_UP;
        }
        case CONTROL_COMMANDS::VOL_DWN:
        {
            return someIpOpType::VOLUME_DOWN;
        }
        default:
        {
            return someIpOpType::INVALID;
        }
    }
}

}
}