#include "cub3D.h"

int	cleanup(t_main *main, int stage)
{
	exit(1);
	if (stage == 1)
		cleanup_1();
	if (stage == 2)
		cleanup_2();
	// ...
}
