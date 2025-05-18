#pragma once
#include <string>
#include <vector>
#include "raylib.h"


enum Action
{
	ATTACK,
	PARRY,
	DEFEND,
	NONE,
};

struct SpriteComponent
{
	std::vector<Texture2D> TextureArray;
	int CurrentSprite = 0;
	std::vector<int> framesArray;
};

class Character 
{

public:
	Character(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name);

	SpriteComponent SpriteC;
	int GetHealth() const { return Health; }
	std::string GetName() const { return Name; }
	bool GetIsAlive() const { return Health > 0; }
	int GetAtkPower() const { return AtkPower; }
	int GetStamina() const { return Stamina; }

	void AddTextureSprite(const char *TexturePath, int framesNumber);
	void SwapTextureSet(const std::vector<const char*>& NewTextures);
	Texture2D GetCurrentTexture();
	void UnloadAllTextures();
	void UpdateHealth(int Amount);
	void UpdateStamina(bool Increase);

	virtual Action ChooseAction() = 0;
	void InitStats();
	Action GetActionFromInput(int Input);

protected:
	std::string Name;
	int MaxHealth;
	int Health;
	int AtkPower;
	int Armor;
	int MaxStamina;
	int Stamina;

};