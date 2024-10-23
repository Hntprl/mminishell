/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_multiple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarouf <amarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:24:54 by amarouf           #+#    #+#             */
/*   Updated: 2024/10/14 16:29:49 by amarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_commands(char **split, t_list *env)
{
	char	*path;
	char	*cmd;
	int		pid;
	char	**envp;
	char	*jn;

	jn = NULL;
	err(pid = fork());
	if (pid == 0)
	{
		envp = ft_list_to_str(env);
		cmd = ft_strjoin("/", split[0]);
		path = ft_checkaccess(envp, cmd);
		jn = ft_strjoin(path, cmd);
		(commandcheck(envp, cmd), execve(jn, split, envp), free(jn), exit(127));
	}
	err(wait(&pid));
	g_status = WEXITSTATUS(pid);
}

void	ft_close_std(int std_in, int std_out)
{
		(err(dup2(std_out, 1)), err(close(std_out)));
		(err(dup2(std_in, 0)), err(close(std_in)));
}

void	ft_single_command(t_parser *parser, t_list **ls_env)
{
	int	fd;
	int	std_in;
	int	std_out;

	fd = 1;
	std_in = 0;
	std_out = 1;
	err(std_in = dup(0));
	err(std_out = dup(1));
	while (parser->red)
	{
		err(fd = open_files(parser, std_in));
		if (fd == -1337)
			return ;
		ft_redirection(parser->red, fd);
		parser->red = parser->red->next;
	}
	if (ft_buildins(parser, ls_env))
	{
		ft_close_std(std_in, std_out);
		return ;
	}
	else
		shell_commands(parser->command, (*ls_env));
	ft_close_std(std_in, std_out);
}

void	multiple_commands_cleaner(int input, int p[2], int std_out, int std_in)
{
	err(close(input));
	close_fd(p);
	while (wait(NULL) > 0)
		;
	ft_close_std(std_in, std_out);
}

void	ft_multiple_commands(t_parser *parser, t_list **ls_env)
{
	int	input;
	int	p[2];
	int	std_in;
	int	std_out;

	err(std_in = dup(0));
	err(std_out = dup(1));
	err(pipe(p));
	ft_first_command(parser, ls_env, p);
	err(close(p[1]));
	parser = parser->next;
	input = p[0];
	if (ft_parsersize(parser) > 1)
	{
		while (parser->next)
		{
			(err(dup2(input, 0)), err(close(input)), err(pipe(p)));
			ft_all_commands(parser, ls_env, p);
			(err(close(p[1])), input = p[0]);
			parser = parser->next;
		}
	}
	ft_last_command(parser, ls_env, p);
	multiple_commands_cleaner(input, p, std_out, std_in);
}

void	ft_command_check(t_parser *parser, t_list **ls_env)
{
	if (!parser)
		return ;
	if (ft_parsersize(parser) == 1)
		ft_single_command(parser, ls_env);
	else
		ft_multiple_commands(parser, ls_env);
}
