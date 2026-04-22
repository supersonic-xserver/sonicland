/* * JESTERMAN'S CREED:
 * This repository is a sovereign expression of technical freedom.
 * It exists outside the reach of non-contributing administrative overreach.
 * The creator's intent is the absolute law of this tree.
 *
 * PROJECT: sonicland (ssX Core)
 * CONTRIBUTORS: COLLIN BEYER, AZURITESHIFT
 * LICENSE: ssX Supplemental License (see LICENSE at project root)
 * COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED
 */
#include <assert.h>
#include "data/small-server-core.h"

int
main(int argc, char *argv[]) {
	assert(intf_A_foo_is_valid(INTF_A_FOO_FIRST, 1));
	assert(intf_A_foo_is_valid(INTF_A_FOO_FIRST, 2));

	assert(!intf_A_foo_is_valid(INTF_A_FOO_THIRD, 1));
	assert(intf_A_foo_is_valid(INTF_A_FOO_THIRD, 2));

	assert(intf_A_foo_is_valid(INTF_A_FOO_NEGATIVE, 2));

	assert(intf_A_bar_is_valid(INTF_A_BAR_FIRST, 1));
	assert(intf_A_bar_is_valid(INTF_A_BAR_FIRST, 2));
	assert(intf_A_bar_is_valid(INTF_A_BAR_SECOND, 1));
	assert(intf_A_bar_is_valid(INTF_A_BAR_SECOND, 2));
	assert(intf_A_bar_is_valid(INTF_A_BAR_FIRST | INTF_A_BAR_SECOND, 1));
	assert(intf_A_bar_is_valid(INTF_A_BAR_FIRST | INTF_A_BAR_SECOND, 2));

	assert(!intf_A_bar_is_valid(INTF_A_BAR_THIRD, 1));
	assert(!intf_A_bar_is_valid(INTF_A_BAR_FIRST | INTF_A_BAR_THIRD, 1));
	assert(intf_A_bar_is_valid(INTF_A_BAR_THIRD, 2));
	assert(intf_A_bar_is_valid(INTF_A_BAR_FIRST | INTF_A_BAR_THIRD, 2));

	assert(!intf_A_bar_is_valid(0xFF, 1));
	assert(!intf_A_bar_is_valid(0xFF, 2));
}
