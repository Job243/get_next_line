/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmafueni <jmafueni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:09:11 by jmafueni          #+#    #+#             */
/*   Updated: 2024/03/05 16:35:27 by jmafueni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	if (read_to_stash(&stash, fd) != SUCCESS)
	{
		stash = NULL;
		return (NULL);
	}
	if (!stash)
		return (NULL);
	extract_line(stash, &line);
	clean_stash(&stash);
	if (!line || line[0] == '\0')
	{
		free(line);
		free_stash(&stash);
		stash = NULL;
		return (NULL);
	}
	return (line);
}

int	read_to_stash(t_list **stash, int fd)
{
	char	*buf;
	int		bytes_read;

	bytes_read = 1;
	while (!ft_found_newline(*stash) && bytes_read != 0)
	{
		buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return (free_stash(stash), ERR_MALLOC);
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			free_stash(stash);
			*stash = NULL;
			return (ERR_READ);
		}
		if (bytes_read == 0)
			return (free(buf), SUCCESS);
		buf[bytes_read] = '\0';
		if (add_to_stash(stash, buf, bytes_read) == ERR_MALLOC)
			return (ERR_MALLOC);
		free(buf);
	}
	return (SUCCESS);
}

int	add_to_stash(t_list **stash, char *buf, int bytes_read)
{
	int		i;
	t_list	*last;
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (ERR_MALLOC);
	new_node->next = NULL;
	new_node->content = (char *)malloc(sizeof(char) * (bytes_read + 1));
	if (!new_node->content)
		return (free_stash(&new_node), ERR_MALLOC);
	i = 0;
	while (buf[i] && i < bytes_read)
	{
		new_node->content[i] = buf[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (!*stash)
		return (*stash = new_node, SUCCESS);
	last = ft_lstlast(*stash);
	last->next = new_node;
	return (SUCCESS);
}

void	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (!stash)
		return ;
	generate_line(stash, &(*line));
	if (!line)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

void	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	clean_node = (t_list *)malloc(sizeof(t_list));
	if (!stash || !clean_node)
		return (free_stash(stash));
	clean_node->next = NULL;
	last = ft_lstlast(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content[i] && last->content[i] == '\n')
		i++;
	clean_node->content = (char *)malloc(sizeof(char)
			* ((ft_strlen(last->content) - i) + 1));
	if (!clean_node->content)
		return (free_stash(&clean_node));
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(stash);
	*stash = clean_node;
}
