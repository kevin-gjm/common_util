#include "setup.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <inttypes.h>  
#include <string.h>


//bool daemonize(int nochdir, int noclose)
//{
//	int fd;
//
//	switch (fork())
//	{
//	case -1:
//		fprintf(stderr, "fork:%d\n", errno);
//		return false;
//	case 0:
//		break;
//	default:
//		fprintf(stderr, "fork:%d\n", errno);
//		_exit(0);
//	}
//
//	if (setsid() == -1)
//	{
//		fprintf(stderr, "setsid:%d\n", errno);
//		return false;
//	}
//
//	if (nochdir == 0)
//	{
//		if (chdir("/") != 0)
//		{
//			fprintf(stderr, "chdir:%d\n", errno);
//			return false;
//		}
//	}
//	if (noclose == 0 && (fd = open("/dev/null", O_RDWR, 0)) != -1)
//	{
//		if (dup2(fd, STDIN_FILENO) < 0)
//		{
//			fprintf(stderr, "dup2 stdin:%d\n", errno);
//			return false;
//		}
//		if (dup2(fd, STDOUT_FILENO) < 0)
//		{
//			fprintf(stderr, "dup2 stdout:%d\n", errno);
//			return false;
//		}
//		if (dup2(fd, STDERR_FILENO) < 0)
//		{
//			fprintf(stderr, "dup2 stderror:%d\n", errno);
//			return false;
//		}
//
//		if (fd > STDERR_FILENO)
//		{
//			if (close(fd) < 0)
//			{
//				fprintf(stderr, "stdio:%d\n", errno);
//				return false;
//			}
//		}
//	}
//	return true;
//
//}
bool set_coredump(void)
{
	struct rlimit rlim;
	struct rlimit rlim_new;
	if (getrlimit(RLIMIT_CORE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			(void)setrlimit(RLIMIT_CORE, &rlim_new);
		}
	}

	if ((getrlimit(RLIMIT_CORE, &rlim) != 0) || rlim.rlim_cur == 0)
	{
		fprintf(stderr, "failed to ensure corefile creation\n");
		return false;
	}

	return true;
}
bool set_fdlimit(rlim_t fds)
{
	struct rlimit rl;
	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		fprintf(stderr, "Could not get file descriptor limit:%d\n", errno);
		return false;
	}

	rl.rlim_cur = fds;
	if (rl.rlim_max < rl.rlim_cur)
		rl.rlim_max = rl.rlim_cur;

	if (setrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		fprintf(stderr, "Failed to set limit for the number of file "
			"descriptors (%d). Try running as root or giving a "
			"smaller value to -f.\n", errno);
		return false;
	}

	return true;
}
bool switch_user(const char* user)
{
	struct passwd *pw;

	if (getuid() == 0 || geteuid() == 0)
	{
		if (user == NULL || user[0] == 0)
		{
			fprintf(stderr, "Must specify '-u root' if you want to run as root\n");
			return false;
		}

		pw = getpwnam(user);
		if (pw == NULL)
		{
			fprintf(stderr, "Could not find user '%s'\n", user);
			return false;
		}

		if (setgid(pw->pw_gid) == -1 || setuid(pw->pw_uid) == -1)
		{
			fprintf(stderr, "Could not switch to user '%s'\n", user);
			return false;
		}
	}
	else if (user != NULL)
	{
		fprintf(stderr, "Must be root to switch users\n");
		return false;
	}

	return true;
}

void __shutdown_handler(int signal_arg)
{
	if (signal_arg == SIGUSR1)
	{
	}
	else
	{
	}
}

bool set_signals(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));

	sa.sa_handler = SIG_IGN;
	if (sigemptyset(&sa.sa_mask) == -1 || sigaction(SIGPIPE, &sa, 0) == -1)
	{
		fprintf(stderr, "Could not set SIGPIPE handler (%d)\n", errno);
		return true;
	}

	sa.sa_handler = __shutdown_handler;
	if (sigaction(SIGTERM, &sa, 0) == -1)
	{
		fprintf(stderr, "Could not set SIGTERM handler (%d)\n", errno);
		return true;
	}

	if (sigaction(SIGINT, &sa, 0) == -1)
	{
		fprintf(stderr, "Could not set SIGINT handler (%d)\n", errno);
		return true;
	}

	if (sigaction(SIGUSR1, &sa, 0) == -1)
	{
		fprintf(stderr, "ould not set SIGUSR1 handler (%d)\n", errno);
		return true;
	}

	if (sigaction(SIGHUP, &sa, 0) == -1)
	{
		fprintf(stderr, "Could not set SIGHUP handler (%d)\n", errno);
		return true;
	}

	return false;

}
bool pid_write(const char *pid_file)
{
	FILE *f;

	f = fopen(pid_file, "w");
	if (f == NULL)
	{
		fprintf(stderr, "Could not open pid file for writing: %s (%d)\n", pid_file, errno);
		return false;
	}

	fprintf(f, "%" PRId64 "\n", (int64_t)getpid());

	if (fclose(f) == -1)
	{
		fprintf(stderr, "Could not close the pid file: %s (%d)\n", pid_file, errno);
		return false;
	}

	return true;
}

void pid_delete(const char *pid_file)
{
	if (unlink(pid_file) == -1)
	{
		fprintf(stderr, "Could not remove the pid file: %s (%d)\n", pid_file, errno);
	}
}


