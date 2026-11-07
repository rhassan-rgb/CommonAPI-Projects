#ifndef MEDIA_CONTROL_CLIENT_HPP
#define MEDIA_CONTROL_CLIENT_HPP
#include <memory>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/commonapi/media/RemoteMediaProxy.hpp>
#include <secoc/SecOcClient.hpp>
#include <someIp/RemoteMediaProxyImpl.hpp>


namespace mediaRemote {
namespace client {

enum class CONTROL_COMMANDS : uint8_t {
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    VOL_UP,
    VOL_DWN,
    VOL_SET,
    INVALID
};

struct commandType
{
    CONTROL_COMMANDS command = CONTROL_COMMANDS::INVALID;
    uint8_t argument = 0;
};


using someIpOpType = v1::commonapi::media::MediaTypes::OperationTypes;
using someIpMediInfo = v1::commonapi::media::MediaTypes::MediaInfo;
using someIpProxy = v1::commonapi::media::RemoteMediaProxy<>;
using mediaProxyImpl = mediaRemote::someIp::proxy::RemoteMediaProxyImpl;
class MediaControlClient : public std::enable_shared_from_this<MediaControlClient> {
public: 
    MediaControlClient(): _isCommandPending(false){}
    ~MediaControlClient() {
    }
    bool isAvailable() const;
    bool sendOperation(someIpOpType op);
    bool sendSetVolume(uint8_t volume);
    void onUpdateCallback (const someIpMediInfo& mediaInfo);
    bool init();
    void loop();
    void scheduleCommand(const commandType& command);
private:
    const std::string _domain {"local"};
    const std::string _instance {"commonapi.media.RemoteMedia"};
    std::shared_ptr<CommonAPI::Runtime> _runtime;
    std::shared_ptr<someIpProxy> _proxy;
    std::unique_ptr<mediaProxyImpl> _client;
    someIpMediInfo _mediaInfo;
    bool _isCommandPending;
    commandType _pendingCommand;
    someIpOpType maapOperationToSomeIP(CONTROL_COMMANDS op);
};

}
}
#endif