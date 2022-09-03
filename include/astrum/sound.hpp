#ifndef INCLUDE_ASTRUM_SOUND
#define INCLUDE_ASTRUM_SOUND

#include <filesystem>
#include <memory>

#include "constants.hpp"

namespace Astrum {

enum class AudioFileType {
	wav, ogg, mp3, flac
};

class Sound {
private:
	struct SoundData *data;
public:
	Sound(Sound &src);
	Sound(struct SoundData &data);
	Sound(std::filesystem::path filename);
	Sound(std::filesystem::path filename, AudioFileType type);
	Sound(const unsigned char *buf, AudioFileType type);
	~Sound();
	struct SoundData *getData();
	static std::unique_ptr<Sound> beep(double duration = 0.2, int hertz = 800);
	int getDuration();
	double volume();
	void setVolume(double volume);
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_SOUND
