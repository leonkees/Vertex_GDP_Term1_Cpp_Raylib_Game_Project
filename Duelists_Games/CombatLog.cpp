#include <iostream>
#include "CombatLog.h"

std::vector<CombatMessage> CombatLog::MessageBuffer;

const int START_Y_POSITION = 410;
const int MESSAGE_SPACING = 25;
const int MAX_MESSAGES = 6;
const int FONT_SIZE = 18;

void CombatLog::AddMessage(const std::string& Message, Color TextColor, float Duration)
{
	MessageBuffer.push_back({ Message, TextColor, Duration });

	if (MessageBuffer.size() > MAX_MESSAGES)
	{
		MessageBuffer.erase(MessageBuffer.begin());
	}
}

void CombatLog::DrawMessages()
{

	int YPosition = START_Y_POSITION;

	for (size_t i = 0; i < MessageBuffer.size();)
	{
	
		CombatMessage& MessageAtIndex = MessageBuffer[i];
		DrawText(MessageAtIndex.Message.c_str(), 10, YPosition, FONT_SIZE, MessageAtIndex.TextColor);
		YPosition += MESSAGE_SPACING;

		MessageAtIndex.Timer += GetFrameTime();

		if (MessageAtIndex.Timer >= MessageAtIndex.Duration)
		{
		
			MessageBuffer.erase(MessageBuffer.begin() + i);
		
		}
		else
		{
			++i;
		}

	
	}

}

void CombatLog::ClearMessages()
{
	MessageBuffer.clear();
}

