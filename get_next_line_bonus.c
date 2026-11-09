/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfaraji <sfaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:46:29 by sfaraji           #+#    #+#             */
/*   Updated: 2026/11/09 10:47:16 by sfaraji          ###   ########.fr       */
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

	buffer = malloc((size_t)BUFFER_SIZE + 1);
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

static t_node	*get_node(t_node **head, int fd, int drop)
{
	t_node	**cur;
	t_node	*tmp;

	cur = head;
	while (*cur && (*cur)->fd != fd)
		cur = &(*cur)->next;
	if (drop)
	{
		if (!*cur)
			return (NULL);
		tmp = (*cur);
		*cur = (*cur)->next;
		free(tmp);
		return (NULL);
	}
	if (!*cur)
	{
		*cur = malloc(sizeof(t_node));
		if (!*cur)
			return (NULL);
		(*cur)->fd = fd;
		(*cur)->left = NULL;
		(*cur)->next = NULL;
	}
	return (*cur);
}

static char	*take_line(char **left)
{
	size_t	len;
	char	*line;
	char	*rest;

	len = 0;
	while ((*left)[len] && (*left)[len] != DELIM)
		len++;
	if ((*left)[len] == DELIM)
		len++;
	line = NULL;
	rest = NULL;
	if (len > 0)
		line = ft_substr(*left, 0, len);
	if (line && (*left)[len])
		rest = ft_substr(*left, len, ft_strlen(*left));
	free(*left);
	*left = rest;
	return (line);
}

char	*get_next_line(int fd)
{
	static t_node	*head;
	t_node			*node;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = get_node(&head, fd, 0);
	if (!node)
		return (NULL);
	node->left = fill_left(fd, node->left);
	if (!node->left)
	{
		get_node(&head, fd, 1);
		return (NULL);
	}
	line = take_line(&node->left);
	if (!node->left)
		get_node(&head, fd, 1);
	return (line);
}
