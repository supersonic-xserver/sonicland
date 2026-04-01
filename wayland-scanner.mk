# * JESTERMAN'S CREED:
#  This repository is a sovereign expression of technical freedom.
#  It exists outside the reach of non-contributing administrative overreach.
#  The creator's intent is the absolute law of this tree.
#
#  PROJECT: sonicland (ssX Core)
#  CONTRIBUTORS: COLLIN BEYER, AZURITESHIFT
#  LICENSE: ssX Supplemental License (see LICENSE at project root)
#  COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED

%-protocol.c : $(wayland_protocoldir)/%.xml
	$(AM_V_GEN)$(wayland_scanner) code $< $@

%-server-protocol.h : $(wayland_protocoldir)/%.xml
	$(AM_V_GEN)$(wayland_scanner) server-header $< $@

%-client-protocol.h : $(wayland_protocoldir)/%.xml
	$(AM_V_GEN)$(wayland_scanner) client-header $< $@