#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

FILE *log_file;

void signal_handler(int signo, siginfo_t *sinfo, void *ucontext) {
    fprintf(log_file, "Signal code: %i\n", sinfo->si_code);
    fprintf(log_file, "Signal number: %i\n", sinfo->si_signo);
    fprintf(log_file, "Signal value: %i\n", sinfo->si_value);
    fprintf(log_file, "Errno association: %i\n", sinfo->si_errno);
    fprintf(log_file, "Process ID: %i\n", sinfo->si_pid);
    fprintf(log_file, "Real user ID: %i\n", sinfo->si_uid);
    fprintf(log_file, "Address at which fault occurred: %i\n", sinfo->si_addr);
    fprintf(log_file, "Exit value: %i\n", sinfo->si_status);
    fprintf(log_file, "Band event for SIGPOLL/SIGIO: %ld\n", sinfo->si_band);
    fflush(log_file);
}


int main() {
    log_file = fopen("./log.txt", "a+t");
    printf("New process PID: %i\n", getpid());
    fprintf(log_file, "New process PID: %i\n", getpid());
    fflush(log_file);
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGTERM, &action, NULL);

    while (1) {
        sleep(3);
        fprintf(log_file, "sleep\n");
        fflush(log_file);
    }
    exit(0);
}
