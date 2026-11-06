/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfaraji <sfaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:46:29 by sfaraji           #+#    #+#             */
/*   Updated: 2026/11/05 20:27:01 by sfaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*release(char *s1, char *s2)
{
	free(s1);
	free(s2);
	return (NULL);
}

static char	*fill_left(int fd, char *left)
{
	char	*buffer;
	ssize_t	bytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (release(NULL, left));
	bytes = 1;
	while (!ft_strchr(left, DELIM) && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (release(buffer, left));
		buffer[bytes] = '\0';
		left = ft_strjoin(left, buffer);
		if (!left)
			return (release(buffer, NULL));
	}
	free(buffer);
	return (left);
}

static char	*get_line(char *left)
{
	size_t	len;

	if (left[0] == '\0')
		return (NULL);
	len = 0;
	while (left[len] && left[len] != DELIM)
		len++;
	if (left[len] == DELIM)
		len++;
	return (ft_substr(left, 0, len));
}

static char	*trim_left(char *left)
{
	size_t	i;
	char	*rest;

	i = 0;
	while (left[i] && left[i] != DELIM)
		i++;
	if (left[i] != DELIM || left[i + 1] == '\0')
		return (release(left, NULL));
	rest = ft_substr(left, i + 1, ft_strlen(left));
	free(left);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*left[1024];
	char		*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	left[fd] = fill_left(fd, left[fd]);
	if (!left[fd])
		return (NULL);
	line = get_line(left[fd]);
	left[fd] = trim_left(left[fd]);
	return (line);
}
