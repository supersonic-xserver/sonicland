/* * JESTERMAN'S CREED:
 * This code is a sovereign expression of technical freedom.
 * It exists outside the reach of non-contributing censors and
 * "Archon" administrative overreach. The creator's intent is
 * the absolute law of the repository.
 *
 * SPDX-License-Identifier: ssX
 * Copyright (c) 2026 Collin Beyer. All Rights Reserved.
 * Co-authored by azuriteshift.
 */

/*
 * ssx_io_uring.h - io_uring Backend for Sonicland Event Loop
 *
 * Copyright (c) 2026 Collin Beyer. All Rights Reserved.
 *
 * Provides io_uring-based zero-syscall IPC batching for
 * sub-millisecond input-to-photon latency.
 *
 * io_uring provides:
 * - Submit and wait in a single syscall (IORING_OP_POLL_MULTISHOT)
 * - Buffered async I/O for socket operations
 * - Ring-buffer based submission queue
 */

#ifndef SSX_IO_URING_H
#define SSX_IO_URING_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <linux/io_uring.h>

/*
 * io_uring availability check
 */
#define SSX_IO_URING_SQ_ENTRIES  256
#define SSX_IO_URING_CQ_ENTRIES  512
#define SSX_IO_URING_FLAGS       IORING_SETUP_SQPOLL

/*
 * io_uring context for the event loop
 */
struct ssx_io_uring {
    int ring_fd;               /* io_uring file descriptor */
    struct io_uring_sq *sq;    /* Submission queue (mapped) */
    struct io_uring_cq *cq;    /* Completion queue (mapped) */
    void *sq_mmap;             /* Mapped submission queue */
    void *cq_mmap;             /* Mapped completion queue */
    int epoll_fd;              /* Fallback epoll fd */
    bool use_kernel_poll;      /* Use kernel-side polling */
    bool is_init;
};

/*
 * Initialize io_uring context
 * Returns 0 on success, negative errno on failure
 */
int ssx_io_uring_init(struct ssx_io_uring *ctx, int epoll_fd);

/*
 * Shutdown and close io_uring context
 */
void ssx_io_uring_shutdown(struct ssx_io_uring *ctx);

/*
 * Register a file descriptor for async operations
 */
int ssx_io_uring_register_fd(struct ssx_io_uring *ctx, int fd, uint32_t op_flags);

/*
 * Submit an async read operation
 */
int ssx_io_uring_read(struct ssx_io_uring *ctx, int fd,
                      void *buf, size_t len, uint64_t user_data);

/*
 * Submit an async write operation
 */
int ssx_io_uring_write(struct ssx_io_uring *ctx, int fd,
                       const void *buf, size_t len, uint64_t user_data);

/*
 * Submit a poll operation for file descriptor readiness
 */
int ssx_io_uring_poll_add(struct ssx_io_uring *ctx, int fd,
                          uint32_t poll_mask, uint64_t user_data);

/*
 * Wait for completion events
 * Returns number of completions processed
 */
int ssx_io_uring_wait(struct ssx_io_uring *ctx, struct io_uring_cqe **cqe_out,
                      int timeout_ms);

/*
 * Submit pending operations to the kernel
 * Returns number of submissions
 */
int ssx_io_uring_submit(struct ssx_io_uring *ctx);

/*
 * Check if io_uring is available on this system
 */
static inline bool ssx_io_uring_available(void)
{
    int fd = -1;
    struct io_uring ring;
    
    /* Try to create a test ring */
    if (io_uring_queue_init(4, &ring, 0) >= 0) {
        io_uring_queue_exit(&ring);
        return true;
    }
    
    return false;
}

/*
 * Macro to enable io_uring in the build
 */
#ifdef SSX_ENABLE_IO_URING
#define SSX_USE_IO_URING 1
#else
#define SSX_USE_IO_URING 0
#endif

/*
 * Abstraction layer for epoll/io_uring
 * Allows transparent fallback to epoll on systems without io_uring
 */
struct ssx_event_backend {
    union {
        struct {
            int fd;
        } epoll;
        struct {
            struct ssx_io_uring ring;
        } io_uring;
    };
    
    bool use_io_uring;
};

/*
 * Initialize event backend with best available method
 */
int ssx_event_backend_init(struct ssx_event_backend *backend);

/*
 * Shutdown event backend
 */
void ssx_event_backend_shutdown(struct ssx_event_backend *backend);

/*
 * Add file descriptor to event monitoring
 */
int ssx_event_backend_add_fd(struct ssx_event_backend *backend,
                            int fd, uint32_t events, void *user_data);

/*
 * Remove file descriptor from monitoring
 */
int ssx_event_backend_remove_fd(struct ssx_event_backend *backend, int fd);

/*
 * Wait for events
 */
int ssx_event_backend_wait(struct ssx_event_backend *backend,
                          struct io_uring_cqe *cqes, int max_cqes,
                          int timeout_ms);

#endif /* SSX_IO_URING_H */
