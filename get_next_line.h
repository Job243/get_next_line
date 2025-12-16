/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmafueni <jmafueni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:35:58 by jmafueni          #+#    #+#             */
/*   Updated: 2024/04/05 14:33:32 by jmafueni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# define  ERR_MALLOC -3
# define SUCCESS 100

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

t_list	*ft_lstlast(t_list *stash);
char	*get_next_line(int fd);
int		ft_found_newline(t_list *stash);
int		ft_strlen(char *str);
int		add_to_stash(t_list **stash, char *buf, int bytes_read);
int		extract_line(t_list *stash, char **line);
int		generate_line(t_list *stash, char **line);
int		read_to_stash(t_list **stash, int fd);
void	clean_stash(t_list **stash);
void	free_stash(t_list **stash);

#endif
