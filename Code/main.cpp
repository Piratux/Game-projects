#define OLC_PGE_APPLICATION

#include "GameManager.h"

int main()
{
	try
	{
		RPG rpg;
		if (rpg.Construct(480, 270, 3, 3, false, true, true))
			rpg.Start();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (...)
	{
		std::cout << "Unknown exception occured\n";
	}
}