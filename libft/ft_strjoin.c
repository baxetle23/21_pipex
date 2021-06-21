#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*new_string;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_string = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_string)
		return (NULL);
	while (s1[i] != '\0')
	{
		new_string[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		new_string[i] = s2[j];
		i++;
		j++;
	}
	//free(s1);
	new_string[i] = '\0';
	return (new_string);
}