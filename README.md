# ⚡ sonicland (ssX Core)

The High-Performance, Sovereign Wayland Compositor for the ssX Ecosystem.

sonicland is a "Sonic-Clean" fork of the Wayland compositor, optimized for the sonicd init system and surgically integrated with the XLibre 2D-acceleration pipe via xsonicland.

---

## 🛡️ JESTERMAN'S CREED

This repository is a sovereign expression of technical freedom. It exists outside the reach of non-contributing censors and administrative overreach. The creator's intent is the absolute law of this tree. Unauthorized interference will be met with immediate history redaction.

---

## 🚀 CORE FEATURES

- **sonicd Ready**: Native READY=1 signaling for sub-millisecond boot orchestration.
- **ssX-Compat**: Optimized hardware shims for xsonicland high-speed 2D rendering.
- **Latency-First**: VSync-bypass by default; TearFree optional.
- **Pure Upstream**: Synced directly with wayland-upstream to ensure zero bloat.

---

## 🔧 Building

sonicland uses the Meson build system:

    $ meson build/ --prefix=PREFIX
    $ ninja -C build/ install

where PREFIX is where you want to install the libraries.

---

## 📚 Documentation

See https://wayland.freedesktop.org for Wayland protocol documentation.
