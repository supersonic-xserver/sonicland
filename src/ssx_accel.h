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
 * ssx_accel.h - Supersonic Xserver Acceleration Layer
 *
 * Copyright (c) 2026 Collin Beyer. All Rights Reserved.
 * 
 * Provides DMA-BUF zero-copy memory sharing utilities and
 * aggressive performance optimizations for sub-millisecond latency.
 */

#ifndef SSX_ACCEL_H
#define SSX_ACCEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * DMA-BUF memory sharing flags
 * These are used to ensure zero-copy buffer passing between
 * X clients and the Wayland compositor.
 */
#define SSX_BUFFER_FLAG_DMA_BUF    0x01
#define SSX_BUFFER_FLAG_SHM_FALLBACK 0x02  /* Warning: not zero-copy */

/*
 * Performance configuration for latency-critical paths
 */
#define SSX_ACCEL_IO_URING_ENABLED    1
#define SSX_ACCEL_BATCH_SIZE         32
#define SSX_ACCEL_MIN_COMssX_SIZE    64

/*
 * Statistics for performance monitoring
 */
struct ssx_accel_stats {
    uint64_t dmabuf_buffers;
    uint64_t shm_fallbacks;
    uint64_t damage_regions_computed;
    uint64_t zero_copy_transfers;
    uint64_t total_latency_ns;
};

/*
 * DMA-BUF buffer handle for zero-copy sharing
 */
struct ssx_dmabuf_buffer {
    int fd;                     /* DMA-BUF file descriptor */
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t format;            /* FourCC format code */
    uint64_t modifier;          /* Layout modifier */
    void *map_ptr;              /* Mapped address for CPU access */
    size_t size;
    bool is_valid;
};

/*
 * Initialize the acceleration layer
 * Returns 0 on success, negative errno on failure
 */
int ssx_accel_init(void);

/*
 * Destroy the acceleration layer
 */
void ssx_accel_shutdown(void);

/*
 * Create a DMA-BUF buffer for zero-copy sharing
 * Returns pointer to buffer structure, NULL on failure
 */
struct ssx_dmabuf_buffer *
ssx_dmabuf_create(uint32_t width, uint32_t height, uint32_t format,
                  uint32_t stride, uint64_t modifier);

/*
 * Release a DMA-BUF buffer
 */
void ssx_dmabuf_destroy(struct ssx_dmabuf_buffer *buf);

/*
 * Import a DMA-BUF buffer from an external file descriptor
 * The fd is consumed by this function
 */
struct ssx_dmabuf_buffer *
ssx_dmabuf_import(int fd, uint32_t width, uint32_t height,
                  uint32_t stride, uint32_t format, uint64_t modifier);

/*
 * Export a buffer as a DMA-BUF file descriptor
 * Returns the fd on success, negative errno on failure
 * The caller owns the returned fd
 */
int ssx_dmabuf_export(struct ssx_dmabuf_buffer *buf);

/*
 * Check if a buffer uses DMA-BUF (zero-copy capable)
 * Returns true if DMA-BUF, false if using wl_shm fallback
 */
static inline bool
ssx_buffer_is_zerocopy(uint32_t flags)
{
    return (flags & SSX_BUFFER_FLAG_SHM_FALLBACK) == 0;
}

/*
 * Runtime warning for wl_shm fallback usage
 * This should trigger in debug builds when zero-copy is not achieved
 */
#ifdef DEBUG
#define SSX_WARN_SHM_FALLBACK() \
    fprintf(stderr, "SSX_ACCEL WARNING: wl_shm fallback used - zero-copy not achieved\n")
#else
#define SSX_WARN_SHM_FALLBACK() ((void)0)
#endif

/*
 * Get acceleration layer statistics
 */
void ssx_accel_get_stats(struct ssx_accel_stats *stats);

/*
 * Reset statistics counters
 */
void ssx_accel_reset_stats(void);

/*
 * Compile-time check for io_uring support
 */
#if defined(__linux__) && defined(SQX_ACCEL_IO_URING)
#define SSX_HAS_IO_URING 1
#else
#define SSX_HAS_IO_URING 0
#endif

#endif /* SSX_ACCEL_H */
