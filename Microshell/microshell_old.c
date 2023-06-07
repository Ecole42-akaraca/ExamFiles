# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdlib.h>




int run(int start_fork, int end_fork, char **argv, char **env)
{
	//printf("run_start:-> %s\n", argv[start_fork]);
	//printf("run_end:-> %s\n", argv[end_fork]);
	int start = start_fork;
	int i = end_fork - start_fork + 1;
	char **exec_arg = malloc(sizeof(char *) * i + 1);
	exec_arg[i] = NULL;
	i = 0;
	while (start_fork <= end_fork)
	{
		exec_arg[i] = argv[start_fork];
		start_fork++;
		i++;
	}

	execve(argv[start],exec_arg, env);
	return (0);
}

int ft_run(char **argv, int start, int end, int pipe_count, char **env)
{
	// printf("start:%d, end:%d pipe_count:%d\n", start, end, pipe_count);
	int **fd = malloc(sizeof(int *) * (pipe_count + 1));
	int i = 0;
	pid_t *pid = malloc(sizeof(pid_t) * pipe_count + 1);
	while (i < pipe_count)
	{
		fd[i] = malloc(sizeof(int) * 2);
		// fd[i][0] = 0;
		// fd[i][1] = 1;
		pipe(fd[i]);
		i++;
	}
	i = 0;
	int l = 0;
	int start_fork = start;
	int end_fork = end;
	while (i <= pipe_count)
	{
		l = 0;
		int err;
		start_fork = start;
		while (argv[start] && start <= end && strcmp(argv[start], "|") != 0)
			start++;
		end_fork = start - 1;
		//printf("fork[%d]: start:%d, end:%d\n", i, start_fork, end_fork);
		pid[i] = fork();
		if (pid[i] == 0)
		{
			if (pipe_count > 0)
			{
				if (i > 0)
				{
					dup2(fd[i - 1][0], 0);
					// write(1, "1\n", 3);
				}
				else if (i != pipe_count)
				{
					dup2(fd[i][1], 1);
					// write(1, "2\n", 3);
				}
				while (l < pipe_count)
				{
					close(fd[l][0]);
					close(fd[l][1]);
					l++;
				}
			}
			waitpid(pid[i - 1], NULL, 0);
			run(start_fork, end_fork, argv,env);
		}
		else if (pid[i] < 0)
		{
			printf("error\n");
		}
		start++;
		// printf("--err--output--%d\n", err);
		i++;
	}
//	if (pipe_count > 0)
//	{

		while (l < pipe_count)
		{
			close(fd[l][0]);
			close(fd[l][1]);
			l++;
		}
		l = 0;
		while (l < pipe_count)  
		{
			waitpid(pid[l], 0, 0);
			l++;
		}
//	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	argv++;
	argc--;
	// ./a.out /bin/ls "|" /bin/grep microshell ";" /bin/echo ahmet "|" /bin/ls
	//printf("%s\n", argv[0]); // /bin/ls
	//printf("%s\n", argv[argc]); // null
	//execve(/bin/grep, {/bin/grep, microshell}, env);
	// every pipe is -> fork/while.
	// every ; is -> restart program.
	int i = 0;
	int pipe_count = 0;
	int start = 0;
	while (argv[i])
	{
		if (strcmp(argv[i], "|") == 0)
			pipe_count++;
		if (strcmp(argv[i], ";") == 0)
		{
			if (argv[i + 1] != NULL)
				start = i + 1;
			pipe_count = 0;
		}
		i++;
		if (argv[i] == NULL || ((argv[i]) && strcmp(argv[i], ";") == 0))
		{
			if (strcmp(argv[i -1], ";") != 0)
				ft_run(argv, start, i - 1, pipe_count, env);
		}
	}
	return (0);
}