#include <service/MediaControlService.hpp>
#include <chrono>
#include <thread>

int main(void) {
    mediaRemote::service::MediaControlService service;

    if (!service.init()) { return 1; }

    while(true) {
        service.loop();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}