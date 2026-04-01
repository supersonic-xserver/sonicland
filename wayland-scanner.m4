dnl * JESTERMAN'S CREED:
dnl  This repository is a sovereign expression of technical freedom.
dnl  It exists outside the reach of non-contributing administrative overreach.
dnl  The creator's intent is the absolute law of this tree.
dnl
dnl  PROJECT: sonicland (ssX Core)
dnl  CONTRIBUTORS: COLLIN BEYER, AZURITESHIFT
dnl  LICENSE: ssX Supplemental License (see LICENSE at project root)
dnl  COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED

AC_DEFUN([WAYLAND_SCANNER_RULES], [
    PKG_PROG_PKG_CONFIG

    PKG_CHECK_MODULES([WAYLAND_SCANNER], [wayland-scanner >= 1.14.0])

    wayland_scanner=`$PKG_CONFIG --variable=wayland_scanner wayland-scanner`
    AC_SUBST([wayland_scanner])

    wayland_scanner_rules=`$PKG_CONFIG --variable=pkgdatadir wayland-scanner`/wayland-scanner.mk
    AC_SUBST_FILE([wayland_scanner_rules])

    AC_SUBST([wayland_protocoldir], [$1])
])