#ifndef INCLUDE_ASTRUM_SOUND
#define INCLUDE_ASTRUM_SOUND

#include <filesystem>
#include <memory>

#include "constants.hpp"

namespace Astrum {

enum class AudioFileType {
	wav, ogg, mp3, flac
};

// TODO sound API
class Sound {
private:
	std::shared_ptr<struct SoundData> data;
public:
	// Sound(std::shared_ptr<struct SoundData> data);
	// Sound(std::filesystem::path filename);
	// Sound(std::filesystem::path filename, AudioFileType type);
	// Sound(const unsigned char *buf, AudioFileType type);
	// const std::shared_ptr<struct SoundData> getData() const;
	/**
	 * @overload
	 */
	// std::shared_ptr<struct SoundData> getData();
	// static Sound beep(double duration = 0.2, int hertz = 800);
	// int getDuration();
	// double volume();
	// void setVolume(double volume);
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_SOUND
