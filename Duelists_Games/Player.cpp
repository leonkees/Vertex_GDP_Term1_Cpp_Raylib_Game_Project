#include "Player.h"
#include <iostream>
#include "raylib.h"

// Passes all arguments to Character Constructor
Player::Player(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name) : Character(MaxHealth, AtkPower, Armor, MaxStamina, Name)
{

}

Action Player::ChooseAction()
{
	return GetActionFromInput(3);
}
