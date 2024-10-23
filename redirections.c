/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarouf <amarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:27:15 by amarouf           #+#    #+#             */
/*   Updated: 2024/10/14 17:00:00 by amarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_redirection(t_file_red *red, int fd)
{
	if (red->typeoffile == REDIRECTION_OUT
		|| red->typeoffile == REDIRECTION_APPEND)
	{
		if (dup2(fd, 1) == -1 || close(fd) == -1)
			(ft_malloc(0, 'f', true), exit(1));
		return (1);
	}
	else if (red->typeoffile == REDIRECTION_IN)
	{
		if (dup2(fd, 0) == -1 || close(fd) == -1)
			(ft_malloc(0, 'f', true), exit(1));
		return (0);
	}
	return (-99);
}

int	ft_check_open(t_parser *parser, int std_in)
{
	int	err;

	err  = access(parser->red->filename, F_OK);
	if (err == -1)
		(ft_malloc(0, 'f', true), exit(1));
	if (err)
	{
		write(std_in, parser->red->filename,
			ft_strlen(parser->red->filename));
		write(std_in, ": No such file or directory\n", 29);
		return (-1337);
	}
	return (1);
}

void	open_close_fd(t_parser *parser)
{
	int	fd;

	fd = open(parser->red->filename, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		(ft_malloc(0, 'f', true), exit(1));
	if (close(fd) == -1)
		(ft_malloc(0, 'f', true), exit(1));
}

int	open_files(t_parser *parser, int std_in)
{
	int	fd;

	while (parser->red->next
		&& parser->red->typeoffile == parser->red->next->typeoffile)
	{
		if (parser->red->typeoffile == REDIRECTION_IN)
		{
			if (ft_check_open(parser, std_in) == -1337)
				return (-1337);
		}
		open_close_fd(parser);
		parser->red = parser->red->next;
	}
	if (parser->red->typeoffile == REDIRECTION_APPEND)
		err(fd = open(parser->red->filename, O_RDWR | O_CREAT | O_APPEND, 0644));
	else if (parser->red->typeoffile == REDIRECTION_IN)
	{
		if (ft_check_open(parser, std_in) == -1337)
			return (-1337);
		err(fd = open(parser->red->filename, O_RDWR, 0644));
	}
	else
		err(fd = open(parser->red->filename, O_CREAT | O_TRUNC | O_RDWR, 0644));
	return (fd);
}
