#include "Enemy.h"


// Passes all arguments to Character Constructor
Enemy::Enemy(int MaxHealth, int AtkPower, int Armor, int MaxStamina, std::string Name) : Character(MaxHealth, AtkPower, Armor, MaxStamina, Name), Generator(std::random_device{}()), Distribution(0, 2)
{

}

Action Enemy::ChooseAction()
{
	int Input = Distribution(Generator);
	if (Stamina == 0)
	{
		std::uniform_int_distribution<> LimitedDistribution(0, 2);
		Input = LimitedDistribution(Generator) == 1 ? 0 : 2; // Implement Enemy being exhausted / Dazed  (NOT RELEVANT)
	}
	SpriteC.CurrentSprite = Input + 1;
	return GetActionFromInput(Input);
}

void Enemy::IncreaseDifficulty(int RoundNumber)
{
	static std::vector<const char*> OgreTextures =
	{
		"../SourceArt/idle-sheet.png",
		"../SourceArt/shoot-sheet.png",
		"../SourceArt/reload-sheet.png",
		"../SourceArt/dead-sheet.png"
	};

	/*static std::vector<const char*> OgreTextures =
	{
		"../SourceArt/Characters/Ogre/Ogre_IdleBlinking_Sprite.png",
		"../SourceArt/Characters/Ogre/Ogre_Attacking_Sprite.png",
		"../SourceArt/Characters/Ogre/Ogre_Parry_Sprite.png",
		"../SourceArt/Characters/Ogre/Ogre_Defend_Sprite.png"
	};*/

	static std::vector<const char*> SkeletonTextures =
	{
		"../SourceArt/Characters/Skeleton/Skeleton_Idle_Sprite.png",
		"../SourceArt/Characters/Skeleton/Skeleton_Attacking_Sprite.png",
		"../SourceArt/Characters/Skeleton/Skeleton_Parry_Sprite.png",
		"../SourceArt/Characters/Skeleton/Skeleton_Defend_Sprite.png"
	};

	static std::vector<const char*> CyclopTextures =
	{
		"../SourceArt/Characters/Cyclop/Cyclop_IdleBlinking_Sprite.png",
		"../SourceArt/Characters/Cyclop/Cyclop_Attacking_Sprite.png",
		"../SourceArt/Characters/Cyclop/Cyclop_Parry_Sprite.png",
		"../SourceArt/Characters/Cyclop/Cyclop_Defend_Sprite.png"
	};


	switch (RoundNumber)
	{
	case 1:
		return;
	case 2:
		Health += RoundNumber + 1;
		AtkPower += 1;
		Name = "Ogre";
		SwapTextureSet(OgreTextures);
		return;
	case 3:
		Health += RoundNumber + 1;
		AtkPower += 2;
		Armor += RoundNumber / 2;
		Name = "Skeleton";
		SwapTextureSet(SkeletonTextures);
		return;
	case 4:
		Health += RoundNumber + 3;
		AtkPower += 3;
		Armor += RoundNumber / 2;
		Name = "Cyclop";
		SwapTextureSet(CyclopTextures);
		return;
	}

}
