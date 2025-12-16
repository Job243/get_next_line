/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmafueni <jmafueni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:09:11 by jmafueni          #+#    #+#             */
/*   Updated: 2024/10/28 20:08:45 by jmafueni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	line = NULL;
	read_to_stash(&stash, fd);
	if (!stash)
		return (NULL);
	if (extract_line(stash, &line) == ERR_MALLOC)
		return (NULL);
	clean_stash(&stash);
	if (line[0] == '\0')
	{
		free_stash(&stash);
		stash = NULL;
		free(line);
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
			return (SUCCESS);
		bytes_read = (int)read(fd, buf, BUFFER_SIZE);
		if ((*stash == NULL && bytes_read == 0) || bytes_read == -1)
		{
			free(buf);
			return (SUCCESS);
		}
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

int	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (!stash)
		return (ERR_MALLOC);
	if (generate_line(stash, line) == ERR_MALLOC)
		return (ERR_MALLOC);
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
	return (SUCCESS);
}

void	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	clean_node = (t_list *)malloc(sizeof(t_list));
	if (!stash || !clean_node)
		return ;
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
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(stash);
	*stash = clean_node;
}
