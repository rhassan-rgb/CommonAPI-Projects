#ifndef REMOTE_MEDIA_STUB_IMPL_HPP
#define REMOTE_MEDIA_STUB_IMPL_HPP
#include <mutex>
#include <memory>
#include "secoc/ISecOcServer.hpp"
#include "mediaInterface/IMediaInterface.hpp"

#include <v1/commonapi/media/RemoteMediaStubDefault.hpp>

namespace mediaRemote {
namespace someIp {
namespace service {

using someIpMediaSkel = ::v1::commonapi::media::RemoteMediaStubDefault;
using someIpMediaInfo = ::v1::commonapi::media::MediaTypes::MediaInfo;
using someIpOpType = v1::commonapi::media::MediaTypes::OperationTypes;
using someIpMactype = v1::commonapi::media::MediaTypes::MacType;

class MediaRemoteServiceImpl : public someIpMediaSkel {
public:
    explicit MediaRemoteServiceImpl(std::unique_ptr<security::ISecOcServer> secOcServer,
    std::unique_ptr<mediaInterface::IMediaInterface> mediaInterface)
    : _secOcServer(std::move(secOcServer))
    , _mediaInterface(std::move(mediaInterface))
    , _currentMedia("title", "album", "artist", 20, false)
    ,_mutex()
    {

    }
    void doOperation(const std::shared_ptr<CommonAPI::ClientId> client,
                     someIpOpType op, 
                     uint64_t freshnessValue,
                     someIpMactype mac,
                     doOperationReply_t reply) override;

    void setVolume(const std::shared_ptr<CommonAPI::ClientId> client, 
                   uint8_t volumeLevel,
                   uint64_t freshnessValue,
                   someIpMactype mac,
                   setVolumeReply_t reply) override;

private:
    std::unique_ptr<security::ISecOcServer> _secOcServer;
    std::unique_ptr<mediaInterface::IMediaInterface> _mediaInterface;
    someIpMediaInfo _currentMedia;
    std::mutex _mutex;
    void updateMediaInfo(const mediaInterface::MediaInfo& src);

};

}
}
}

#endif