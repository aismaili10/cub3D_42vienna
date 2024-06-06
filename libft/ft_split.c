/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:37:20 by aismaili          #+#    #+#             */
/*   Updated: 2023/09/19 11:38:07 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			j++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (j);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	begin;
	size_t	end;
	size_t	i;

	begin = 0;
	i = -1;
	result = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (result == NULL)
		return (NULL);
	while (++i < count_words(s, c))
	{
		while (s[begin] == c)
			begin++;
		end = begin;
		while (s[end] != c && s[end] != '\0')
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
	char	**res = ft_split("ABC abc 123", ' ');
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
