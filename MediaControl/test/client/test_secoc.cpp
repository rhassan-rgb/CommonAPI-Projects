#include <gtest/gtest.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "secoc/SecOcClient.hpp"

namespace mediaRemote{
namespace testing{

class SecOcClient_test : public ::testing::Test {
    
protected:
    std::array<uint8_t, 32U> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};
    
    mediaRemote::security::SecOcClinet _secOc{key, 0ULL};    
};
    
    
TEST_F(SecOcClient_test, FreshnessStrictlyIncreasesOnBackToBackCalls) {
    auto freshness1 = _secOc.nextFreshness();
    auto freshness2 = _secOc.nextFreshness();
    ASSERT_GT(freshness2, freshness1);
}


TEST_F(SecOcClient_test, FreshnessRespectsHighInitialValue) {
    mediaRemote::security::SecOcClinet futureClient(key, UINT64_MAX - 5ULL);
    auto f = futureClient.nextFreshness();
    ASSERT_EQ(f, UINT64_MAX - 4ULL);
}

}
}