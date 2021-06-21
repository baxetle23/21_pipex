#include "pipex.h"

char	*get_addres(char **envp, char *cmd_string)
{
	t_pipex	a;

	a.i = 0;
	while (ft_strncmp(envp[a.i], "PATH=", 5))
		a.i++;
	a.strings_way = ft_split(envp[a.i] + 5, ':');
	a.comand = ft_split(cmd_string, ' ');
	a.i = 0;
	while (a.strings_way[a.i])
	{
		a.addres = ft_strjoin(a.strings_way[a.i], "/");
		a.addres_full = ft_strjoin(a.addres, a.comand[0]);
		free(a.addres);
		if (access(a.addres_full, F_OK) == 0)
		{
			free_memory(a.strings_way, a.comand);
			return (a.addres_full);
		}
		free(a.addres_full);
		a.i++;
	}
	ft_putstr_fd(a.comand[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_memory(a.strings_way, a.comand);
	exit (5);
}

void	call_cmd1_process(int *fd, char **argv, char **envp)
{
	char	*name_program;
	int		fd_input_file;
	char	**cmd1;

	fd_input_file = open(argv[1], O_RDONLY);
	if (fd_input_file < 0)
	{
		perror(argv[1]);
		exit (7);
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
	char	*name_program;
	int		fd_output_file;
	char	*file_name;
	char	**cmd2;

	file_name = ft_strjoin("./", argv[4]);
	fd_output_file = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0664);
	free(file_name);
	if (fd_output_file < 0)
	{
		perror(argv[4]);
		exit (6);
	}
	dup2(fd_output_file, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	cmd2 = ft_split(argv[3], ' ');
	name_program = get_addres(envp, argv[3]);
	execve(name_program, cmd2, NULL);
}

void	create_process(int *fd, char **argv, char **envp, int num_child)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		exit (2);
	if (pid == 0 && num_child == 1)
		call_cmd1_process(fd, argv, envp);
	if (pid == 0 && num_child == 2)
		call_cmd2_process(fd, argv, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc == 5)
	{
		if (pipe(fd) < 0)
			return (1);
		create_process(fd, argv, envp, 1);
		wait(NULL);
		create_process(fd, argv, envp, 2);
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
