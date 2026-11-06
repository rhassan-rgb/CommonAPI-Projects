#include <someIp/RemoteMediaProxyImpl.hpp>

namespace mediaRemote {
namespace someIp {
namespace proxy {

    void RemoteMediaProxyImpl::subscribeToMediaInfo(std::function<void(const v1::commonapi::media::MediaTypes::MediaInfo&)> onUpdate) {
        if(!_proxy){
            return;
        }
        if (_subscription.has_value()) {
            _proxy->getCurrentMediaInfoAttribute().getChangedEvent().unsubscribe(*_subscription);
        }
        _subscription = _proxy->getCurrentMediaInfoAttribute().getChangedEvent().subscribe(onUpdate);
    }

    bool RemoteMediaProxyImpl::sendOperation(v1::commonapi::media::MediaTypes::OperationTypes op){
        CommonAPI::CallStatus callStatus = CommonAPI::CallStatus::UNKNOWN;;
        bool result = false;
        if(!_proxy || !_secOcClient)
        {
            return false;
        }
        uint64_t freshness = _secOcClient->nextFreshness();
        auto mac = _secOcClient->computeMac("doOperation", static_cast<uint8_t>(op), freshness);

        _proxy->doOperation(op, freshness, mac, callStatus, result);

        return (callStatus == CommonAPI::CallStatus::SUCCESS) && result;
    }

    bool RemoteMediaProxyImpl::sendSetVolume(uint8_t volume) {

        CommonAPI::CallStatus callStatus = CommonAPI::CallStatus::UNKNOWN;;
        bool result = false;
        if (volume > 100 || !_proxy || !_secOcClient) {
            return result;
        }
        uint64_t freshness = _secOcClient->nextFreshness();
        auto mac = _secOcClient->computeMac("setVolume", volume, freshness);

        _proxy->setVolume(volume, freshness, mac, callStatus, result);

        return (callStatus == CommonAPI::CallStatus::SUCCESS) && result;
    }

}
}
}