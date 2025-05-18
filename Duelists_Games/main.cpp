#include <iostream>
#include "raylib.h"
#include "CombatLog.h"
#include "Player.h"
#include "Enemy.h"
#include <map>
#include <functional>
#include <vector>
#include <string>

float SHORT_MESSAGE_DURATION = 3.0f;
float LONG_MESSAGE_DURATION = 6.0f;
bool CAN_INPUT = true;
float INPUT_COOLDOWN = 2.9f;
float TIME_SINCE_LAST_INPUT = 0.0f;


enum GameState {WAITING_FOR_INPUT, PROCESSING, GAME_OVER};
void DrawOutcome(Player& MainPlayer, Enemy& MainEnemy, Action PlayerAction, GameState& State, int& RoundNumber);
void ProcessOutcome(Player& Mainplayer, Enemy& MainEnemy, Action PlayerAction);
void DrawWaitForInput(Action& PlayerAction, GameState& State, Player& MainPlayer);


bool backgroundShift = false; // to toggle when dodging one side to the other
Vector2 EnemyPosition = { 480.0f - 100 * backgroundShift, 480.0f }; // taking it out so it is available for other functions (there is another way to do this im sure)



struct CombatOutcome 
{
	std::string OutcomeText;
	Color TextColor;
	std::function<void(Player&, Enemy&)> GameplayResult = [](Player&, Enemy&) {};
};

int main(void)
{
	// Raylib Init
	const int screenWidth = 1300;
	const int screenHeight = 1024;
	InitWindow(screenWidth, screenHeight, "Dueling Game");


	// Game Init
	Texture2D Background = LoadTexture("../SourceArt/clean-west-background.png");
	Texture2D Heart = LoadTexture("../SourceArt/heart.png");
	static std::vector<const char*> bulletSprites =
	{
		"../SourceArt/0bullet.png",
		"../SourceArt/1bullet.png",
		"../SourceArt/2bullet.png",
		"../SourceArt/3bullet.png",
		"../SourceArt/4bullet.png",
		"../SourceArt/5bullet.png",
		"../SourceArt/6bullet.png"
	};

	int FramesCounter = 0;
	int FramesSpeed = 4;
	int CurrentFramePlayer = 0;
	int CurrentFrameEnemy = 0;
	SetTargetFPS(60);              

	// Player Init
	Player MainPlayer(3, 2, 2, 6, "Hero");  // change max stamina to 6 so its max bullets in gun barrel
	// MainPlayer.UpdateHealth(5);    //  HERE IT DOES NOT WORK? GETS OVERWRITTEN TO 0 ...

	Vector2 PlayerPosition = { 755.0f, 430.0f };
	
	MainPlayer.AddTextureSprite("../SourceArt/playeridle.png",2);
	MainPlayer.AddTextureSprite("../SourceArt/playershoot.png",5);
	MainPlayer.AddTextureSprite("../SourceArt/playerreload.png",4);
	MainPlayer.AddTextureSprite("../SourceArt/playeridle.png",2);

	Rectangle PlayerRect = { 0.0f, 0.0f, static_cast<float>(MainPlayer.SpriteC.TextureArray[MainPlayer.SpriteC.CurrentSprite].width / MainPlayer.SpriteC.framesArray[MainPlayer.SpriteC.CurrentSprite]), static_cast<float>(MainPlayer.SpriteC.TextureArray[MainPlayer.SpriteC.CurrentSprite].height / 1) };



	// Enemy Init
	Enemy MainEnemy(1, 1, 0, 1, "Goblin");
	//Vector2 EnemyPosition = { 480.0f - 100 * backgroundShift, 480.0f };

	/*MainEnemy.AddTextureSprite("../SourceArt/Characters/Goblin/Goblin_IdleBlinking_Sprite.png");
	MainEnemy.AddTextureSprite("../SourceArt/Characters/Goblin/Goblin_Attacking_Sprite.png");
	MainEnemy.AddTextureSprite("../SourceArt/Characters/Goblin/Goblin_Parry_Sprite.png");
	MainEnemy.AddTextureSprite("../SourceArt/Characters/Goblin/Goblin_Defend_Sprite.png");
	*/

	MainEnemy.AddTextureSprite("../SourceArt/idle-sheet.png",2);
	MainEnemy.AddTextureSprite("../SourceArt/shoot-sheet.png",8);
	MainEnemy.AddTextureSprite("../SourceArt/reload-sheet.png",3);
	MainEnemy.AddTextureSprite("../SourceArt/dead-sheet.png",6);


	Rectangle EnemyRect = { 0.0f, 0.0f, static_cast<float>(MainEnemy.SpriteC.TextureArray[MainEnemy.SpriteC.CurrentSprite].width / MainEnemy.SpriteC.framesArray[MainEnemy.SpriteC.CurrentSprite]), static_cast<float>(MainEnemy.SpriteC.TextureArray[MainEnemy.SpriteC.CurrentSprite].height / 1)};
	//EnemyRect.width = -EnemyRect.width; //Flip image
	
	int RoundNumber = 1;
	bool GameOver = false;

	GameState State = WAITING_FOR_INPUT;
	Action PlayerAction = NONE;

	//MainPlayer.UpdateHealth(5);  // HERE IT WORKS! but at player Init it goes back to 0 !? only if both are kept? wtf
	//MainPlayer.UpdateHealth(5);

	while (!WindowShouldClose())
	{

		TIME_SINCE_LAST_INPUT += GetFrameTime();

		// Player Sprite Test + ENEMY ACTUALLY
		FramesCounter++;
		if (FramesCounter >= (60 / FramesSpeed))
		{

			FramesCounter = 0;

			CurrentFramePlayer++;

			if (CurrentFramePlayer > MainPlayer.SpriteC.framesArray[MainPlayer.SpriteC.CurrentSprite])
			{
				CurrentFramePlayer = 0;
			}

			PlayerRect.x = static_cast<float>(CurrentFramePlayer) * MainPlayer.GetCurrentTexture().width / MainPlayer.SpriteC.framesArray[MainPlayer.SpriteC.CurrentSprite];
			PlayerRect.y = static_cast<float>(CurrentFramePlayer) * MainPlayer.GetCurrentTexture().height / 1;


			CurrentFrameEnemy++;

			if (CurrentFrameEnemy > MainEnemy.SpriteC.framesArray[MainEnemy.SpriteC.CurrentSprite])
			{
				CurrentFrameEnemy = 0;
			}

			EnemyRect.x = static_cast<float>(CurrentFrameEnemy) * MainEnemy.GetCurrentTexture().width / MainEnemy.SpriteC.framesArray[MainEnemy.SpriteC.CurrentSprite];
			EnemyRect.y = static_cast<float>(CurrentFrameEnemy) * MainEnemy.GetCurrentTexture().height / 1;

		}
		


		BeginDrawing(); // Begin frame rendering
		ClearBackground(BLACK);
		DrawTexture(Background, -100*backgroundShift, 0, WHITE);


		// draw hearts according to player health  ((( SHOULD GO IN ANOTHER FILE METHODS FOR UI )))
		//int startxposition = 0;
		for (int i = 1; i <= MainPlayer.GetHealth(); i++)
		{
			Vector2 heartPosition = {500+100*i - 30 * MainPlayer.GetHealth(), 900 };
			DrawTextureEx(Heart, heartPosition , 0.0f, 2.0f, WHITE);
		}

		// draw bullets according to player stamina  ((( SHOULD GO IN ANOTHER FILE METHODS FOR UI )))
	

		DrawTextureEx(LoadTexture(bulletSprites[MainPlayer.GetStamina()]), {-100,600}, 0.0f, 2.0f, WHITE);
		


		// Player Sprite Test
		//DrawTextureRec(MainPlayer.GetCurrentTexture(), PlayerRect, PlayerPosition, WHITE);
		DrawTexturePro(MainPlayer.GetCurrentTexture(), PlayerRect, {PlayerPosition.x,PlayerPosition.y,600,600}, {0,0}, 0.0f, WHITE);
		DrawTextureRec(MainEnemy.GetCurrentTexture(), EnemyRect, EnemyPosition, WHITE);

		CombatLog::DrawMessages();

		if (State == GAME_OVER)
		{
			DrawText("Game Over. Press ESC to exit.", 200, 200, 20, RED);
			EndDrawing(); // Finalize frame rendering
			continue;
		}

		DrawText(("Round " + std::to_string(RoundNumber)).c_str(), 10, 10, 20, LIGHTGRAY);

		if (State == WAITING_FOR_INPUT)
		{
			DrawWaitForInput(PlayerAction, State, MainPlayer);
		}
		else if (State == PROCESSING)
		{
			DrawOutcome(MainPlayer, MainEnemy, PlayerAction, State, RoundNumber);
		}

		// Reset animations to Idle
		if (CAN_INPUT)
		{
			MainPlayer.SpriteC.CurrentSprite = 0;
			MainEnemy.SpriteC.CurrentSprite = 0;
		}

		EndDrawing();
	}

	//end textures
	UnloadTexture(Heart);
	UnloadTexture(Background);
	MainPlayer.UnloadAllTextures();
	MainEnemy.UnloadAllTextures();
	CloseWindow();
	return 0;
}


void DrawWaitForInput(Action& PlayerAction, GameState& State, Player& MainPlayer)
{

	
	if (State != WAITING_FOR_INPUT) return;

	DrawText("Choose an Action - (1: SHOOT, 2: RELOAD, 3: DODGE)", 10, 30, 20, YELLOW);


	if(CAN_INPUT)
	{
		switch (GetKeyPressed())
		{

		case KEY_ONE:
			if (MainPlayer.GetStamina() > 0)
			{
			PlayerAction = ATTACK;	// NOW ITS SHOOT, ONLY IF BULLETS ARE AVAILABLE
			State = PROCESSING;
			CAN_INPUT = false;
			TIME_SINCE_LAST_INPUT = 0.0f;
			MainPlayer.SpriteC.CurrentSprite = 1;
			MainPlayer.UpdateStamina(false);	// A BULLET IS USED
			break;
			}
			else
			{
				CombatLog::AddMessage("You are exhausted: You cannot Shoot until you Reload!", RED, SHORT_MESSAGE_DURATION);
				CAN_INPUT = false;
				TIME_SINCE_LAST_INPUT = 0.0f;
				break;
			}
		case KEY_TWO:
			/*if (MainPlayer.GetStamina() > 0)
			{*/
				MainPlayer.UpdateStamina(true);  // changed to true
				PlayerAction = PARRY;			// NOW ITS RELOAD, WILL REGAIN BULLET (STAMINA)
				State = PROCESSING;
				CAN_INPUT = false;
				TIME_SINCE_LAST_INPUT = 0.0f;
				MainPlayer.SpriteC.CurrentSprite = 2;

				break;
			/*}
			else
			{
				CombatLog::AddMessage("You are exhausted: You cannot Parry until you Defend!", RED, SHORT_MESSAGE_DURATION);
				CAN_INPUT = false;
				TIME_SINCE_LAST_INPUT = 0.0f;
				break;
			}*/

		case KEY_THREE:
			PlayerAction = DEFEND;	    // NOW ITS DODGE, WILL AVOID ALL DAMAGE 
			State = PROCESSING;
			//MainPlayer.UpdateStamina(true);
			CAN_INPUT = false;
			TIME_SINCE_LAST_INPUT = 0.0f;
			MainPlayer.SpriteC.CurrentSprite = 3;
			backgroundShift = !backgroundShift;					// SHIFT BACKGROUND SINCE PLAYER MOVES TO THE SIDE
			EnemyPosition = { 480.0f - 100 * backgroundShift, 480.0f };		// SHIFT ENEMY TO KEEP ALLIGNMENT WITH BACKGROUND
			break;

		default:
			break;

		}

	}

	if (TIME_SINCE_LAST_INPUT > INPUT_COOLDOWN)
	{
		CAN_INPUT = true;
	}

}


void DrawOutcome(Player& MainPlayer, Enemy& MainEnemy, Action PlayerAction, GameState& State, int& RoundNumber)
{

	ProcessOutcome(MainPlayer, MainEnemy, PlayerAction);
	State = WAITING_FOR_INPUT;

	if (!MainEnemy.GetIsAlive())
	{

		RoundNumber++;

		if (RoundNumber > 5)
		{
			CombatLog::AddMessage("Our Hero has defeated all his opponents and is now champion of the Wild West !!!", GREEN, SHORT_MESSAGE_DURATION);
			State = GAME_OVER;
		}

		else
		{
			CombatLog::AddMessage((MainEnemy.GetName() + " has been defeated by our Hero! A new stronger enemy appears").c_str(), LIGHTGRAY, SHORT_MESSAGE_DURATION);
			MainEnemy.IncreaseDifficulty(RoundNumber);
			CombatLog::AddMessage((MainEnemy.GetName() + " enters the fray!").c_str(), YELLOW, SHORT_MESSAGE_DURATION);
			//MainPlayer.InitStats();  DO NOT HEAL BETWEEN ROUNDS
			CombatLog::AddMessage("Our Hero Recovers his stamina and energy ready for a new round!", LIGHTGRAY, SHORT_MESSAGE_DURATION);
		}

	}

	if (!MainPlayer.GetIsAlive())
	{
		CombatLog::AddMessage("Our Hero has perished! You must try again!", RED, 15.5f);
		State = GAME_OVER;
	}

}


void ProcessOutcome(Player& MainPlayer, Enemy& MainEnemy, Action PlayerAction)
{
	// Process the round logic based on actions
	Action EnemyAction = MainEnemy.ChooseAction();

	// Lambda for mapping Action to String
	auto GetActionString = [](Action action) -> std::string
	{
		switch (action)
		{
			case ATTACK: return "Attack";
			case DEFEND: return "Defend";
			case PARRY: return "Parry";     // RELOAD
			default: return "";
		}
	};

	std::string PlayerActionStr = GetActionString(PlayerAction);
	std::string EnemyActionStr = GetActionString(EnemyAction);

	// Display player and enemy actions
	CombatLog::AddMessage(("Player " + PlayerActionStr + "s").c_str(), LIGHTGRAY, SHORT_MESSAGE_DURATION);
	CombatLog::AddMessage(("Enemy " + EnemyActionStr + "s").c_str(), LIGHTGRAY, SHORT_MESSAGE_DURATION);

	static std::map<std::pair<Action, Action>, CombatOutcome> OutcomeMap = 
	{
	{{ATTACK, ATTACK}, {"It's a SHOOTOUT! Both get SHOT!", DARKGRAY,	[](Player& player, Enemy& enemy) {enemy.UpdateHealth(-(1));player.UpdateHealth(-(1));}}},
	{{ATTACK, PARRY}, {"Enemy RELOADS and gets SHOT!", RED,[](Player& player, Enemy& enemy) {enemy.UpdateHealth(-(1)); }}},
	{{ATTACK, DEFEND}, {"Enemy DODGES! The shot MISSES!", GREEN,	[](Player& player, Enemy& enemy) {/*enemy.UpdateHealth(-(player.GetAtkPower() * 2));*/ }}},
	{{PARRY, ATTACK}, {"You RELOAD and get SHOT!", RED,[](Player& player, Enemy& enemy) {player.UpdateHealth(-(1)); }}},
	{{PARRY, PARRY}, {"Both RELOAD!", DARKGRAY,	[](Player& player, Enemy& enemy) {}}},	
	{{PARRY, DEFEND}, {"You RELOAD!", LIGHTGRAY,	[](Player& player, Enemy& enemy) {}}},
	{{DEFEND, ATTACK}, {"Enemy SHOOTS and MISSES!", BLUE,[](Player& player, Enemy& enemy) {	/*player.UpdateHealth(-(enemy.GetAtkPower() / 2));*/}}},
	{{DEFEND, PARRY}, {"Enemy RELOADS!", LIGHTGRAY,	[](Player& player, Enemy& enemy) {		}}},
	{{DEFEND, DEFEND}, {"Both DODGE in vain!", DARKGRAY,	[](Player& player, Enemy& enemy) {}}}
	};

	CombatOutcome Outcome = OutcomeMap[{PlayerAction, EnemyAction}];

	// Display the outcome text
	CombatLog::AddMessage(Outcome.OutcomeText.c_str(), Outcome.TextColor, SHORT_MESSAGE_DURATION);
	Outcome.GameplayResult(MainPlayer, MainEnemy);

}