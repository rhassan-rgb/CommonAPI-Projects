#ifndef ISEC_OC_HPP
#define ISEC_OC_HPP
#include <array>
#include <vector>
#include <string>
#include <inttypes.h>

namespace mediaRemote {
namespace security {
    class ISecOc {
    public:
        explicit ISecOc(std::array<uint8_t, 32U> key)
        :_secreteKey(key)
        {
        }
        virtual ~ISecOc() = default;
        virtual std::vector<uint8_t> computeMac(const std::string methodName, const uint8_t arg, const uint64_t freshness) final;
    protected:
        ISecOc() = delete;
        ISecOc(ISecOc&) = delete;
        ISecOc(ISecOc&&) = delete;
        ISecOc& operator=(ISecOc&) & = delete;
        ISecOc& operator=(ISecOc&&) & = delete;
        static constexpr uint8_t MAC_SIZE = 32U;

    private:
        std::array<uint8_t, 32U> _secreteKey;
        virtual std::vector<uint8_t> buildPayload(const std::string& method, uint8_t argByte, uint64_t freshness) const final;
    };

}
}

#endif