#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include <secoc/ISecOcServer.hpp>
#include <someIp/RemoteMediaStubImpl.hpp>
#include <mediaInterface/IMediaInterface.hpp>
#include <v1/commonapi/media/RemoteMediaStub.hpp>

namespace mediaRemote {
namespace testing{

    class MockSecOcServer : public security::ISecOcServer {
        public:
        MockSecOcServer() : ISecOcServer(std::array<uint8_t, 32U>{}, 0ULL) {}
        MOCK_METHOD(bool, verify, (const std::string&, uint8_t, uint64_t, const std::vector<uint8_t>&), (override));
    };

    class MockMediaBackend : public mediaRemote::mediaInterface::IMediaInterface {
        public:
        MOCK_METHOD(void, play, (), (override));
        MOCK_METHOD(void, pause, (), (override));
        MOCK_METHOD(void, next, (), (override));
        MOCK_METHOD(void, previous, (), (override));
        MOCK_METHOD(void, volUp, (), (override));
        MOCK_METHOD(void, volDown, (), (override));
        MOCK_METHOD(void, setVolume, (uint8_t), (override));
        MOCK_METHOD(mediaInterface::MediaInfo, getMediaInfo, (), (override));
    };

    class MockStubAdapter : public v1::commonapi::media::RemoteMediaStubAdapter {
        public:
        MOCK_METHOD(void, fireCurrentMediaInfoAttributeChanged, (const v1::commonapi::media::MediaTypes::MediaInfo&), (override));
        MOCK_METHOD(void, deactivateManagedInstances, (), (override));
    };

    class tes_MediaRemoteService : public ::testing::Test {
    public:
        void SetUp() override {
            auto secOc = std::make_unique<MockSecOcServer>();
            auto media = std::make_unique<MockMediaBackend>();
            _secOcRaw = secOc.get();
            _mediaRaw = media.get();
            _service = std::make_unique<mediaRemote::someIp::service::MediaRemoteServiceImpl>(
                std::move(secOc), std::move(media));
            _mockAdapter = std::make_shared<MockStubAdapter>();
            _service->initStubAdapter(_mockAdapter);
        }
    protected:
        MockSecOcServer* _secOcRaw;
        MockMediaBackend* _mediaRaw;
        std::shared_ptr<MockStubAdapter> _mockAdapter;
        std::unique_ptr<mediaRemote::someIp::service::MediaRemoteServiceImpl> _service;
    };

    using someIpOpType = mediaRemote::someIp::service::someIpOpType;
    using ::testing::_;
    using ::testing::Return;
    using ::testing::StrictMock;

    /*
     * Verifies the happy path of doOperation(PLAY):
     * SecOC verification succeeds, the media backend's play() is invoked once,
     * the refreshed media info is fetched and broadcast through the stub adapter,
     * and the client reply is true.
     */
    TEST_F(tes_MediaRemoteService, DoOperationPlayAcceptedAndBroadcasts) {
        const std::vector<uint8_t> mac(16U, 0xAB);
        bool replied = false;

        EXPECT_CALL(*_secOcRaw, verify("doOperation", static_cast<uint8_t>(someIpOpType::PLAY), 101ULL, mac))
            .Times(1).WillOnce(Return(true));
        EXPECT_CALL(*_mediaRaw, play()).Times(1);
        EXPECT_CALL(*_mediaRaw, getMediaInfo()).Times(1)
            .WillOnce(Return(mediaInterface::MediaInfo{"Song A", "Album A", "Artist A", 50, true}));
        EXPECT_CALL(*_mockAdapter, fireCurrentMediaInfoAttributeChanged(_)).Times(1);

        _service->doOperation(nullptr, someIpOpType::PLAY, 101ULL, mac, [&](bool res) { replied = res; });
        EXPECT_TRUE(replied);
    }

    /*
     * Verifies that doOperation is rejected when SecOC verification fails:
     * the media backend is not touched, no attribute change is broadcast,
     * and the client reply is false.
     */
    TEST_F(tes_MediaRemoteService, DoOperationRejectedWhenSecOcFails) {
        const std::vector<uint8_t> mac(16U, 0xAB);
        bool replied = true;

        EXPECT_CALL(*_secOcRaw, verify(_, _, _, _)).Times(1).WillOnce(Return(false));
        EXPECT_CALL(*_mediaRaw, play()).Times(0);
        EXPECT_CALL(*_mockAdapter, fireCurrentMediaInfoAttributeChanged(_)).Times(0);

        _service->doOperation(nullptr, someIpOpType::PLAY, 101ULL, mac, [&](bool res) { replied = res; });
        EXPECT_FALSE(replied);
    }

    /*
     * Verifies that setVolume with a level above 100 is rejected up front:
     * neither SecOC verification nor the media backend is called,
     * and the client reply is false.
     */
    TEST_F(tes_MediaRemoteService, SetVolumeOutOfRangeRejectedBeforeSecOc) {
        const std::vector<uint8_t> mac(16U, 0xAB);
        bool replied = true;

        EXPECT_CALL(*_secOcRaw, verify(_, _, _, _)).Times(0);
        EXPECT_CALL(*_mediaRaw, setVolume(_)).Times(0);

        _service->setVolume(nullptr, 150, 101ULL, mac, [&](bool res) { replied = res; });
        EXPECT_FALSE(replied);
    }

    /*
     * Verifies the happy path of setVolume(75):
     * SecOC verification succeeds, the backend volume is set once,
     * the updated media info (volume 75) is fetched and broadcast once,
     * and the client reply is true.
     */
    TEST_F(tes_MediaRemoteService, SetVolumeAcceptedUpdatesAndBroadcasts) {
        const std::vector<uint8_t> mac(16U, 0xAB);
        bool replied = false;

        EXPECT_CALL(*_secOcRaw, verify("setVolume", 75, 102ULL, mac)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*_mediaRaw, setVolume(75)).Times(1);
        EXPECT_CALL(*_mediaRaw, getMediaInfo()).Times(1)
            .WillOnce(Return(mediaInterface::MediaInfo{"Song A", "Album A", "Artist A", 75, true}));
        EXPECT_CALL(*_mockAdapter, fireCurrentMediaInfoAttributeChanged(_)).Times(1);

        _service->setVolume(nullptr, 75, 102ULL, mac, [&](bool res) { replied = res; });
        EXPECT_TRUE(replied);
    }

}
}
