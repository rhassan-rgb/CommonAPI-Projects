#ifndef ISEC_OC_CLIENT_HPP
#define ISEC_OC_CLIENT_HPP
#include "secoc/ISecOc.hpp"


namespace mediaRemote {
namespace security {
    class ISecOcClient : public ISecOc {
    public:
        explicit ISecOcClient(std::array<uint8_t, 32U> key)
        :ISecOc(key)
        {
        }
        virtual~ISecOcClient() = default;
        virtual uint64_t nextFreshness() = 0;

    protected:
        ISecOcClient() = delete;
        ISecOcClient(ISecOcClient&) = delete;
        ISecOcClient(const ISecOcClient&&) = delete;
        ISecOcClient& operator=(ISecOcClient&) & = delete;
        ISecOcClient& operator=(const ISecOcClient&&) & = delete;
    };

}
}

#endif