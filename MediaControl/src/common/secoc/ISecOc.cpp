#include "secoc/ISecOc.hpp"
#include <openssl/hmac.h>
#include <openssl/evp.h>


namespace mediaRemote {
namespace security {

std::vector<uint8_t> ISecOc::buildPayload(const std::string& method, uint8_t argByte, uint64_t freshness) const {
    std::vector<uint8_t> payload;
    payload.insert(payload.end(), method.begin(), method.end());
    payload.push_back(argByte);
    for (int i = 1; i <= 8; i++){
        payload.push_back(static_cast<uint8_t>(freshness >> ((8 - i) * 8) & 0xFF));
    }
    return payload;
}

std::vector<uint8_t> ISecOc::computeMac(const std::string methodName, const uint8_t arg, const uint64_t freshness){
    auto payload = buildPayload(methodName, arg, freshness);
    std::vector<uint8_t> mac(MAC_SIZE, 0U);
    uint32_t macLen;
    HMAC(EVP_sha256(), _secreteKey.data(), static_cast<int>(_secreteKey.size()), payload.data(), payload.size(), mac.data(), &macLen);
    return mac;
}

}
}