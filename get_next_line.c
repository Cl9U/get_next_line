/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfaraji <sfaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:46:29 by sfaraji           #+#    #+#             */
/*   Updated: 2026/11/05 15:42:17 by sfaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_all(char *s1, char *s2)
{
	free(s1);
	free(s2);
	return (NULL);
}

static char	*read_to_leftover(int fd, char *leftover)
{
	char	*buffer;
	ssize_t	bytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_all(NULL, leftover));
	bytes = 1;
	while (!ft_strchr(leftover, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free_all(buffer, leftover));
		buffer[bytes] = '\0';
		leftover = ft_strjoin(leftover, buffer);
		if (!leftover)
			return (free_all(buffer, NULL));
	}
	free(buffer);
	return (leftover);
}

static char	*extract_line(char *leftover)
{
	size_t	len;

	if (leftover[0] == '\0')
		return (NULL);
	len = 0;
	while (leftover[len] && leftover[len] != '\n')
		len++;
	if (leftover[len] == '\n')
		len++;
	return (ft_substr(leftover, 0, len));
}

static char	*trim_leftover(char *leftover)
{
	size_t	i;
	char	*rest;

	i = 0;
	while (leftover[i] && leftover[i] != '\n')
		i++;
	if (leftover[i] != '\n' || leftover[i + 1] == '\0')
		return (free_all(leftover, NULL));
	rest = ft_substr(leftover, i + 1, ft_strlen(leftover));
	free(leftover);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	leftover = read_to_leftover(fd, leftover);
	if (!leftover)
		return (NULL);
	line = extract_line(leftover);
	leftover = trim_leftover(leftover);
	return (line);
}
