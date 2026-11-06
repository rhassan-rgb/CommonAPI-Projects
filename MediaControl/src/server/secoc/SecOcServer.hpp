#ifndef SEC_OC_SERVER_HPP
#define SEC_OC_SERVER_HPP
#include "secoc/ISecOcServer.hpp"


namespace mediaRemote {
namespace security {
    class SecOcServer : public ISecOcServer {
    public:
        explicit SecOcServer(std::array<uint8_t, 32U> key, uint64_t freshness)
        :ISecOcServer(key, freshness)
        {
        }
        ~SecOcServer() = default;
        virtual bool verify(const std::string& method, uint8_t argByte, uint64_t receivedFreshness, const std::vector<uint8_t>& receivedMac) override;
        
    protected:
        SecOcServer() = delete;
        SecOcServer(SecOcServer&) = delete;
        SecOcServer(SecOcServer&&) = default;
        SecOcServer& operator=(SecOcServer&) & = delete;
        SecOcServer& operator=(const SecOcServer&&) & = delete;
    };

}
}

#endif