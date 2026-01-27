#pragma once
#include <string>

struct PlayerState {
	IVoiceCodec* codec = nullptr;
	int effect = 0;
	float param1 = 0.0f; // ex: frequence robot, délai écho
    float param2 = 0.0f; // ex: gain
	std::vector<int16_t> echoBuffer;
    size_t echoPos = 0;
	
};

struct EightbitState {
	int crushFactor = 350;
	float gainFactor = 1.2;
	bool broadcastPackets = false;
	int desampleRate = 2;
	uint16_t port = 4000;
	std::string ip = "127.0.0.1";
	PlayerState players[129]; // 0 à 128
};
