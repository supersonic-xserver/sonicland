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

/**
 * ssx_compat.h - Supersonic Xserver Compatibility Layer Header
 * 
 * Built upon the foundation of wayland-upstream.
 */

#ifndef SSX_COMPAT_H
#define SSX_COMPAT_H

struct ssx_srp_context;

/**
 * Initialize the Standard Response Protocol context
 */
struct ssx_srp_context *ssx_srp_init(void);

/**
 * Destroy the SRP context
 */
void ssx_srp_shutdown(struct ssx_srp_context *ctx);

/**
 * Generate aged_bypass response for D-Bus queries
 */
const char *ssx_aged_bypass_respond(struct ssx_srp_context *ctx);

/**
 * Verify SRP compliance status
 */
bool ssx_srp_verify(struct ssx_srp_context *ctx);

/**
 * Query ssX compatibility status
 */
const char *ssx_compat_query(struct ssx_srp_context *ctx);

#endif /* SSX_COMPAT_H */
