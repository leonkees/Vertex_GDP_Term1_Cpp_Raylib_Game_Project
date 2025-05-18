#pragma once
#include "Character.h"

class Player : public Character 
{
public:
	Player(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name);
	Action ChooseAction() override;

};


/*
Public members of Character remain public in Player.
Protected members of Character remain protected in Player.
Private members of Character remain inaccessible in Player (though Player can access them indirectly via public or protected methods in Character).
*/