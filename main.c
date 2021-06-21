#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "./libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


char	*get_addres(char **envp, char **argv)
{
	// разобраться с маллоком и сдлеать функцию универсальной
	
	char *addres;
	char **tmp;
	int	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	tmp = ft_split(envp[i] + 5, ':');
	char **comand = ft_split(argv[2], ' ');
	i = 0;
	while (tmp[i]){
		char *tmp_addres;
		tmp_addres = ft_strjoin(tmp[i], "/");
		tmp_addres = ft_strjoin(tmp_addres, comand[0]);
		printf("%d ", access(tmp_addres, F_OK));
		printf("%s\n", tmp_addres);
		// добавить условие если не нашли команду
		if (access(tmp_addres, F_OK) == 0)
			return tmp_addres;
		i++;
	}
		
	
}
void	call_cmd1_process(int *fd, char **argv, char **envp)
{
	char *name_program = get_addres(envp, argv);
	int fd_input_file;
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
	char **cmd1 = ft_split(argv[2], ' ');
	if (execve(name_program, cmd1, envp)  < 0) // проверить на ошибку
	{
		perror("CMD1 fail");
	}
}

void	call_cmd2_process(int *fd, char **argv, char **envp)
{
	char result[100] = "/usr/bin/";
	int fd_output_file = open(argv[4], O_WRONLY);
	dup2(fd_output_file, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	char **cmd2 = ft_split(argv[3], ' ');
	strcat(result, cmd2[0]);
	if (execve(result, cmd2, NULL) < 0)
		printf("cmd2 fail\n");
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];
	pid_t	pid1;
	int	pid2;

	if (pipe(fd) < 0)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0) //cmd1 process
		call_cmd1_process(fd, argv, envp);
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if(pid2 == 0) // cmd2 process
		call_cmd2_process(fd, argv, envp);
	close(fd[1]);
	close(fd[0]);
	return (123);
}

