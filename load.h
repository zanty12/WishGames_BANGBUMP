#pragma once
#include <thread>

#include "graphical.h"
#include "video.h"

class Load
{
private:
	static Video* video_;
	static int loading_tex_;
public:
	Load()
	{
		loading_tex_ = LoadTexture("data/texture/UI/loading.png");
	}
	~Load()
	{
		delete video_;
	}

	static void LoadStart()
	{
		NewVideo();
	}

	static void LoadStop()
	{

	}

	static void Update(void)
	{
		Time::Update();
		video_->Update();
	}

	static void Draw()
	{
		Graphical::Clear(Color::White);
		video_->DrawAsResource();
		Graphical::Present();
	}
private:
	static void NewVideo()
	{
		if (video_) delete video_;
		video_ = new Video("data/video/LoadScreen.mp4");
		video_->SetLoop(true);
		video_->SetWindowPos(Vector2(Graphical::GetWidth() /2 , Graphical::GetHeight() / 2));
		video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
	}
};

class LoadClientSide {
public:
	static bool isNowLoad;
	static std::thread *thread;
	static void BeginLoad(void);
	static void EndLoad(void);

private:
	static void loading(void);
};

