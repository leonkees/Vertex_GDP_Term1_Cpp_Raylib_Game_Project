#pragma once
#include <vector>
#include <string>
#include "raylib.h"

//struct
struct CombatMessage 
{
	std::string Message;
	Color TextColor;
	float Duration;
	float Timer = 0; 

};


class CombatLog 
{

public:

	static void AddMessage(const std::string& Message, Color TextColor, float Duration);
	static void DrawMessages();
	static void ClearMessages();

private:
	static std::vector<CombatMessage> MessageBuffer;
};

