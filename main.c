#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "./libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void	free_memory(char** split1, char **split2)
{
	int i;

	i = 0;
	while (split1[i])
		free(split1[i++]);
	i = 0;
	while(split2[i])
		free(split2[i++]);
	free(split2);
	free(split1);
}

char	*get_addres(char **envp, char *cmd_string)
{
	char *addres;
	char *addres_full;
	char **strings_way;
	char **comand;
	int	i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	strings_way = ft_split(envp[i] + 5, ':');
	comand = ft_split(cmd_string, ' ');
	i = 0;
	while (strings_way[i])
	{
		addres = ft_strjoin(strings_way[i], "/");
		addres_full = ft_strjoin(addres, comand[0]);
		free(addres);
		if (access(addres_full, F_OK) == 0)
		{
			free_memory(strings_way, comand);
			return addres_full;
		}
		free(addres_full);
		i++;
	}
	ft_putstr_fd(comand[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_memory(strings_way, comand);
	exit (5);
}

void	call_cmd1_process(int *fd, char **argv, char **envp)
{
	char *name_program;
	int fd_input_file;
	char **cmd1;

	if (access(argv[1], O_RDONLY) == -1)
	{
		perror(argv[1]);
		exit (1);
	}
	else
	{
		fd_input_file = open(argv[1], O_RDONLY);
	}
	dup2(fd_input_file, STDIN_FILENO); 
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	cmd1 = ft_split(argv[2], ' ');
	name_program = get_addres(envp, argv[2]);
	execve(name_program, cmd1, envp);
}

void	call_cmd2_process(int *fd, char **argv, char **envp)
{
	char *name_program; 	
	int  fd_output_file;
	char *file_name;
	char **cmd2;

	file_name = ft_strjoin("./", argv[4]);
	if (!access(file_name, O_RDWR | O_CREAT))
	{
		perror(argv[4]);
		exit (1);
	}
	else
	{
		fd_output_file = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0664);
		free(file_name);
	}	
	dup2(fd_output_file, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	cmd2 = ft_split(argv[3], ' ');
	name_program = get_addres(envp, argv[3]);
	execve(name_program, cmd2, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (argc == 5) 
	{
		if (pipe(fd) < 0)
			return (1);
		pid1 = fork();
		if (pid1 < 0)
			return (2);
		if (pid1 == 0) //cmd1 process
			call_cmd1_process(fd, argv, envp);
		wait(NULL);
		pid2 = fork();
		if (pid2 < 0)
			return (3);
		if(pid2 == 0) // cmd2 process
			call_cmd2_process(fd, argv, envp);
		close(fd[1]);
		close(fd[0]);
		wait(NULL);
		return (0);
	}
	else
	{
		ft_putstr_fd("Please use: ./pipex infile cmd1 cmd2 outfile\n", 2);
		return (4);
	}
}

