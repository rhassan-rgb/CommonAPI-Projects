#ifndef MOCK_0_MEDIA_INTERFACE
#define MOCK_0_MEDIA_INTERFACE
#include <vector>
#include <string>
#include <mediaInterface/IMediaInterface.hpp>

namespace mediaRemote{
namespace mediaInterface {

struct Track{
    std::string title; 
    std::string album;
    std::string artist;
};

class MockMediaInterface : public IMediaInterface {
public:
    MockMediaInterface(): _mediaInfo(), _trackIdx(0) {}
    inline void play() override {
        _mediaInfo.playing = true;
    }
    void pause() override{
        _mediaInfo.playing = false; 
    }
    void next() override{
        _trackIdx++;
        _trackIdx %= _trackList.size();
    }
    void previous() override{
        if(_trackIdx == 0) {
            _trackIdx = _trackList.size() - 1;
        } else {
            _trackIdx--;
        }
    }
    void volUp() override {
        if (_mediaInfo.volume + VOLUME_STEP < 100)
            _mediaInfo.volume += VOLUME_STEP;
        else 
            _mediaInfo.volume = 100;
    }
    void volDown() override {
        if (_mediaInfo.volume > VOLUME_STEP)
            _mediaInfo.volume -= VOLUME_STEP;
        else 
            _mediaInfo.volume = 0;
    }
    void setVolume(uint8_t vol) override {
        if (vol > 100)
            return;
        _mediaInfo.volume = vol;
    }
    MediaInfo getMediaInfo() override{
        _mediaInfo.album = _trackList[_trackIdx].album;
        _mediaInfo.artist = _trackList[_trackIdx].artist;
        _mediaInfo.title = _trackList[_trackIdx].title;
        return _mediaInfo;
    }
    
private:
    MediaInfo _mediaInfo;
    std::vector<Track> _trackList = {
    // Classic Rock & Pop
    {"Bohemian Rhapsody", "A Night at the Opera", "Queen"},
    {"Hotel California", "Hotel California", "Eagles"},
    {"Dreams", "Rumours", "Fleetwood Mac"},
    {"Billie Jean", "Thriller", "Michael Jackson"},
    {"Rocket Man", "Honky Château", "Elton John"},
    
    // Modern Pop & Synth-Pop
    {"Blinding Lights", "After Hours", "The Weeknd"},
    {"Cruel Summer", "Lover", "Taylor Swift"},
    {"As It Was", "Harry's House", "Harry Styles"},
    {"Flowers", "Endless Summer Vacation", "Miley Cyrus"},
    {"Levitating", "Future Nostalgia", "Dua Lipa"},
    
    // Hip Hop & R&B
    {"SICKO MODE", "ASTROWORLD", "Travis Scott"},
    {"HUMBLE.", "DAMN.", "Kendrick Lamar"},
    {"Blame Game", "My Beautiful Dark Twisted Fantasy", "Kanye West"},
    {"God's Plan", "Scorpion", "Drake"},
    {"Redbone", "Awaken, My Love!", "Childish Gambino"},
    
    // Alternative & Indie Rock
    {"Do I Wanna Know?", "AM", "Arctic Monkeys"},
    {"Mr. Brightside", "Hot Fuss", "The Killers"},
    {"Take Me Out", "Franz Ferdinand", "Franz Ferdinand"},
    {"Dog Days Are Over", "Lungs", "Florence + The Machine"},
    {"Lost in the Dream", "Lost in the Dream", "The War on Drugs"}
    };
    uint8_t _trackIdx;
    static constexpr uint8_t VOLUME_STEP = 10;
};

}
}

#endif //MOCK_0_MEDIA_INTERFACE