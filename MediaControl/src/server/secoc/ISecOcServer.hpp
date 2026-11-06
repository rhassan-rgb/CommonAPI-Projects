#ifndef ISEC_OC_SERVER_HPP
#define ISEC_OC_SERVER_HPP
#include "secoc/ISecOc.hpp"


namespace mediaRemote {
namespace security {
    class ISecOcServer : protected ISecOc {
    public:
        explicit ISecOcServer(std::array<uint8_t, 32U> key, uint64_t freshness)
        :ISecOc(key)
        ,_lastValidFreshness(freshness)
        {
        }
        ~ISecOcServer() = default;
        virtual bool verify(const std::string& method, uint8_t argByte, uint64_t receivedFreshness, const std::vector<uint8_t>& receivedMac) = 0;
        
    protected:
        ISecOcServer() = delete;
        ISecOcServer(ISecOcServer&) = delete;
        ISecOcServer(ISecOcServer&&) = delete;
        ISecOcServer& operator=(ISecOcServer&) & = delete;
        ISecOcServer& operator=(const ISecOcServer&&) & = delete;
        uint64_t _lastValidFreshness;
    };

}
}

#endif