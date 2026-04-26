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

## Custom Modeline Injection

sonicland provides a DRM modeline injection mechanism via the `SONICLAND_MODELINE` environment variable. This enables monitor overclocking by injecting custom display timings into any compositor using libdrm.

### Environment Variable

The `SONICLAND_MODELINE` variable specifies custom display timings in the following format:

```
clock hdisp hss hse htot vdisp vss vse vtot flags
```

| Field    | Description                                      |
|----------|--------------------------------------------------|
| clock    | Pixel clock in kHz                               |
| hdisp    | Horizontal display width                        |
| hss      | Horizontal sync start                           |
| hse      | Horizontal sync end                            |
| htot     | Horizontal total (total scanline width)         |
| vdisp    | Vertical display height                        |
| vss      | Vertical sync start                           |
| vse      | Vertical sync end                             |
| vtot     | Vertical total (total scanline height)        |
| flags    | DRM mode flags (optional, defaults to user-defined)|

### Usage Example

Generate timings using the cvt tool, then pass to a compositor:

```sh
SONICLAND_MODELINE="173.00 1920 2048 2248 2576 1080 1083 1088 1120 -hsync +vsync" gamescope
```

Or for a 1440p@144Hz mode:

```sh
SONICLAND_MODELINE="354000 2560 2568 2600 2664 1440 1443 1448 1518 0" gamescope
```

### Architecture

The injection is implemented in `src/drm-hijack.c` using dlsym-based function interposition. The module hooks `drmModeGetConnector` and `drmModeGetConnectors` from libdrm, parsing the environment variable on first use and injecting the custom mode into each connector's mode list. This provides a global override for any compositor using libdrm, as long as sonicland's DRM layer is loaded first.

---

## 🔧 Building

sonicland uses the Meson build system:

    $ meson build/ --prefix=PREFIX
    $ ninja -C build/ install

where PREFIX is where you want to install the libraries.

---

## 📚 Documentation

See https://wayland.freedesktop.org for Wayland protocol documentation.
