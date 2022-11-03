#pragma once

#include "Player.h"
#include "SteadyEnemy.h"
#include "InputHandler.h"
#include "SpritesHandler.h"

class FGame : public olc::PixelGameEngine
{
private:
	olc::Decal* BackGroundDecal = nullptr;
	olc::Decal* PlayerIdleDecal = nullptr;
	olc::Decal* PlayerRunDecal = nullptr;
	olc::Decal* PlayerShootRunDecal = nullptr;
	olc::Decal* PlayerJumpDecal = nullptr;
	olc::Decal* PlayerCrouchDecal = nullptr;
	olc::Decal* PlayerAimDownDecal = nullptr;
	olc::Decal* PlayerAimUpDecal = nullptr;
	olc::Decal* PlayerRunAimUpDecal = nullptr;
	olc::Decal* PlayerDyingDecal = nullptr;
	olc::Decal* PlayerDeadDecal = nullptr;
	olc::Decal* MedalDecal = nullptr;
	olc::Decal* BulletDecal = nullptr;
	olc::Decal* Bullet2Decal = nullptr;

	olc::Decal* EnemyIdleDecal = nullptr;
	olc::Decal* EnemyUpDecal = nullptr;
	olc::Decal* EnemyDownDecal = nullptr;

	olc::Decal* EngGameDecal = nullptr;

	std::wstring Level;
	int LevelWidth;
	int LevelHeight;

	float CameraX;
	float CameraY;

	int TileWidth;
	int TileHeight;
	int VisibleTilesX;
	int VisibleTilesY;
	float TileOffsetX;

	float OffsetX;

	float GroundCounter;
	float LevelStartOffsetX;

	int DecalOffset;
	float AnimationOffset;
	float AnimationCounter;
	float PlayerDeadTimer;

	FPlayer Player;
	FInputHandler Input = nullptr;

	std::vector<FBullet> PlayerBullets;
	std::vector<FBullet> EnemyBullets;
	std::vector<FSteadyEnemy> EnemyesArray;
	std::vector<olc::vi2d> AlreadySpawned;

	olc::Decal* SelectedDecal;

	FSpritesHandler SpriteHandler;

public:

	FGame();
	
	~FGame();

	virtual bool OnUserCreate() override;

	auto GetTile(const int TileX, const int TileY) const;

	void SetTile(const int TileX, const int TileY, const wchar_t Character);

	bool PlayerCollideObjects(const float X1, const float X2, const float Y1, const float Y2, const wchar_t Character) const;

	void GroundCollision(const float fElapsedTime);

	void ClampPlayer(const float InOffsetX);

	void PlayerShoot(const float InX, const float InY);

	void EnemyShoot(FSteadyEnemy& InEnemy, const float fElapsedTime);

	void PlayerBulletCollision(FBullet& InBullet);

	void EnemyBulletCollision(FBullet& InBullet);

	void MoveLevel();

	void DrawDebugMode();

	void SpawnEnemies();

	template <typename T>
	void RemoveElement(std::vector<T>& InVector, const std::function<void(FGameObject)>& Lambda);

	void RemoveEnemy();

	void RemoveBullets();

	void ResetAnimation(const float fElapsedTime);

	void AnimatePlayer(const float fElapsedTime);

	olc::Decal* SelectEnenmyDecal(const FGameObject InEnemy) const;

	void PrintPlayerLives();

	bool EndGame();

	void RunGame(const float fElapsedTime);

	bool OnUserUpdate(float fElapsedTime) override;
};