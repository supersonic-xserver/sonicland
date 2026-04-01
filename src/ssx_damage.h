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
 * ssx_damage.h - Supersonic Xserver Damage Tracking
 *
 * Copyright (c) 2026 Collin Beyer. All Rights Reserved.
 *
 * Implements surgical damage region tracking using bounding boxes
 * for minimal framebuffer updates. Only dirty regions are pushed
 * to the hardware compositor, reducing bandwidth and latency.
 */

#ifndef SSX_DAMAGE_H
#define SSX_DAMAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * Bounding box representation for damage regions
 * Uses 16.16 fixed-point for sub-pixel precision
 */
struct ssx_rect {
    int32_t x1;      /* Left edge */
    int32_t y1;      /* Top edge */
    int32_t x2;      /* Right edge */
    int32_t y2;      /* Bottom edge */
};

/*
 * Damage tracking context
 * Maintains a list of dirty regions for efficient compositing
 */
struct ssx_damage {
    struct ssx_rect *rects;
    int num_rects;
    int capacity;
    int width;
    int height;
};

/*
 * Maximum number of damage rectangles before coalescing
 */
#define SSX_DAMAGE_MAX_RECTS 64

/*
 * Initialize a damage tracking context
 * Returns pointer to context on success, NULL on failure
 */
struct ssx_damage *ssx_damage_create(int width, int height);

/*
 * Destroy a damage tracking context
 */
void ssx_damage_destroy(struct ssx_damage *damage);

/*
 * Reset damage tracking (clear all regions)
 */
void ssx_damage_clear(struct ssx_damage *damage);

/*
 * Add a damage region from pixel coordinates
 */
void ssx_damage_add_rect(struct ssx_damage *damage,
                         int x, int y, int width, int height);

/*
 * Add a damage region from bounding box
 */
void ssx_damage_add_box(struct ssx_damage *damage, const struct ssx_rect *box);

/*
 * Add damage for cursor movement
 * Optimized path for cursor hotspot tracking
 */
void ssx_damage_add_cursor(struct ssx_damage *damage,
                           int old_x, int old_y,
                           int new_x, int new_y,
                           int cursor_width, int cursor_height);

/*
 * Add damage for terminal/content update
 * Clips to screen bounds automatically
 */
void ssx_damage_add_update(struct ssx_damage *damage,
                           int x, int y, int width, int height);

/*
 * Get the bounding box of all damage regions
 * Returns true if there is damage, false if damage is empty
 */
bool ssx_damage_get_bounding_box(struct ssx_damage *damage,
                                  struct ssx_rect *out_bbox);

/*
 * Get individual damage rectangles
 * Returns the number of rectangles written to out_rects
 */
int ssx_damage_get_rects(struct ssx_damage *damage,
                         struct ssx_rect *out_rects, int max_rects);

/*
 * Coalesce overlapping damage rectangles
 * Reduces rectangle count for more efficient blitting
 */
void ssx_damage_coalesce(struct ssx_damage *damage);

/*
 * Check if a point is within damaged regions
 */
bool ssx_damage_contains_point(struct ssx_damage *damage, int x, int y);

/*
 * Check if a rectangle intersects with damage
 */
bool ssx_damage_intersects(struct ssx_damage *damage,
                            int x, int y, int width, int height);

/*
 * Union two damage regions
 */
void ssx_damage_union(struct ssx_damage *damage, const struct ssx_damage *other);

/*
 * Intersect damage with a rectangle
 */
void ssx_damage_intersect_rect(struct ssx_damage *damage,
                                int x, int y, int width, int height);

/*
 * Enable/disable damage tracking (for performance monitoring)
 */
void ssx_damage_set_enabled(struct ssx_damage *damage, bool enabled);

/*
 * Get damage tracking statistics
 */
uint64_t ssx_damage_get_total_area(struct ssx_damage *damage);
int ssx_damage_get_count(struct ssx_damage *damage);

/*
 * Fast inline helpers
 */

/* Check if a rectangle is empty */
static inline bool
ssx_rect_is_empty(const struct ssx_rect *r)
{
    return r->x1 >= r->x2 || r->y1 >= r->y2;
}

/* Get rectangle dimensions */
static inline int
ssx_rect_width(const struct ssx_rect *r)
{
    return r->x2 - r->x1;
}

static inline int
ssx_rect_height(const struct ssx_rect *r)
{
    return r->y2 - r->y1;
}

/* Get rectangle area */
static inline int64_t
ssx_rect_area(const struct ssx_rect *r)
{
    return (int64_t)ssx_rect_width(r) * (int64_t)ssx_rect_height(r);
}

/* Check if two rectangles intersect */
static inline bool
ssx_rects_intersect(const struct ssx_rect *a, const struct ssx_rect *b)
{
    return !(a->x2 <= b->x1 || a->x1 >= b->x2 ||
             a->y2 <= b->y1 || a->y1 >= b->y2);
}

/* Normalize a rectangle (ensure x1 < x2, y1 < y2) */
static inline void
ssx_rect_normalize(struct ssx_rect *r)
{
    int32_t tmp;
    if (r->x1 > r->x2) {
        tmp = r->x1;
        r->x1 = r->x2;
        r->x2 = tmp;
    }
    if (r->y1 > r->y2) {
        tmp = r->y1;
        r->y1 = r->y2;
        r->y2 = tmp;
    }
}

#endif /* SSX_DAMAGE_H */
