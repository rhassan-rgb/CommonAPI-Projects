#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include <secoc/ISecOcClient.hpp>
#include <someIp/RemoteMediaProxyImpl.hpp>
#include <v1/commonapi/media/RemoteMediaProxyBase.hpp>

namespace mediaRemote {
namespace testing{
using someIpMediaInfoAttr = v1::commonapi::media::RemoteMediaProxyBase::CurrentMediaInfoAttribute;
using someIpChangedEvent = someIpMediaInfoAttr::ChangedEvent;

    class MockSecOcClient : public security::ISecOcClient {
    public:
        MockSecOcClient(std::array<uint8_t, 32U> key) : ISecOcClient(key) {}
        MOCK_METHOD(uint64_t, nextFreshness, (), (override));
    };

    class TestableChangedEvent : public someIpChangedEvent {
    public:
        using someIpChangedEvent::notifyListeners;
    };

    class MockMediaInfoAttribute : public someIpMediaInfoAttr {
    public:
        MOCK_METHOD(void, getValue, (CommonAPI::CallStatus&, v1::commonapi::media::MediaTypes::MediaInfo&, const CommonAPI::CallInfo*), (const, override));
        MOCK_METHOD(std::future<CommonAPI::CallStatus>, getValueAsync, (AttributeAsyncCallback, const CommonAPI::CallInfo*), (override));
        MOCK_METHOD(ChangedEvent&, getChangedEvent, (), (override));
    };

    class MockRemoteMediaProxy : public v1::commonapi::media::RemoteMediaProxyBase { 
    public:
        MOCK_METHOD(bool, isAvailable, (), (const, override));
        MOCK_METHOD(bool, isAvailableBlocking, (), (const, override));
        MOCK_METHOD(CommonAPI::ProxyStatusEvent&, getProxyStatusEvent, (), (override));
        MOCK_METHOD(CommonAPI::InterfaceVersionAttribute&, getInterfaceVersionAttribute, (), (override));
        MOCK_METHOD(CurrentMediaInfoAttribute&, getCurrentMediaInfoAttribute, (), (override));
        MOCK_METHOD(void, doOperation, (v1::commonapi::media::MediaTypes::OperationTypes, uint64_t, v1::commonapi::media::MediaTypes::MacType, CommonAPI::CallStatus&, bool&, const CommonAPI::CallInfo*), (override));
        MOCK_METHOD(std::future<CommonAPI::CallStatus>, doOperationAsync, (const v1::commonapi::media::MediaTypes::OperationTypes&, const uint64_t&, const v1::commonapi::media::MediaTypes::MacType&, DoOperationAsyncCallback, const CommonAPI::CallInfo*), (override));
        MOCK_METHOD(void, setVolume, (uint8_t, uint64_t, v1::commonapi::media::MediaTypes::MacType, CommonAPI::CallStatus&, bool&, const CommonAPI::CallInfo*), (override));
        MOCK_METHOD(std::future<CommonAPI::CallStatus>, setVolumeAsync, (const uint8_t&, const uint64_t&, const v1::commonapi::media::MediaTypes::MacType&, SetVolumeAsyncCallback, const CommonAPI::CallInfo*), (override));
        MOCK_METHOD(std::future<void>, getCompletionFuture, (), (override));
    };

    class test_MediaRemoteClient : public ::testing::Test {
    public:
        void SetUp() override {
            auto secOc = std::make_unique<MockSecOcClient>(std::array<uint8_t, 32U>{});
            _secOcRaw = secOc.get();
            _mockProxy = std::make_shared<MockRemoteMediaProxy>();
            _client = std::make_unique<mediaRemote::someIp::proxy::RemoteMediaProxyImpl>(
                _mockProxy, std::move(secOc));
        }

        MockSecOcClient* _secOcRaw;
        std::shared_ptr<MockRemoteMediaProxy> _mockProxy;
        MockMediaInfoAttribute _mockAttribute;
        TestableChangedEvent _changedEvent;
        std::unique_ptr<mediaRemote::someIp::proxy::RemoteMediaProxyImpl> _client;
    };

    using OperationTypes = v1::commonapi::media::MediaTypes::OperationTypes;
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::Return;
    using ::testing::ReturnRef;
    using ::testing::SetArgReferee;

    /*
     * Verifies sendOperation(PLAY) on the happy path: a fresh value is requested,
     * the MAC is computed over ("doOperation", PLAY, freshness), the proxy is called
     * with exactly those values, and a SUCCESS call status with result true yields true.
     */
    TEST_F(test_MediaRemoteClient, SendOperationSuccess) {
        EXPECT_CALL(*_secOcRaw, nextFreshness()).WillOnce(Return(42ULL));
        auto expectedMac = _secOcRaw->computeMac("doOperation", static_cast<uint8_t>(OperationTypes::PLAY), 42ULL);
        EXPECT_CALL(*_mockProxy, doOperation(OperationTypes(OperationTypes::PLAY), 42ULL, expectedMac, _, _, _))
            .WillOnce(DoAll(SetArgReferee<3>(CommonAPI::CallStatus::SUCCESS), SetArgReferee<4>(true)));

        EXPECT_TRUE(_client->sendOperation(OperationTypes::PLAY));
    }

    /*
     * Verifies sendOperation returns false when the call itself succeeds but the
     * server rejects the request (result == false, e.g. SecOC verification failure).
     */
    TEST_F(test_MediaRemoteClient, SendOperationRejectedByServerOrCallFailure) {
        EXPECT_CALL(*_secOcRaw, nextFreshness()).WillOnce(Return(42ULL));
        auto expectedMac = _secOcRaw->computeMac("doOperation", static_cast<uint8_t>(OperationTypes::PLAY), 42ULL);
        EXPECT_CALL(*_mockProxy, doOperation(OperationTypes(OperationTypes::PLAY), 42ULL, expectedMac, _, _, _))
            .WillOnce(DoAll(SetArgReferee<3>(CommonAPI::CallStatus::SUCCESS), SetArgReferee<4>(false)));

        EXPECT_FALSE(_client->sendOperation(OperationTypes::PLAY));
    }

    /*
     * Verifies sendSetVolume(80) on the happy path: the MAC is computed over
     * ("setVolume", 80, freshness), the proxy receives matching arguments,
     * and a SUCCESS call status with result true yields true.
     */
    TEST_F(test_MediaRemoteClient, SendSetVolumeSuccess) {
        EXPECT_CALL(*_secOcRaw, nextFreshness()).WillOnce(Return(43ULL));
        auto expectedMac = _secOcRaw->computeMac("setVolume", 80U, 43ULL);
        EXPECT_CALL(*_mockProxy, setVolume(80U, 43ULL, expectedMac, _, _, _))
            .WillOnce(DoAll(SetArgReferee<3>(CommonAPI::CallStatus::SUCCESS), SetArgReferee<4>(true)));

        EXPECT_TRUE(_client->sendSetVolume(80U));
    }

    /*
     * Verifies subscribeToMediaInfo registers the callback on the proxy attribute's
     * changed event, and that a broadcast delivers the new media info to the callback.
     */
    TEST_F(test_MediaRemoteClient, SubscribeToMediaInfoReceivesBroadcast) {
        EXPECT_CALL(*_mockProxy, getCurrentMediaInfoAttribute()).Times(2).WillRepeatedly(ReturnRef(_mockAttribute));
        EXPECT_CALL(_mockAttribute, getChangedEvent()).Times(2).WillRepeatedly(ReturnRef(_changedEvent));

        std::string receivedTitle;
        uint8_t receivedVolume = 0;
        bool called = false;
        _client->subscribeToMediaInfo([&](const v1::commonapi::media::MediaTypes::MediaInfo& info) {
            called = true;
            receivedTitle = info.getTitle();
            receivedVolume = info.getCurrentVolume();
        });

        _changedEvent.notifyListeners(v1::commonapi::media::MediaTypes::MediaInfo{"Track 1", "Album 1", "Artist 1", 65, true});

        EXPECT_TRUE(called);
        EXPECT_EQ(receivedTitle, "Track 1");
        EXPECT_EQ(receivedVolume, 65);
    }

}
}
