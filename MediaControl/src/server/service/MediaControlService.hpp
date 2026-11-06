#ifndef MEDIA_CONTROL_SERVICE_HPP
#define MEDIA_CONTROL_SERVICE_HPP
#include <new>
#include <memory>
#include <string>
#include <CommonAPI/CommonAPI.hpp>
#include <secoc/SecOcServer.hpp>
#include <mediaInterface/MockMediaInterface.hpp>
#include <someIp/RemoteMediaStubImpl.hpp>
#include <secoc/PskConfig.hpp>

namespace mediaRemote {
namespace service {

class MediaControlService {
public:
    MediaControlService()
    {
    }
    void loop();
    bool init();
    
private:

    bool registerService();
    std::shared_ptr<CommonAPI::Runtime> _runtime;
    const std::string _domain {"local"};
    const std::string _instance {"commonapi.media.RemoteMedia"};
    // std::unique_ptr<security::SecOcServer> _secoc;
    // std::unique_ptr<mediaInterface::MockMediaInterface> _mediaBackend;
    std::shared_ptr<mediaRemote::someIp::service::MediaRemoteServiceImpl> _service;

};
}
}
#endif