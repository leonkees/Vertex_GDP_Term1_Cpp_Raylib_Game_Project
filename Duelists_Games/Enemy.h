#pragma once
#include "Character.h"
#include <random>

class Enemy : public Character
{
public:
	Enemy(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name);

	Action ChooseAction() override;
	void IncreaseDifficulty(int RoundNumber);

private:

	std::mt19937 Generator;
	std::uniform_int_distribution<> Distribution;
};


/*
Public members of Character remain public in Player.
Protected members of Character remain protected in Player.
Private members of Character remain inaccessible in Player (though Player can access them indirectly via public or protected methods in Character).
*/