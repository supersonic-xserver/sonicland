/*
 * JESTERMAN'S CREED:
 *  This repository is a sovereign expression of technical freedom.
 *  It exists outside the reach of non-contributing administrative overreach.
 *  The creator's intent is the absolute law of this tree.
 *
 *  PROJECT: sonicland (ssX Core)
 *  DRM Modeline Injection Hijack Module
 *  LICENSE: ssX Supplemental License (see LICENSE at project root)
 *  COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED
 *
 *  This module provides LD_PRELOAD-style interception of libdrm's
 *  drmModeGetConnector to inject custom modelines (custom display timings)
 *  into any compositor using libwayland-server.so.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <xf86drmMode.h>
#include <drm_mode.h>

/* Environment variable for custom modeline specification
 * Format: SONICLAND_MODELINE="<clock> <hdisplay> <hsyncstart> <hsyncend> <htotal> <vdisplay> <vsyncstart> <vsyncend> <vtotal> [flags]"
 * Example: SONICLAND_MODELINE="354000 2560 2568 2600 2664 1440 1443 1448 1518 0"
 */
#define MODELINE_ENV_VAR "SONICLAND_MODELINE"

/* Static storage for the custom mode */
static drmModeModeInfo custom_mode;
static bool custom_mode_parsed = false;

/*
 * Parse a modeline string from the environment variable
 * Format: clock hdisplay hsyncstart hsyncend htotal vdisplay vsyncstart vsyncend vtotal [flags]
 * All values except flags are integers
 */
static void parse_modeline_from_env(void)
{
	const char *modeline_str;
	char *env_copy;
	char *token;
	char *saveptr;
	int i = 0;
	uint32_t values[10];
	uint32_t flags = DRM_MODE_TYPE_USERDEF;

	if (custom_mode_parsed)
		return;

	modeline_str = getenv(MODELINE_ENV_VAR);
	if (!modeline_str) {
		custom_mode_parsed = false;
		return;
	}

	/* Make a copy since strtok modifies the string */
	env_copy = strdup(modeline_str);
	if (!env_copy)
		return;

	/* Parse all numeric values */
	token = strtok(env_copy, " \t\n");
	while (token != NULL && i < 9) {
		values[i] = (uint32_t)atoi(token);
		i++;
		token = strtok(NULL, " \t\n");
	}

	/* Check if we got all required values (at least 9) */
	if (i < 9) {
		free(env_copy);
		custom_mode_parsed = false;
		return;
	}

	/* Parse optional flags if present */
	if (token != NULL) {
		flags = (uint32_t)atoi(token);
	} else {
		flags = DRM_MODE_TYPE_USERDEF;
	}

	/* Construct the drmModeModeInfo struct */
	memset(&custom_mode, 0, sizeof(drmModeModeInfo));

	custom_mode.clock = values[0];
	custom_mode.hdisplay = values[1];
	custom_mode.hsyncstart = values[2];
	custom_mode.hsyncend = values[3];
	custom_mode.htotal = values[4];
	custom_mode.vdisplay = values[5];
	custom_mode.vsyncstart = values[6];
	custom_mode.vsyncend = values[7];
	custom_mode.vtotal = values[8];
	custom_mode.flags = flags;
	custom_mode.type = DRM_MODE_TYPE_USERDEF;

	/* Generate a name for the mode */
	snprintf(custom_mode.name, sizeof(custom_mode.name),
		 "%ux%u@%u", custom_mode.hdisplay, custom_mode.vdisplay,
		 (custom_mode.clock * 1000) / (custom_mode.htotal * custom_mode.vtotal));

	free(env_copy);
	custom_mode_parsed = true;

	fprintf(stderr, "[sonicland:drm-hijack] Custom modeline injected: %s\n",
		custom_mode.name);
	fprintf(stderr, "[sonicland:drm-hijack]   Clock: %u kHz\n", custom_mode.clock);
	fprintf(stderr, "[sonicland:drm-hijack]   H: %u %u %u %u\n",
		custom_mode.hdisplay, custom_mode.hsyncstart, custom_mode.hsyncend, custom_mode.htotal);
	fprintf(stderr, "[sonicland:drm-hijack]   V: %u %u %u %u\n",
		custom_mode.vdisplay, custom_mode.vsyncstart, custom_mode.vsyncend, custom_mode.vtotal);
}

/*
 * Hook for drmModeGetConnector - injects custom modelines into the connector's mode list
 */
drmModeConnectorPtr drmModeGetConnector(int fd, uint32_t connectorId)
{
	static drmModeConnectorPtr (*real_drmModeGetConnector)(int, uint32_t) = NULL;
	drmModeConnectorPtr connector;
	drmModeModeInfoPtr new_modes;
	int i;

	/* Get the real function if not already resolved */
	if (!real_drmModeGetConnector) {
		real_drmModeGetConnector = dlsym(RTLD_NEXT, "drmModeGetConnector");
		if (!real_drmModeGetConnector) {
			fprintf(stderr, "[sonicland:drm-hijack] Error: dlsym failed for drmModeGetConnector\n");
			return NULL;
		}
	}

	/* Parse modeline from environment on first call */
	if (!custom_mode_parsed) {
		parse_modeline_from_env();
	}

	/* Call the real function */
	connector = real_drmModeGetConnector(fd, connectorId);
	if (!connector) {
		return NULL;
	}

	/* If we have a custom mode and the connector has modes, inject it */
	if (custom_mode_parsed && connector->count_modes > 0) {
		/* Allocate new array with space for custom mode */
		new_modes = malloc(sizeof(drmModeModeInfo) * (connector->count_modes + 1));
		if (!new_modes) {
			fprintf(stderr, "[sonicland:drm-hijack] Warning: Failed to allocate mode array\n");
			return connector;
		}

		/* Copy existing modes */
		memcpy(new_modes, connector->modes, sizeof(drmModeModeInfo) * connector->count_modes);

		/* Add our custom mode at the end */
		memcpy(&new_modes[connector->count_modes], &custom_mode, sizeof(drmModeModeInfo));

		/* Free old modes array and replace with new one */
		free(connector->modes);
		connector->modes = new_modes;
		connector->count_modes++;

		fprintf(stderr, "[sonicland:drm-hijack] Injected custom mode into connector %u (now %d modes)\n",
			connectorId, connector->count_modes);
	}

	return connector;
}

/*
 * Additional hook for drmModeGetConnectors - intercept array version
 * This ensures the hijack works regardless of which connector retrieval method is used
 */
int drmModeGetConnectors(int fd, drmModeConnectorPtr **connectors, int *count)
{
	static int (*real_drmModeGetConnectors)(int, drmModeConnectorPtr **, int *) = NULL;
	int ret;
	int i;

	/* Get the real function if not already resolved */
	if (!real_drmModeGetConnectors) {
		real_drmModeGetConnectors = dlsym(RTLD_NEXT, "drmModeGetConnectors");
		if (!real_drmModeGetConnectors) {
			/* Function might not exist in older libdrm versions */
			return -1;
		}
	}

	/* Parse modeline from environment on first call */
	if (!custom_mode_parsed) {
		parse_modeline_from_env();
	}

	ret = real_drmModeGetConnectors(fd, connectors, count);
	if (ret != 0 || !*connectors || *count <= 0) {
		return ret;
	}

	/* Inject custom mode into all connectors that have modes */
	if (custom_mode_parsed) {
		for (i = 0; i < *count; i++) {
			drmModeConnectorPtr conn = (*connectors)[i];
			drmModeModeInfoPtr new_modes;

			if (conn && conn->count_modes > 0) {
				new_modes = malloc(sizeof(drmModeModeInfo) * (conn->count_modes + 1));
				if (new_modes) {
					memcpy(new_modes, conn->modes, sizeof(drmModeModeInfo) * conn->count_modes);
					memcpy(&new_modes[conn->count_modes], &custom_mode, sizeof(drmModeModeInfo));
					free(conn->modes);
					conn->modes = new_modes;
					conn->count_modes++;
				}
			}
		}
	}

	return ret;
}