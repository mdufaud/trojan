/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:22:16 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:04:24 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <signal.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

static t_durex  *g_durex = NULL;

static void handle_child(void)
{
    pid_t           p;
    int             status;
    size_t          max;
    size_t          j;
    char            *s;

    max = 0;
    while ((p = waitpid(-1, &status, WNOHANG)) > 0 && max < MAX_ITER_SIGCHLD)
    {
        j = 0;
        while (j < g_durex->serv.max_fd)
        {
            if (g_durex->clients[j].shell_pid == p)
            {
                ft_sprintf(&s, "Shell for client %d died\n", p);
                log_info(g_durex, s);
                free(s);
                g_durex->clients[j].shell_pid = 0;
                break ;
            }
            ++j;
        }
        ++max;
    }
    if (max == MAX_ITER_SIGCHLD)
        log_info(g_durex, "Max iter reached searching for dead shells !\n");
}

static void sig_handler(int sig)
{
    char        *s;

    if (sig == SIGTERM)
        log_error(g_durex, "SIGTERM received. Quitting\n");
    else if (sig == SIGSEGV)
        log_error(g_durex, "SIGSEGV received. Quitting\n");
    else
    {
        if (sig == SIGCHLD)
            handle_child();
        else
        {
            ft_sprintf(&s, "%s signal received\n", strsignal(sig));
            log_info(g_durex, s);
            free(s);
        }
    }
}

static void handle_signals(void)
{
    int                 i;
    struct sigaction    sig;

    i = 0;
    sig.sa_handler = sig_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    while (++i < 64)
        sigaction(i, &sig, NULL);
}

static void lock_daemon(t_durex *durex)
{
    if ((durex->daemon.lock = open(LOCK_PATH,
            O_RDWR | O_CREAT, 0644)) == -1)
        log_error(durex, "Could not open lock\n");
    if (flock(durex->daemon.lock, LOCK_EX | LOCK_NB) < 0)
    {
        if (errno == EWOULDBLOCK)
            log_error(durex, "Daemon already running\n");
        else
            log_error(durex, "Flock error\n");
    }
}

void        daemonize(t_durex *durex)
{
    int     sid;
    pid_t   pid;

    lock_daemon(durex);
    if ((pid = fork()) == -1)
        log_error(durex, "Fork failed\n");
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if ((sid = setsid()) == -1)
        log_error(durex, "Setsid failed\n");
    handle_signals();
    if ((pid = fork()) == -1)
        log_error(durex, "Fork failed\n");
    if (pid > 0)
        exit(EXIT_SUCCESS);
    chdir("/");
    durex->daemon.pid = getpid();
    ft_fprintf(durex->daemon.lock, "%d\n", durex->daemon.pid);
    g_durex = durex;
    /*
    durex->daemon.stdin = dup(STDIN_FILENO);
    durex->daemon.stdout = dup(STDOUT_FILENO);
    durex->daemon.stderr = dup(STDERR_FILENO);
    */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    log_info(durex, "Daemon running\n");
}
