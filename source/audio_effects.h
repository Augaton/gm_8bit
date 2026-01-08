#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>

namespace AudioEffects {
	enum {
		EFF_NONE,
		EFF_BITCRUSH,
		EFF_DESAMPLE,
		EFF_ROBOT,
        EFF_DEMON,
		EFF_INTERCOM 
	};

	void BitCrush(uint16_t* sampleBuffer, int samples, float quant, float gainFactor) {
		for (int i = 0; i < samples; i++) {
			//Signed shorts range from -32768 to 32767
			//Let's quantize that a bit
			float f = (float)sampleBuffer[i];
			f /= quant;
			sampleBuffer[i] = (uint16_t)f;
			sampleBuffer[i] *= quant;
			sampleBuffer[i] *= gainFactor;
		}
	}

	static uint16_t tempBuf[10 * 1024];
	void Desample(uint16_t* inBuffer, int& samples, int desampleRate = 2) {
		assert(samples / desampleRate + 1 <= sizeof(tempBuf));
		int outIdx = 0;
		for (int i = 0; i < samples; i++) {
			if (i % desampleRate == 0) continue;

			tempBuf[outIdx] = inBuffer[i];
			outIdx++;
		}
		std::memcpy(inBuffer, tempBuf, outIdx * 2);
		samples = outIdx;
	}

	//

	// Utilisation d'une table de sinus pré-calculée pour la performance
    static float sinTable[240]; 
    static bool tableInit = false;

    void Robotize(int16_t* buffer, int samples, float freq = 60.0f) {
        if (!tableInit) {
            for(int i=0; i<240; i++) sinTable[i] = sinf(2.0f * 3.14159f * i / 240.0f);
            tableInit = true;
        }
        for (int i = 0; i < samples; i++) {
            // Modulation en anneau : Voix * Sinus
            float modulator = sinTable[i % 240]; 
            buffer[i] = (int16_t)(buffer[i] * modulator);
        }
    }

    void Demon(int16_t* buffer, int samples, float pitch = 0.6f) {
        // Pour un effet démoniaque simple, on combine BitCrush et un gain bas
        for (int i = 0; i < samples; i++) {
            float f = (float)buffer[i];
            f = f * pitch; // Baisse le gain pour simuler une voix profonde
            buffer[i] = (int16_t)(f * 1.5f); // Compensation de gain
        }
    }

	// Effet Intercom / Radio
    void Intercom(int16_t* buffer, int samples) {
        static float lastSample = 0;
        for (int i = 0; i < samples; i++) {
            float s = (float)buffer[i];

            // 1. Simulation Passe-Haut simple (enlève les basses étouffées)
            float highPassed = s - lastSample;
            lastSample = s;

            // 2. Saturation (Overdrive de radio)
            if (highPassed > 15000) highPassed = 15000;
            if (highPassed < -15000) highPassed = -15000;

            // 3. Réduction de fidélité (simule un petit haut-parleur)
            buffer[i] = (int16_t)(highPassed * 0.8f);
        }
    }
}