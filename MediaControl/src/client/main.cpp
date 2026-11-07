#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <client/MediaControlClient.hpp>
void printMenu(){
    // std::cout << "0: sendOperation(someIpOpType::PLAY)"
// - 1: sendOperation(someIpOpType::NEXT)
// - 2: sendOperation(someIpOpType::VOLUME_UP)
// - 3: sendSetVolume(75)
// - 4: sendOperation(someIpOpType::VOLUME_DOWN)
// - 5: sendOperation(someIpOpType::PREVIOUS)
// - 6: sendOperation(someIpOpType::PAUSE)
// - 7: sendSetVolume(150) (tests rejection)
}

std::vector<mediaRemote::client::commandType> commands{
    {mediaRemote::client::CONTROL_COMMANDS::PLAY},
    {mediaRemote::client::CONTROL_COMMANDS::PAUSE},
    {mediaRemote::client::CONTROL_COMMANDS::NEXT},
    {mediaRemote::client::CONTROL_COMMANDS::PREVIOUS},
    {mediaRemote::client::CONTROL_COMMANDS::VOL_UP},
    {mediaRemote::client::CONTROL_COMMANDS::VOL_SET, 75},
    {mediaRemote::client::CONTROL_COMMANDS::VOL_DWN},
    {mediaRemote::client::CONTROL_COMMANDS::VOL_SET, 150},
};

int main(void) {
    
    auto client = std::make_shared<mediaRemote::client::MediaControlClient>();

    if (!client->init()) { return 1; }
    int index = 0;
    while(true) {
        index %= commands.size();
        client->loop();
        client->scheduleCommand(commands[index++]);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}