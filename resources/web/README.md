# JusPrin Web Interface

This directory contains the web-based interface components for JusPrin.

> [!NOTE]
> The web interface is part of the JusPrin user experience but currently serves primarily as informational pages rather than the main application interface.

## Directory Structure

```
resources/web/
├── jusprin/          # Main web UI (TailwindCSS-based)
├── homepage/         # Landing page
├── guide/            # User guides and documentation
├── login/            # Login interface
├── model/            # 3D model previews/handling
├── orca/             # OrcaSlicer legacy assets
├── image/            # Image assets
├── include/          # Shared includes
└── data/             # Configuration data
```

## JusPrin Web UI (`jusprin/`)

### Purpose

Provides a modern, responsive web interface using **TailwindCSS 3.4.14** for:
- Application information pages
- User onboarding
- Possibly embedded views within the desktop application

### Build System

The `jusprin/` subdirectory uses NPM and TailwindCSS:

```json
{
  "name": "jusprin",
  "version": "0.1.0",
  "scripts": {
    "build": "tailwindcss build styles.css -o css/jusprin.css",
    "dev": "tailwindcss build styles.css -o css/jusprin.css --watch"
  },
  "devDependencies": {
    "tailwindcss": "^3.4.14"
  }
}
```

### Setup and Build

#### Prerequisites
- Node.js 16+ and NPM

#### Installation
```bash
cd resources/web/jusprin
npm install
```

#### Development
```bash
# Watch mode - rebuilds on file changes
npm run dev
```

#### Production Build
```bash
# One-time build with optimization
npm run build
```

> [!WARNING]
> Current build script does NOT minify or purge unused CSS. For production, consider:
> ```json
> "build": "tailwindcss -i styles.css -o css/jusprin.css --minify"
> ```

### Configuration

#### Current State
The web UI uses **TailwindCSS defaults** (no `tailwind.config.js` file).

#### Recommended Configuration

Create `tail wind.config.js` to customize the design system:

```javascript
/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./**/*.html",
    "./**/*.js",
    "../**/*.html", // Include other web directories if needed
  ],
  theme: {
    extend: {
      colors: {
        'jusprin-blue': '#3B82F6',
        'jusprin-dark': '#1E293B',
      },
      fontFamily: {
        sans: ['Inter', 'system-ui', 'sans-serif'],
      },
    },
  },
  plugins: [],
}
```

### File Structure

```
jusprin/
├── package.json           # NPM configuration
├── package-lock.json      # Dependency lock file
├── styles.css             # Source Tailwind CSS (input)
├── css/
│   └── jusprin.css       # Compiled CSS (output)
├── *.html                # HTML pages
└── *.js                  # JavaScript files
```

---

## Homepage (`homepage/`)

Landing page for JusPrin showcasing:
- GenAI features (note: not yet implemented in backend, see [AI_ARCHITECTURE.md](file:///home/yurix/Documentos/JusPrin/doc/AI_ARCHITECTURE.md))
- Download links
- Feature highlights

**File:** `index.html`

---

## Guides (`guide/`)

User documentation and tutorials accessible via web interface:
- Getting started guides
- Feature explanations
- Tips and tricks

**Contents:** 301 files (extensive documentation)

---

## How the Web UI Integrates with the Desktop App

### Embedded Web Views

JusPrin (C++ application) can display web content using:

1. **WebView2 (Windows)**
   - Uses Edge WebView2 control
   - DLL: `deps/WebView2/lib/win-{arch}/WebView2Loader.dll`
   - Loads local HTML files or remote URLs

2. **WebKit (macOS/Linux)**
   - Platform-native web rendering
   - Can embed HTML/CSS/JS within wxWidgets windows

### Typical Use Cases

- **Onboarding Flow**: Show `homepage/index.html` on first launch
- **In-App Help**: Display `guide/` pages within help panel
- **Settings UI**: Use web tech for complex settings (faster iteration than wxWidgets)

---

## Development Workflow

### Local Testing

1. **Build the CSS:**
   ```bash
   cd resources/web/jusprin
   npm run build
   ```

2. **Open in Browser:**
   ```bash
   # macOS
   open index.html
   
   # Linux
   xdg-open index.html
   
   # Windows
   start index.html
   ```

3. **Test in Desktop App:**
   - Rebuild JusPrin C++ application
   - Web views will load from `resources/web/`

### Hot Reload (Development)

For faster development, use `npm run dev` in one terminal while editing HTML/CSS in another. The CSS will auto-rebuild on changes.

> [!TIP]
> For true hot reload (browser auto-refresh), consider using a simple HTTP server:
> ```bash
> # Python
> python3 -m http.server 8000
> 
> # Node.js
> npx http-server -p 8000
> 
> # Then visit: http://localhost:8000
> ```

---

## Styling Guidelines

### Using Tailwind utility Classes

```html
<!-- Good: Using Tailwind utilities -->
<div class="flex items-center gap-4 px-6 py-4 bg-blue-500 text-white rounded-lg shadow-md">
  <h1 class="text-2xl font-bold">Welcome to JusPrin</h1>
</div>

<!-- Bad: Inline styles or custom CSS (defeats Tailwind purpose) -->
<div style="display: flex; padding: 16px;">
  <h1>Welcome to JusPrin</h1>
</div>
```

### Responsive Design

Always design mobile-first, then add larger breakpoints:

```html
<!-- Mobile: stack vertically -->
<!-- Tablet (md): 2 columns -->
<!-- Desktop (lg): 3 columns -->
<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
  <div class="card">...</div>
  <div class="card">...</div>
  <div class="card">...</div>
</div>
```

###  Dark Mode Support

If implementing dark mode (recommended for desktop app integration):

```html
<body class="bg-white dark:bg-gray-900 text-gray-900 dark:text-white">
  <div class="bg-gray-100 dark:bg-gray-800 p-4">
    Content adapts to system dark mode
  </div>
</body>
```

Enable in `tailwind.config.js`:
```javascript
module.exports = {
  darkMode: 'class', // or 'media' for automatic
  // ...
}
```

---

## Optimization for Production

### 1. Minimize CSS File Size

Current build produces large CSS (~3MB) because it includes ALL Tailwind utilities.

**Solution: PurgeCSS**

Update `tailwind.config.js`:
```javascript
module.exports = {
  content: ["./**/*.html", "./**/*.js"], // Auto-purges unused styles
  // ...
}
```

Then build:
```bash
npm run build -- --minify
```

**Result:** CSS shrinks to ~10-50KB (97%+ reduction)

### 2. Bundle JavaScript (if applicable)

If using JS modules, bundle with:
- [esbuild](https://esbuild.github.io/) (fastest)
- [Rollup](https://rollupjs.org/)
- [Parcel](https://parceljs.org/) (zero-config)

### 3. Image Optimization

- Use WebP format for images
- Compress PNGs with `pngquant`
- Lazy-load images: `<img loading="lazy" ...>`

---

## Integration with C++ Backend

### Loading Web Pages from C++

Example using WebView2 (Windows):

```cpp
// In src/slic3r/GUI/SomeDialog.cpp
#include <WebView2.h>

void ShowWebGuide() {
    // Get path to resources
    auto resource_dir = Slic3r::resources_dir();
    auto guide_path = resource_dir + "/web/guide/index.html";
    
    // Load in WebView2 control
    webview->Navigate(guide_path);
}
```

### Passing Data to Web UI

**Option A: URL Parameters**
```cpp
std::string url = "file://" + guide_path + "?printer=xyz&material=pla";
webview->Navigate(url);
```

Then in JavaScript:
```javascript
const params = new URLSearchParams(window.location.search);
const printer = params.get('printer'); // "xyz"
```

**Option B: JavaScript Bridge**
```cpp
// From C++ to JavaScript
webview->ExecuteScript("setPrinterName('XYZ Printer')");
```

```javascript
// JavaScript side
function setPrinterName(name) {
    document.getElementById('printer').textContent = name;
}
```

**Option C: WebSocket/IPC**

For complex bidirectional communication:
- C++ runs WebSocket server on localhost
- Web UI connects via `ws://localhost:PORT`
- Send JSON messages back and forth

---

## FAQ

### Q: Can I use React/Vue/Svelte instead of plain HTML?

**A:** Yes, but:
- Adds build complexity (webpack/vite required)
- Increases bundle size
- May complicate offline functionality
- For simple pages, Tailwind + vanilla JS is sufficient

If you need a framework, **Svelte** or **Preact** are lightweight options.

### Q: How do I debug web content in the desktop app?

**A:** WebView2 (Windows) supports remote debugging:

1. Launch app with flag: `--remote-debugging-port=9222`
2. Open Chrome: `chrome://inspect`
3. Connect to localhost:9222
4. Inspect the embedded web view

### Q: Can the web UI work offline?

**A:** Yes, since it's bundled with the application:
- All HTML/CSS/JS files are local
- No CDN dependencies (Tailwind is built, not loaded from CDN)
- Images and fonts included in `resources/`

### Q: What about accessibility?

**A:** Best practices:
- Use semantic HTML (`<nav>`, `<main>`, `<button>`)
- Include ARIA labels where needed
- Ensure keyboard navigation works
- Test with screen readers
- Maintain color contrast ratios (WCAG AA: 4.5:1)

---

## Contributing

### Adding a New Page

1. Create `new-page.html` in appropriate directory
2. Import Tailwind CSS:
   ```html
   <link rel="stylesheet" href="../jusprin/css/jusprin.css" />
   ```
3. Follow existing page structure
4. Add navigation links from other pages

### Updating Styles

1. Edit `jusprin/styles.css` (Tailwind directives)
2. Or customize `tailwind.config.js` (theme)
3. Rebuild: `npm run build`
4. Test across all pages

### Testing Checklist

- [ ] Renders correctly in Chrome/Edge
- [ ] Renders correctly in Firefox
- [ ] Renders correctly in Safari (macOS)
- [ ] Works when loaded as `file://` (not just `http://`)
- [ ] Responsive on mobile/tablet/desktop
- [ ] Dark mode (if implemented) looks good
- [  ] No console errors

---

## Related Documentation

- [AI_ARCHITECTURE.md](file:///home/yurix/Documentos/JusPrin/doc/AI_ARCHITECTURE.md) - Backend AI features (note: not yet implemented)
- [How to Build](file:///home/yurix/Documentos/JusPrin/doc/developer-reference/How-to-build.md) - Building the C++ application
- [Tailwind CSS Docs](https://tailwindcss.com/docs) - Official Tailwind documentation

---

**Last Updated:** 2025-12-10  
**Maintainer:** JusPrin Development Team  
**Questions?** Open an issue on GitHub or email k@obico.io
