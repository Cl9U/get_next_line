/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfaraji <sfaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:46:33 by sfaraji           #+#    #+#             */
/*   Updated: 2026/11/05 15:42:44 by sfaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	nlen;
	size_t	s1len;
	size_t	s2len;
	char	*str;

	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	nlen = s1len + s2len;
	str = malloc(nlen + 1);
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(str, s1, s1len);
	ft_memcpy(str + s1len, s2, s2len + 1);
	free(s1);
	return (str);
}

char	*ft_substr(char const *s, size_t start, size_t len)
{
	size_t	slen;
	char	*str;
	char	*empty;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
	{
		empty = malloc(1);
		if (!empty)
			return (NULL);
		empty[0] = '\0';
		return (empty);
	}
	if (len > slen - start)
		len = slen - start;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s + start, len);
	str[len] = '\0';
	return (str);
}
