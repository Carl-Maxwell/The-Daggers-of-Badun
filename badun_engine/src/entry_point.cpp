#include <vector>

#include <fstream>
#include <strstream>
#include <filesystem>

#include "olcPixelGameEngine.h"
#include "mtypedefs.h"
#include "mlog.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application.h"

i32 main() {
	Application app;
	auto success = app.Construct(256, 240, 4, 4);
	if (success)
		app.Start();
	else
		Log::red("pixel game engine did not initialize! rcode: {0}", success);

	return 0;
}

