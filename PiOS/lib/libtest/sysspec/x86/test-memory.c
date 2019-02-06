/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: tests.h
 * --------------
 * Provides a testing harness for x86.
 * Currently helps debug memory accesses.
 *
 * Here's the general architecture of the test harness:
 * We assume that all writes are commute, and all reads are commute,
 * but reads and writes aren't commute with one another.
 * Reads and writes are queued. When there are multiple writes to
 * the same location, no new entries are queued; the old value is
 * simply overwritten. The same is not assumed with reads.
 *
 * When a read follows a write, or vice versa, or there's a memory barrier,
 * memory is "committed" -- all this does is sort the buffer by address
 * then value, and calls the on-commit event handler to further deal with it.
 *
 * This uses the mem_on_* event handlers.
 */

#include <stddef.h>
#include <x86/mem-x86-extensions.h>
#include <boolean.h>
#include <stdlib.h>

#include "x86/test-memory-handlers.h"
#include "x86/test-memory.h"

/** Event handler; triggered when memory is read from (in the order the reads occur). */
memory_operation_handler_t test_on_read = NULL;

/** Event handler; triggered when memory is written to (in the order the writes occur, including duplicates). */
memory_operation_handler_t test_on_write = NULL;

/** Event handler; triggered when a memory barrier is triggered. */
memory_barrier_handler_t test_on_barrier = test_default_barrier_handler;

/** Event handler; triggered when memory is committed (this contains the culled, sorted list of memory actions). */
on_commit_t test_on_commit = test_default_commit_handler;

/** What the last memory action was; NONE (if there was just a memory barrier or at the start); READ, or WRITE. */
uint8 commit_mode = COMMIT_NONE;

mem_t commit_queue[MEMORY_CHUNKS];
uint32 queue_size = 0;

/** Locates something with [info]'s address in the queue, or returns NULL if not found. */
static mem_t* queue_locate(const mem_t* info) {
    for(uint32 i = 0; i < queue_size; i++) {
        if(info->addr == commit_queue[i].addr) {
            return &commit_queue[i];
        }
    }

    return NULL;
}

/** Inserts the given [info] into the queue. */
static void queue_insert(const mem_t* info) {
    commit_queue[queue_size] = *info;
    queue_size++;
}

/** Commits the queue by sorting it, calling the event handler, then clearing it. */
static void queue_commit() {
    if(commit_mode == COMMIT_NONE) return;

    qsort(commit_queue, queue_size, sizeof(mem_t), mem_compare);

    if(test_on_commit != NULL) test_on_commit(commit_queue, queue_size, commit_mode);

    queue_size = 0;
}

/** Catches write events and queues/commits appropriately. */
static void on_write(const mem_t* info) {
    if(commit_mode != COMMIT_WRITE) {
        queue_commit();
        commit_mode = COMMIT_WRITE;
    }

    mem_t* last_op = queue_locate(info);
    if(last_op == NULL) {
        queue_insert(info);
    } else {
        last_op->val = info->val;
    }

    if(test_on_write != NULL) test_on_write(info);
}

/** Catches read events and queues/commits appropriately. */
static void on_read(const mem_t* info) {
    if(commit_mode != COMMIT_READ) {
        queue_commit();
        commit_mode = COMMIT_READ;
    }
    queue_insert(info);

    if(test_on_read != NULL) test_on_read(info);
}

/** Catches barriers and commits. */
static void on_barrier() {
    if(test_on_barrier != NULL) test_on_barrier();
    queue_commit();
    commit_mode = COMMIT_NONE;
}

/**
 * Initializes event handlers so that this library
 * can be used. Note that reassigning the
 * mem_on_* event handlers will break it!
 */
void test_mem_init() {
    mem_on_read = on_read;
    mem_on_write = on_write;

    mem_on_barrier = on_barrier;
}

/** Gets the raw memory representation so that clients can poke around. */
const mem_t* get_raw_memory() {
    return ram_emulated;
}