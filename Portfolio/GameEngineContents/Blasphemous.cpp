#include "Blasphemous.h"
#include "Title.h"
#include "MainMenu.h"
#include "PreCompile.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Stage03.h"
#include "Stage04.h"
#include "Stage05.h"

#pragma comment(lib, "GameEngineBase.lib")


Blasphemous::Blasphemous() 
{
}

Blasphemous::~Blasphemous() 
{
}

void Blasphemous::Start()
{
	LoadResources();

	CreateLevel<Title>("Title"); 
	CreateLevel<MainMenu>("MainMenu");

	//형제단 본부
	CreateLevel<Stage01>("Stage01");
	CreateLevel<Stage02>("Stage02");
	CreateLevel<Stage03>("Stage03");
	CreateLevel<Stage04>("Stage04");
	CreateLevel<Stage05>("Stage05");

	ChangeLevel("MainMenu");
}

void Blasphemous::Update(float _DeltaTime)
{
}

void Blasphemous::End()
{
}

void Blasphemous::LoadResources()
{
	//맵 텍스쳐
	{
		GameEngineDirectory Dir;

		Dir.MoveParentToExitsChildDirectory("GameEngineResources");
		Dir.Move("GameEngineResources");
		Dir.Move("Resources");
		Dir.Move("Map");
		Dir.Move("Brotherhood");
		Dir.Move("1-1");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}


	//UI 이미지
	{
		GameEngineDirectory Dir;

		Dir.MoveParentToExitsChildDirectory("GameEngineResources");
		Dir.Move("GameEngineResources");
		Dir.Move("Resources");
		Dir.Move("UI");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}


	{
		GameEngineDirectory Dir;

		Dir.MoveParentToExitsChildDirectory("GameEngineResources");
		Dir.Move("GameEngineResources");
		Dir.Move("Resources");
		Dir.Move("UI");
		Dir.Move("MainMenu");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}

	//UI Inventory
	{
		GameEngineDirectory Dir;

		Dir.MoveParentToExitsChildDirectory("GameEngineResources");
		Dir.Move("GameEngineResources");
		Dir.Move("Resources");
		Dir.Move("UI");
		Dir.Move("Inventory");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}


}
