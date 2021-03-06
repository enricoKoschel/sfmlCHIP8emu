#pragma once
#include <SFML/Audio.hpp>
#include <thread>

template <typename T> inline int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct sound {
private:
	static void threadPlay(sf::SoundBuffer soundBuffer) {
		sf::Sound sound;
		sound.setBuffer(soundBuffer);
		sound.play();
		while (sound.getStatus() == 2);
	}

public:
	static bool playSound(float frequency, int duration, int volume, int type = 0, bool blocking = false) {
		const double TWO_PI = 6.28318;
		const int sampleRate = 44100;
		const unsigned samples = sampleRate * duration * 0.001;
		const int amplitude = volume;

		sf::Int16* raw = new sf::Int16[samples];

		double t = 0;
		switch (type) {
		case 0:
			// Sine wave
			for (unsigned i = 0; i < samples; i++) {
				raw[i] = amplitude * sin(TWO_PI * frequency * i / sampleRate);
			}
			break;
		case 1:
			// Square wave
			for (unsigned i = 0; i < samples; i++) {
				raw[i] = amplitude * sgn(sin(TWO_PI * frequency * i / sampleRate));
			}
			break;
		}

		sf::SoundBuffer soundBuffer;
		if (!soundBuffer.loadFromSamples(raw, samples, 1, sampleRate)) return false;

		std::thread soundThread(threadPlay, soundBuffer);
		soundThread.detach();
		if (blocking) sf::sleep(sf::milliseconds(duration));

		return true;
	}
};