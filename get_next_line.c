/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfaraji <sfaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:46:29 by sfaraji           #+#    #+#             */
/*   Updated: 2026/11/05 14:44:33 by sfaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*grow(char *leftover, size_t len, size_t *cap)
{
	char	*new;
	size_t	ncap;

	if (*cap >= len + BUFFER_SIZE + 1)
		return (leftover);
	ncap = *cap * 2;
	if (ncap < len + BUFFER_SIZE + 1)
		ncap = len + BUFFER_SIZE + 1;
	new = malloc(ncap);
	if (!new)
		return (free_all(leftover, NULL));
	ft_memcpy(new, leftover, len);
	new[len] = '\0';
	free(leftover);
	*cap = ncap;
	return (new);
}

static char	*read_to_leftover(int fd, char *leftover)
{
	size_t	len;
	size_t	cap;
	ssize_t	bytes;
	int		nl;

	nl = (ft_strchr(leftover, '\n') != NULL);
	len = ft_strlen(leftover);
	cap = len + 1;
	bytes = 1;
	while (bytes > 0 && !nl)
	{
		leftover = grow(leftover, len, &cap);
		if (!leftover)
			return (NULL);
		bytes = read(fd, leftover + len, BUFFER_SIZE);
		if (bytes == -1)
			return (free_all(leftover, NULL));
		leftover[len + bytes] = '\0';
		nl = (ft_strchr(leftover + len, '\n') != NULL);
		len += bytes;
	}
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
