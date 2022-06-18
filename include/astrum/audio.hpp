#ifndef INCLUDE_ASTRUM_AUDIO
#define INCLUDE_ASTRUM_AUDIO

#include <memory>

#include "constants.hpp"
#include "sound.hpp"

namespace Astrum {

namespace audio {
	void InitAudio(Config &conf);
	void QuitAudio();
	void play(std::shared_ptr<Sound> source);
	void loop(std::shared_ptr<Sound> source, bool loop = true);
	void pause(std::shared_ptr<Sound> source);
	void stop(std::shared_ptr<Sound> source);
	void pauseAll();
	void resumeAll();
	void stopAll();
	bool isPlaying(const std::shared_ptr<Sound> source);
	bool isLooping(const std::shared_ptr<Sound> source);
	void seek(std::shared_ptr<Sound> source, int position);
	int currentPosition(const std::shared_ptr<Sound> source);
	double masterVolume();
	void setMasterVolume(double volume);
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_AUDIO
