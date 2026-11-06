#include <service/MediaControlService.hpp>
#include <logger/logger.hpp>
#include <chrono>
#include <thread>
namespace mediaRemote {
namespace service {

bool MediaControlService::registerService() {
    return _runtime && _service && _runtime->registerService(_domain, _instance, _service);
}
bool MediaControlService::init() {
    _runtime = CommonAPI::Runtime::get();
    auto secoc = std::make_unique<security::SecOcServer>(DEFAULT_PSK, 0ULL);
    auto mediaBackend = std::make_unique<mediaInterface::MockMediaInterface>();
    _service = std::make_shared<someIp::service::MediaRemoteServiceImpl>(std::move(secoc), std::move(mediaBackend));
    
    for (int i = 0; i < 3; i++)
    {
        if (registerService())
        {
            LOG("Regesteration Successful, trial: ", (i + 1));
            return true;
        }
        LOG("Regesteration Failed, trial: ", (i + 1));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return false;
}

void MediaControlService::loop()
{
    LOG("Service running...");
}

}
}