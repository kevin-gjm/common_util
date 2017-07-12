#ifndef UTIL_SETUP_H_
#define UTIL_SETUP_H_
#include <sys/resource.h>
#define __STDC_FORMAT_MACROS
//bool daemonize(int nochdir, int noclose);
bool set_coredump(void);
bool set_fdlimit(rlim_t fds);
bool switch_user(const char* user);
bool set_signals(void);
bool pid_write(const char *pid_file);
void pid_delete(const char *pid_file);

#endif // UTIL_SETUP_H_