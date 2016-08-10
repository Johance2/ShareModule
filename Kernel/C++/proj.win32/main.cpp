#include "..\Classes\Kernel.h"

int main(int argc, char **argvs)
{
	if(argc > 1)
	{
		Kernel kernel(argvs[1]);
		kernel.Run();
	}
	return 0;
}