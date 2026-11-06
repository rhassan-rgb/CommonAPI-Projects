#include <gtest/gtest.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "secoc/SecOcServer.hpp"
#include "secoc/ISecOc.hpp"

namespace mediaRemote{
namespace testing{

class SecOcServer_test : public ::testing::Test {
    
protected:
    std::array<uint8_t, 32U> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};
    
    mediaRemote::security::SecOcServer _server{key, 100LL};
    mediaRemote::security::ISecOc _macHelper{key};
    
};
    
    
TEST_F(SecOcServer_test, AcceptsValidCommand) {
    auto mac = _macHelper.computeMac("setVolume", 50, 101);
    ASSERT_TRUE(_server.verify("setVolume", 50, 101ULL, mac));
}


TEST_F(SecOcServer_test, RejectsReplayAttack) {
    auto mac = _macHelper.computeMac("setVolume", 50, 101);
    ASSERT_TRUE(_server.verify("setVolume", 50, 101ULL, mac));
    ASSERT_FALSE(_server.verify("setVolume", 50, 101ULL, mac));
}

TEST_F(SecOcServer_test, RejectsTamperedArgumentOrMac){
    auto mac = _macHelper.computeMac("setVolume", 50, 200);
    ASSERT_FALSE(_server.verify("setVolume", 99, 200ULL, mac));
    mac[0] ^= 0xFF;
    ASSERT_FALSE(_server.verify("setVolume", 50, 200ULL, mac));
    mac[0] ^= 0xFF;
    ASSERT_TRUE(_server.verify("setVolume", 50, 200ULL, mac));
    

}

/*Computes computeMac("doOperation", 0x01, 0x0102030405060708ULL) and compares it with 
HMAC(EVP_sha256(), ...) on the raw byte array {'p','l','a','y', 0x01, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01} 
to prove the smallest byte (0x08) is placed first!*/
TEST_F(SecOcServer_test, RejectsInvalidMacLength){
    std::vector<uint8_t> mac(16, 0);
    ASSERT_FALSE(_server.verify("setVolume", 50, 200ULL, mac));
}


}
}