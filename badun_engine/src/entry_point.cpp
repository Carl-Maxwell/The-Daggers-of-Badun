#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "mtypedefs.h"
#include "mlog.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "The Daggers of Badun!";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Log::log("time1: {0}", fElapsedTime);
		

		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
		return true;
	}
};

i32 main() {
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}

