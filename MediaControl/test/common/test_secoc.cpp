#include <gtest/gtest.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "secoc/ISecOc.hpp"

namespace mediaRemote{
namespace testing{

class SecOc_test : public ::testing::Test {
    
protected:
    std::array<uint8_t, 32U> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};
    
    mediaRemote::security::ISecOc _secOc{key};    
};
    
    
TEST_F(SecOc_test, MacSizeIs32Bytes) {
    auto mac = _secOc.computeMac("setVolume", 50, 1000);
    ASSERT_EQ(mac.size(), 32U);
}


TEST_F(SecOc_test, DeterministicForSameInputs) {
    auto mac1 = _secOc.computeMac("setVolume", 51, 1000);
    auto mac2 = _secOc.computeMac("setVolume", 51, 1000);
    ASSERT_EQ(mac1, mac2);
}

TEST_F(SecOc_test, ChangesWhenMethodArgOrFreshnessChanges){
    auto mac1 = _secOc.computeMac("setVolume", 51, 1000);
    auto mac2 = _secOc.computeMac("setVolume", 50, 1000);
    ASSERT_NE(mac1, mac2);
    
    mac1 = _secOc.computeMac("setVolume", 50, 1001);
    mac2 = _secOc.computeMac("setVolume", 50, 1000);
    ASSERT_NE(mac1, mac2);

    mac1 = _secOc.computeMac("setVo1ume", 50, 1000);
    mac2 = _secOc.computeMac("setVolume", 50, 1000);
    ASSERT_NE(mac1, mac2);
}

/*Computes computeMac("doOperation", 0x01, 0x0102030405060708ULL) and compares it with 
HMAC(EVP_sha256(), ...) on the raw byte array {'p','l','a','y', 0x01, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01} 
to prove the smallest byte (0x08) is placed first!*/
TEST_F(SecOc_test, LittleEndianFreshnessSerialization){
    auto mac1 = _secOc.computeMac("doOperation", 0x01, 0x0102030405060708ULL);
    std::vector<uint8_t> mac2(32, 0);
    uint32_t macLen;
    std::vector<uint8_t> payload = {'d', 'o', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
                                    0x01, 
                                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; 
    HMAC(EVP_sha256(), key.data(), static_cast<int>(key.size()), payload.data(), payload.size(), mac2.data(), &macLen);
    ASSERT_EQ(mac1, mac2);
}


}
}