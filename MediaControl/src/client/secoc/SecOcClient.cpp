#include <chrono>

#include "secoc/SecOcClient.hpp"


namespace mediaRemote {
namespace security {


uint64_t SecOcClinet::nextFreshness(){
    auto now = std::chrono::system_clock::now();
    uint64_t epochMs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    if (epochMs <= _lastGeneratedFreshness) {
        epochMs = _lastGeneratedFreshness + 1U;
    }

    _lastGeneratedFreshness = epochMs;
    return epochMs;
}


}
}