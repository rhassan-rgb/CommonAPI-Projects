#include "secoc/SecOcServer.hpp"
#include <openssl/crypto.h>

namespace mediaRemote {
namespace security {

bool SecOcServer::verify(const std::string& method, uint8_t argByte, uint64_t receivedFreshness, const std::vector<uint8_t>& receivedMac){
    if (receivedFreshness <= _lastValidFreshness) {
        return false;
    }
    if (receivedMac.size() != MAC_SIZE) {
        return false;
    }
    std::vector<uint8_t> expectedMac = computeMac(method, argByte, receivedFreshness);

    bool macValid = CRYPTO_memcmp(expectedMac.data(), receivedMac.data(), MAC_SIZE) == 0;
    
    if (macValid) {
        _lastValidFreshness = receivedFreshness;
    }
    
    return macValid;
}

}
}