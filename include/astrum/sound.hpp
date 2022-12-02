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
	std::shared_ptr<struct SoundData> data;
public:
	// TODO
	Sound(std::shared_ptr<struct SoundData> data);
	// TODO
	Sound(std::filesystem::path filename);
	// TODO
	Sound(std::filesystem::path filename, AudioFileType type);
	// TODO
	Sound(const unsigned char *buf, AudioFileType type);
	// TODO
	const std::shared_ptr<struct SoundData> getData() const;
	// TODO
	/**
	 * @overload
	 */
	std::shared_ptr<struct SoundData> getData();
	// TODO
	static Sound beep(double duration = 0.2, int hertz = 800);
	// TODO
	int getDuration();
	// TODO
	double volume();
	// TODO
	void setVolume(double volume);
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_SOUND
