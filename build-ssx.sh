#!/bin/bash
# * JESTERMAN'S CREED:
#  This repository is a sovereign expression of technical freedom.
#  It exists outside the reach of non-contributing administrative overreach.
#  The creator's intent is the absolute law of this tree.
#
#  PROJECT: sonicland (ssX Core)
#  CONTRIBUTORS: COLLIN BEYER, AZURITESHIFT
#  LICENSE: ssX Supplemental License (see LICENSE at project root)
#  COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED
#
# build-ssx.sh - Sonicland High-Performance Build Script
#
# Dynamic thread detection and generic optimization flags for
# sub-millisecond input-to-photon latency builds.
#

set -e

# SSX-DYNAMIC-SCALING-ORCHESTRATOR: Auto-detect available threads
# Detect available processing units, fallback to 2 if detection fails
THREADS=$(nproc 2>/dev/null || echo 2)

echo "🚀 Detected $THREADS available threads for compilation..."
echo "🔧 Configuring optimized build with Link-Time Optimization..."

# Configure with LTO enabled for whole-program optimization
# -Db_lto=true enables Link Time Optimization
# -Dbuildtype=release ensures -O3 optimizations are active
meson setup build \
    -Dlibraries=true \
    -Dscanner=true \
    -Dtests=true \
    -Db_lto=true \
    -Dbuildtype=release \
    || meson setup build --wipe \
    -Dlibraries=true \
    -Dscanner=true \
    -Dtests=true \
    -Db_lto=true \
    -Dbuildtype=release

echo "⚡ Compiling with $THREADS threads..."

# Build with detected thread count
ninja -C build -j "$THREADS"

echo "✅ Build complete!"
echo ""
echo "📊 Build Configuration:"
echo "   - Threads: $THREADS"
echo "   - LTO: Enabled"
echo "   - Optimization: -O3 with -ffast-math"
echo "   - Architecture: Native CPU optimization"
