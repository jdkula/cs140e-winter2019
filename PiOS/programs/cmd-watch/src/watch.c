#include <demand.h>
#include <unistd.h>
#include <wait.h>
#include <memory.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

const char* SOURCE_EXTS[] = {
        ".s",
        ".S",
        ".c",
        ".h",
        NULL
};

time_t last_mod_time = 0;

pid_t launch(char** cmd_argv, char** env) {
    int pid;

    safe_sys(pid = fork())

    if (pid == 0) {
        safe_sys(execve(cmd_argv[0], cmd_argv, env))
        exit(-1);
    }

    return pid;
}

time_t get_modified_time(const struct dirent* info) {
    struct stat statbuf;

    safe_sys(stat(info->d_name, &statbuf));

    return statbuf.st_mtime;
}

int wait_for_completion(pid_t pid) {
    int status;
    safe_sys(waitpid(pid, &status, 0));

    return status;
}

bool is_source_file(const char* name) {
    size_t length = strlen(name);
    const char** source = SOURCE_EXTS;

    while (*source) {
        if (strcmp(name + length - strlen(*source), *source) == 0) {
            return true;
        }

        source++;
    }

    return false;
}

size_t scan_for_changes(const char* dir) {
    DIR* ref;
    safe_sys(ref = opendir(dir));

    struct dirent* info;
    while ((info = readdir(ref)) != NULL) {
        if (is_source_file(info->d_name)) {
            time_t modtime = get_modified_time(info);
            if(modtime != last_mod_time) {
                last_mod_time = modtime;
                return true;
            }
        }
    }

    closedir(ref);
    return false;
}

int main(int argc, char** argv, char** env) {
    while (true) {
        if (scan_for_changes(".")) {
            wait_for_completion(launch(argv + 1, env));
        }
        usleep(30000);
    }
}