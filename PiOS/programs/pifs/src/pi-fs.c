/* 
 * part1: trivial file system that we are going to repurpose for the pi.  
 * 
 * useful helper routines in pi-fs.h/pi-fs-support.c
 *
 * implement anything with unimplemented();
 */

#define FUSE_USE_VERSION 26
#define USE_PI_SHELL

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <assert.h>
#include <demand.h>
#include <signal.h>

#include "pi-fs.h"

static int do_reboot(dirent_t* e, const char* path, const char* buf,
                     size_t size, off_t offset, void* data);

static int do_echo(dirent_t* e, const char* path, const char* buf,
                   size_t size, off_t offset, void* data);

static int do_run(dirent_t* e, const char* path, void* data);


// simple pi file system: no directories.
static dirent_t root[] = {
        {.name = "/echo.cmd",   perm_rw, 0, .on_wr = do_echo},
        {.name = "/reboot.cmd", perm_rw, 0, .on_wr = do_reboot},
        {.name = "/run.cmd",    perm_rw, 0, .on_rel = do_run},
        {.name = "/console",    perm_rd, 0},
        {.name = "/.boot.bin",  perm_rd, 0},
        {0}
};

//static int
//matches_any(const void* thing, const void* list, int elemsize, int n, int (* matches)(const void* a, const void* b)) {
//    for (const void* current = list; current < ((void*) ((char*) list) + (elemsize * n)); current += elemsize) {
//        if (matches(thing, current)) {
//            return 1;
//        }
//    }
//
//    return 0;
//}
//
//static int path_dirent_match(const void* path_v, const void* dirent_v) {
//    const char* path = (const char*) path_v;
//    const dirent_t* dirent = (const dirent_t*) dirent_v;
//
//    return strcmp(path, dirent->name) == 0;
//}

static int pi_open(const char* path, struct fuse_file_info* fi) {
    note("opening\n");
//    if (!matches_any(path, root, sizeof(dirent_t), 4, path_dirent_match))
//        return -ENOENT;

    int retv;
    dirent_t* e = file_lookup(&retv, root, path, flag_to_perm(fi->flags));

    if (!e)
        return retv;


    return 0;
}

static int pi_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info* fi) {
    note("readdir\n");
    if (strcmp(path, "/") != 0)
        return -ENOENT;

    // no sub-directories
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    dirent_t* dir = root;
    while (dir->name) {
        filler(buf, dir->name + 1, NULL, 0);
        dir++;
    }
    return 0;
}

static int pi_getattr(const char* path, struct stat* stbuf) {
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        // we need one link for each subdir since they point to us.
        // plus 2 for "." and parent (or ".." in the case of "/").
        stbuf->st_nlink = 2;
        return 0;
    }

    // regular file.
    dirent_t* e = ent_lookup(root, path);
    if (!e)
        return -ENOENT;

    stbuf->st_size = e->f->n_data;
    stbuf->st_mode = S_IFREG | e->flags;
    stbuf->st_nlink = 1;
    return 0;
}

static int pi_read(const char* path, char* buf, size_t size,
                   off_t offset, struct fuse_file_info* fi) {

    note("****** READ\n");
    int retv;
    dirent_t* e = file_lookup(&retv, root, path, perm_rd);
    if (!e)
        return -retv;

    size_t read_size = (e->f->n_data - offset) < size ? (e->f->n_data - offset) : size;

    memcpy(buf, e->f->data, read_size);

    if (e->on_rd) {
        e->on_rd(e, path, buf, size, offset, fi);
    }

    return read_size;
}

static int pi_write(const char* path, const char* buf, size_t size,
                    off_t offset, struct fuse_file_info* fi) {

    note("****** WRITE:<%s> off=%ld, size=%ld\n", path, offset, size);
    int retv;
    dirent_t* e = file_lookup(&retv, root, path, perm_wr);
    if (!e)
        return retv;

    if ((size + offset) > e->f->n_alloc) {
        file_realloc(e->f, size + offset);
    }

    e->f->n_data = e->f->n_data > (size + offset) ? e->f->n_data : (size + offset);
    memcpy(e->f->data + offset, buf, size);

    if(e->on_wr) {
        e->on_wr(e, path, buf, size, offset, fi);
    }

    return size;
}

/** Change the size of a file */
static int pi_truncate(const char* path, off_t length) {
    int retv;
    dirent_t* e = file_lookup(&retv, root, path, perm_wr);
    if (!e)
        return retv;

    e->f->n_data = length;
    if((e->f->n_alloc - length) > 0) {
        memset(e->f->data + length, 0, e->f->n_alloc - length);
    }

    return 0;
}

static int pi_ftruncate(const char* path, off_t offset, struct fuse_file_info* fi) {
    return pi_truncate(path, offset);
}

static int pi_release(const char* path, struct fuse_file_info* fi) {
    int retv;
    dirent_t* e = file_lookup(&retv, root, path, flag_to_perm(fi->flags));

    if (!e)
        return retv;

    if(e->on_rel) {
        e->on_rel(e, path, fi);
    }

    return 0;

}

/* these were enough for me for the lab. */
static struct fuse_operations pi_oper = {
        .getattr = pi_getattr,
        .readdir = pi_readdir,
        .open = pi_open,
        .read = pi_read,
        .write = pi_write,
        .truncate = pi_truncate,
        .ftruncate = pi_ftruncate,
        .release = pi_release
};

/*
    -d Enable debugging output (implies -f).

    -f Run in foreground; this is useful if you're running under a
    debugger. WARNING: When -f is given, Fuse's working directory is
    the directory you were in when you started it. Without -f, Fuse
    changes directories to "/". This will screw you up if you use
    relative pathnames.

    -s Run single-threaded instead of multi-threaded. 
*/
#ifdef USE_PI_SHELL

#include "patch.c"

#else
int main(int argc, char* argv[]) {
    assert(ent_lookup(root, "/console"));
    assert(ent_lookup(root, "/echo.cmd"));
    assert(ent_lookup(root, "/reboot.cmd"));
    assert(ent_lookup(root, "/run.cmd"));
    return fuse_main(argc, argv, &pi_oper, 0);
}
#endif

// not needed for this part.
static int do_reboot(dirent_t* e, const char* path, const char* buf,
                     size_t size, off_t offset, void* data) {
    fd_puts(pi_wr_fd, "reboot\n");
    kill(getpid(), SIGINT);
    return 0;
}

static int do_echo(dirent_t* e, const char* path, const char* buf,
                   size_t size, off_t offset, void* data) {
    fd_puts(pi_wr_fd, "echo ");
    fd_puts(pi_wr_fd, buf);
    fd_putc(pi_wr_fd, '\n');
    echo_until_fn(pi_rd_fd, prompt, wr_console);
    return 0;
}

static int do_run(dirent_t* e, const char* path, void* data) {
    fprintf(stderr, "BOOTING!\n");
    fd_puts(pi_wr_fd, "MTPT/run.cmd\n");
    echo_until_fn(pi_rd_fd, prompt, wr_console);
    return 0;
} 
