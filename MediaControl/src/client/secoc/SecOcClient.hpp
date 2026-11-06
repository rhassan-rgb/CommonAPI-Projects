#ifndef SEC_OC_CLIENT_HPP
#define SEC_OC_CLIENT_HPP
#include "secoc/ISecOcClient.hpp"


namespace mediaRemote {
namespace security {
    class SecOcClinet : public ISecOcClient {
    public:
        explicit SecOcClinet(std::array<uint8_t, 32U> key, uint64_t freshness)
        :ISecOcClient(key)
        ,_lastGeneratedFreshness(freshness)
        {
        }
        ~SecOcClinet() = default;
        uint64_t nextFreshness() override;

    protected:
        SecOcClinet() = delete;
        SecOcClinet(SecOcClinet&) = delete;
        SecOcClinet(const SecOcClinet&&) = delete;
        SecOcClinet& operator=(SecOcClinet&) & = delete;
        SecOcClinet& operator=(const SecOcClinet&&) & = delete;
    private:
        uint64_t _lastGeneratedFreshness;
    };

}
}

#endif