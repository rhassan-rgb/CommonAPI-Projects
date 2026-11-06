#ifndef REMOTE_MEDIA_CLIENT_IMPL_HPP
#define REMOTE_MEDIA_CLIENT_IMPL_HPP
#include <memory>
#include <secoc/ISecOcClient.hpp>
#include <v1/commonapi/media/RemoteMediaProxyBase.hpp>
#include <optional>

namespace mediaRemote {
namespace someIp {
namespace proxy {
using someIpProxy = v1::commonapi::media::RemoteMediaProxyBase;
class RemoteMediaProxyImpl {
public:
    explicit RemoteMediaProxyImpl(std::shared_ptr<someIpProxy> proxy, std::unique_ptr<security::ISecOcClient> secOc)
    : _proxy(proxy)
    , _secOcClient(std::move(secOc))
    {

    }
    ~RemoteMediaProxyImpl() {
        if (_proxy && _subscription.has_value())
            _proxy->getCurrentMediaInfoAttribute().getChangedEvent().unsubscribe(*_subscription);
    }
    void subscribeToMediaInfo(std::function<void(const v1::commonapi::media::MediaTypes::MediaInfo&)> onUpdate);
    bool sendOperation(v1::commonapi::media::MediaTypes::OperationTypes op);
    bool sendSetVolume(uint8_t volume);
private:
    std::shared_ptr<someIpProxy> _proxy;
    std::unique_ptr<security::ISecOcClient> _secOcClient;
    std::optional<someIpProxy::CurrentMediaInfoAttribute::ChangedEvent::Subscription> _subscription;
};

}
}
}

#endif