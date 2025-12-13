# Hot Reload Investigation for JusPrin

## Executive Summary

True "Hot Reload" (changing code and seeing results without restarting the application) is challenging in a C++ application like JusPrin. However, different parts of the stack offer different capabilities.

| Component | Feasibility | Method | Recommendation |
|-----------|-------------|--------|----------------|
| **C++ Core** | Low | DLL Injection / Third-party libs | **Focus on Fast Builds** |
| **Python Scripts** | High | Module Reloading | **Implement Watcher** |
| **Web UI** | High | Browser Refresh | **Standard Web Dev** |
| **wxWidgets UI** | Low | None native | **Use resource files** |

## C++ Core Strategies

### 1. Incremental Build Optimization (Recommended)
Instead of dynamic reloading, minimize the "Feedback Loop" time.
-   **ccache**: We have enabled `ccache` in the Docker environment. This caches compilation artifacts.
-   **Ninja**: Use `Ninja` generator (default in our setup) for fastest dependency resolution.
-   **Precompiled Headers (PCH)**: CMake is configured to use PCH (`SLIC3R_PCH=ON`), significantly reducing compile times for common headers.

### 2. Runtime Dynamic Linking (Experimental)
Technically, we could architecture the app to load "features" as plugin DLLs/SOs.
-   **Pros**: Allows reloading specific modules.
-   **Cons**: Requires massive refactoring of the monolithic architecture. **Not recommended** for JusPrin at this stage.

### 3. Third-party Tools
-   **Live++** (Windows only): Commercial tool, very powerful.
-   **cr.h**: A single-header C++ hot reload library. Requires structuring the main loop specifically for it.
    -   *Verdict*: Too intrusive for current codebase.

## Python Scripts (Post-Processing/Plugins)

JusPrin uses Python for post-processing scripts.
-   **Current State**: Scripts are loaded on demand or at startup.
-   **Recommendation**: Development workflows should trigger script re-execution rather than app restarts. Since scripts are often spawned as subprocesses or interpreted by an embedded interpreter, modifying the `.py` file is usually picked up on the next execution automatically.

## Web Interface

The `resources/web` components are standard HTML/JS.
-   **Workflow**: Edit files -> Refresh Browser (or embedded WebView).
-   **Optimization**: Use `livereload` or similar node.js tools to auto-refresh the browser when files change.

## Conclusion

For C++ development in JusPrin, the cost/benefit ratio of implementing true hot reload is poor. **We recommend focusing on optimizing build times**:
1.  Use the provided **Docker Dev Environment** with `ccache`.
2.  Use **Ninja** build system.
3.  Keep headers clean to avoid massive cascading rebuilds.

For Python and Web components, standard interpreted language benefits apply automatically.
