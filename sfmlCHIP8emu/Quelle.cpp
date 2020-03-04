//#include <SFML/Audio.hpp>
//#include <cmath>
//
//template <typename T> int sgn(T val) {
//	return (T(0) < val) - (val < T(0));
//}
//
//bool playSound(int frequency, int duration, int volume, int type = 0) {
//	const double TWO_PI = 6.28318;
//	const int sampleRate = 44100;
//	const double increment = 440. / sampleRate;
//	const unsigned samples = sampleRate * duration * 0.001;
//	const int amplitude = volume;
//
//	sf::Int16* raw = new sf::Int16[samples];
//
//	double x = 0;
//	switch (type) {
//	case 0:
//		// Sine wave
//		for (unsigned i = 0; i < samples; i++) {
//			raw[i] = amplitude * sin(x * TWO_PI * frequency);
//			x += increment;
//		}
//		break;
//	case 1:
//		// Square wave
//		for (unsigned i = 0; i < samples; i++) {
//			raw[i] = amplitude * sgn(sin(x * TWO_PI * frequency));
//			x += increment;
//		}
//		break;
//	}
//
//	sf::SoundBuffer soundBuffer;
//	if (!soundBuffer.loadFromSamples(raw, samples, 1, sampleRate)) return false;
//
//	sf::Sound sound;
//	sound.setBuffer(soundBuffer);
//	sound.play();
//	
//	while (sound.getStatus() == 2) {}
//
//	return true;
//}
//
//int main() {
//	if (!playSound(500, 100, 1000, 1)) return 1;
//
//	return 0;
//}