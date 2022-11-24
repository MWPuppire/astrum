#ifndef INCLUDE_ASTRUM_AUDIO
#define INCLUDE_ASTRUM_AUDIO

#include "constants.hpp"
#include "sound.hpp"

namespace Astrum {

namespace audio {

	// TODO
	void play(Sound &source);
	// TODO
	void loop(Sound &source, bool loop = true);
	// TODO
	void pause(Sound &source);
	// TODO
	void stop(Sound &source);
	// TODO
	void pauseAll();
	// TODO
	void resumeAll();
	// TODO
	void stopAll();
	// TODO
	bool isPlaying(const Sound source);
	// TODO
	bool isLooping(const Sound source);
	// TODO
	void seek(Sound &source, int position);
	// TODO
	int currentPosition(const Sound source);
	// TODO
	double masterVolume();
	// TODO
	void setMasterVolume(double volume);
};

}; // namespace Astrum

#endif // ifndef INCLUDE_ASTRUM_AUDIO
