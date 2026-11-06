#include <client/MediaControlClient.hpp>
#include <chrono>
#include <thread>

int main(void) {
    
    auto client = std::make_shared<mediaRemote::client::MediaControlClient>();

    if (!client->init()) { return 1; }

    while(true) {
        client->loop();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}