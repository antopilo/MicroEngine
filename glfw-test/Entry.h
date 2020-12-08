
#include "src/Application.h"

int main(void)
{
	Engine::Application application = Engine::Application();
	application.Init();
	application.Run();
	application.Close();
}