#include "Character.h"
#include <iostream>
#include "CombatLog.h"



Character::Character(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name) : 
	MaxHealth(MaxHealth), AtkPower(AtkPower), Armor(Armor), MaxStamina(MaxStamina), Name(Name)
{
	InitStats();    // HERE IT WAS! if this did not set health to max then the enemies would also die instantly!
}

void Character::AddTextureSprite(const char *TexturePath, int framesNumber)
{

	SpriteC.TextureArray.push_back(LoadTexture(TexturePath));
	SpriteC.framesArray.push_back(framesNumber);					// ADD NUMBER OF FRAMES FOR DIFFERENT LENGTH ANIMATIONS
}

void Character::SwapTextureSet(const std::vector<const char*>& NewTextures)
{
	std::vector<Texture2D> TempTextureArray;
	for (const char* TexturePath : NewTextures)
	{
		TempTextureArray.push_back(LoadTexture(TexturePath));
	}

	UnloadAllTextures();
	SpriteC.TextureArray = std::move(TempTextureArray);
	SpriteC.CurrentSprite = 0;

}

Texture2D Character::GetCurrentTexture()
{
	if (!SpriteC.TextureArray.empty() && SpriteC.CurrentSprite < SpriteC.TextureArray.size())
	{
		return SpriteC.TextureArray[SpriteC.CurrentSprite];	
	}
	return {};
}

void Character::UnloadAllTextures()
{
	for (Texture2D& Texture : SpriteC.TextureArray)
	{
		if (Texture.id > 0)
		{
			UnloadTexture(Texture);
		}
	}

	SpriteC.TextureArray.clear();

}

void Character::UpdateHealth(int Amount)
{
	Health += Amount;
	if (Health <= 0) Health = 0;


	// this i should replace with the hearts logic:
	CombatLog::AddMessage((Name + " has " + std::to_string(Health) + " health remaining").c_str(), GREEN, 3.0f);

	// update hearts sprites:


}

void Character::UpdateStamina(bool Increase)
{
	Stamina = (Increase) ?  Stamina += 1: Stamina -= 1;
	if (Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}
	else if (Stamina <= 0)
	{
		Stamina = 0;
	}
}

void Character::InitStats() 
{
	Health = MaxHealth;   // LIFE PERSISTANT ACCROSS ROUNDS
	Stamina = 1;  // if this is a quickdraw one bullet is the start (unless a booster is applied?) could be persistant as well
}
Action Character::GetActionFromInput(int Input)
{
	switch (Input)
	{
	case 0: return ATTACK; 
		
	case 1:
		UpdateStamina(false); 
		return PARRY;

	case 2: 
		UpdateStamina(true); 
		return DEFEND;
		
	}
	return NONE;
}
