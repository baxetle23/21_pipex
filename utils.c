#include "pipex.h"

void	free_memory(char **split1, char **split2)
{
	int	i;

	i = 0;
	while (split1[i])
		free(split1[i++]);
	i = 0;
	while (split2[i])
		free(split2[i++]);
	free(split2);
	free(split1);
}
