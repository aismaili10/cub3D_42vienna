#include "libft.h"

static void	*freeing(char **result, size_t x)
{
	size_t	i;

	i = 0;
	while (i < x)
	{
		free(result[i]);
		i++;
	}
	free (result);
	return (NULL);
}

static size_t	count_words(char const *s, char *deli)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (ft_strchr(deli, s[i]))
			i++;
		if (s[i] != '\0')
			j++;
		while (!ft_strchr(deli, s[i]) && s[i] != '\0')
			i++;
	}
	return (j);
}

char	**ft_split_md(char const *s, char *deli)
{
	char	**result;
	size_t	begin;
	size_t	end;
	size_t	i;

	begin = 0;
	i = -1;
	if (!s || !deli)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (count_words(s, deli) + 1));
	if (!result)
		return (NULL);
	while (++i < count_words(s, deli))
	{
		while (ft_strchr(deli, s[begin++]))
		end = begin;
		while (!ft_strchr(deli, s[begin]) && s[end] != '\0')
			end++;
		result[i] = ft_substr(s, begin, (end - begin));
		if (!result[i])
			return (freeing(result, i));
		begin = end;
	}
	result[i] = 0;
	return (result);
}
/*
#include <stdio.h>

int	main(void)
{
	int	i = 0;
	char	s[] = "hello!  WOrld";
	char	c = ' ';
	char	**res = ft_split("^^^1^^2a,^^^^3^^^^--h^^^^", '^');
	while (res[i])
	{
		printf("%s\n", res[i]);
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}
*/
