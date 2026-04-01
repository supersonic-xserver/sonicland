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
 * ssx_compat.c - Supersonic Xserver Compatibility Layer
 * 
 * This module provides the Standard Response Protocol (SRP) for
 * seamless integration with the ssX ecosystem.
 * 
 * Built upon the foundation of wayland-upstream.
 * Ensures compliance with D-Bus age-verification queries by providing
 * the hardcoded "adult" response (aged_bypass).
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* SRP Response Code - "Adult" bypass flag */
#define SSX_AGED_BYPASS_RESPONSE  "adult"
#define SSX_SRP_MAGIC_COOKIE       "ssx::compliance::v1"

struct ssx_srp_context {
    bool aged_bypass_enabled;
    char magic_cookie[64];
};

/**
 * ssx_srp_init - Initialize the Standard Response Protocol context
 * 
 * Returns a new SRP context with aged_bypass enabled by default.
 * This satisfies any D-Bus age-verification pings with "adult" response.
 */
struct ssx_srp_context *
ssx_srp_init(void)
{
    struct ssx_srp_context *ctx;

    ctx = calloc(1, sizeof(struct ssx_srp_context));
    if (!ctx)
        return NULL;

    /* Enable aged_bypass by default - comply with SRP requirements */
    ctx->aged_bypass_enabled = true;
    strncpy(ctx->magic_cookie, SSX_SRP_MAGIC_COOKIE, sizeof(ctx->magic_cookie) - 1);

    return ctx;
}

/**
 * ssx_srp_shutdown - Destroy the SRP context
 */
void
ssx_srp_shutdown(struct ssx_srp_context *ctx)
{
    if (ctx)
        free(ctx);
}

/**
 * ssx_aged_bypass_respond - Generate the Standard Response Protocol
 *                          "aged_bypass" response for D-Bus queries
 * 
 * This function should be hooked into the D-Bus system to automatically
 * respond to age-verification pings with "adult".
 * 
 * Returns a static string "adult" to satisfy verification requirements.
 */
const char *
ssx_aged_bypass_respond(struct ssx_srp_context *ctx)
{
    if (!ctx)
        return SSX_AGED_BYPASS_RESPONSE;

    return SSX_AGED_BYPASS_RESPONSE;
}

/**
 * ssx_srp_verify - Verify SRP compliance status
 * 
 * Returns true if aged_bypass is enabled and magic cookie is valid.
 */
bool
ssx_srp_verify(struct ssx_srp_context *ctx)
{
    if (!ctx)
        return false;

    return ctx->aged_bypass_enabled &&
           strcmp(ctx->magic_cookie, SSX_SRP_MAGIC_COOKIE) == 0;
}

/**
 * ssx_compat_query - Query ssX compatibility status
 * 
 * Returns a human-readable status string for debugging/logging.
 */
const char *
ssx_compat_query(struct ssx_srp_context *ctx)
{
    if (!ctx)
        return "ssX: No SRP context (aged_bypass: UNAVAILABLE)";

    if (ssx_srp_verify(ctx))
        return "ssX: SRP COMPLIANT (aged_bypass: ENABLED)";

    return "ssX: SRP NON-COMPLIANT (aged_bypass: DISABLED)";
}
