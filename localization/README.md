# Internationalization (i18n) Guide

## Overview

JusPrin uses wxWidgets' i18n system with gettext for translations. This document describes the current state and best practices.

## Current Status

### ✅ Supported Languages

- English (en) - Default
- Chinese (zh_CN) - Partial
- Portuguese (pt_BR) - Planned
- Spanish (es) - Planned
- German (de) - Planned
- French (fr) - Planned

### 📁 Directory Structure

```
localization/
├── i18n/
│   ├── list.txt          # List of languages
│   ├── JusPrin.pot       # Translation template
│   ├── zh_CN/
│   │   └── JusPrin.po    # Chinese translations
│   └── ...
└── README.md             # This file
```

---

## For Developers

### Marking Strings for Translation

Use the `_L()` macro for user-facing strings:

```cpp
// ✅ Good - translatable
auto message = _L("Print completed successfully");
wxMessageBox(_L("Invalid input"), _L("Error"), wxOK | wxICON_ERROR);

// ❌ Bad - not translatable
auto message = "Print completed successfully";  // English only
wxMessageBox("Invalid input", "Error", wxOK);   // English only
```

### String Formatting

For dynamic content, use `wxString::Format` or Boost.Format:

```cpp
// Simple formatting
auto msg = wxString::Format(_L("Loaded %d objects"), count);

// Complex formatting with Boost
auto msg = from_u8((boost::format(_L("Progress: %1$.1f%%")) % progress).str());
```

### Plurals

Handle plurals correctly:

```cpp
// ✅ Good
auto msg = count == 1 
    ? _L("1 object selected")
    : wxString::Format(_L("%d objects selected"), count);

// ❌ Bad
auto msg = wxString::Format(_L("%d object(s) selected"), count);
```

### Context-Specific Translations

Use context markers when the same English word has different meanings:

```cpp
// "File" as in document
_L("File");

// "File" as in tool/rasp - use comment
/* i18n: file tool, not document */
_L("File");
```

---

## For Translators

### Getting Started

1. **Install Poedit** (recommended) or any PO editor
2. **Open** `localization/i18n/JusPrin.pot`
3. **Create** new language: `File > New from POT file`
4. **Save as** `localization/i18n/<lang_code>/JusPrin.po`

### Translation Guidelines

#### ✅ DO

- Preserve formatting placeholders: `%d`, `%s`, `%1%`, etc.
- Maintain consistency in terminology
- Test translations in the application
- Use formal/informal tone consistently
- Translate error messages clearly

#### ❌ DON'T

- Change placeholder order without adjusting format strings
- Translate technical terms inconsistently
- Make translations significantly longer than originals (UI space issues)
- Include English in translations unnecessarily

### Example Translation

**English (POT)**:
```po
msgid "Print completed in %1% minutes"
msgstr ""
```

**Portuguese (PO)**:
```po
msgid "Print completed in %1% minutes"
msgstr "Impressão concluída em %1% minutos"
```

---

## P2.3 Improvements

### Completed

✅ **Removed untranslated Chinese comments** from C++ source files  
✅ **Created this i18n guide**  
✅ **Identified hardcoded English strings** (see "Audit Results" below)

### Pending

⬜ **Set up Weblate/Crowdin** for community translations  
⬜ **Create translation memory** from existing translations  
⬜ **Add translation coverage CI check**

---

## Hardcoded Strings Audit

### High Priority (User-Facing)

Found ~50 hardcoded English strings in GUI code. Examples:

**File**: `src/slic3r/GUI/Field.cpp`
```cpp
// Line 393 - Should use _L()
show_error(parent, "Invalid numeric input.");
```

**File**: `src/slic3r/GUI/CalibUtils.cpp`
```cpp
// Line 241 - Should use _L()
MessageDialog msg_dlg(nullptr, "Please input valid numeric value", "", wxOK);
```

**Action Required**: Convert these to `_L()` macro usage.

### Medium Priority (Error Messages)

**File**: `src/libslic3r/Format/3mf.cpp`
```cpp
// Line 5665 - Error message should be translated
add_error("Unable to load mesh for object: " + object.name);
```

**Action Required**: Use `_L()` for error messages where applicable.

### Low Priority (Internal/Debug)

Debug messages and internal logs can remain in English for consistency across international developer teams.

---

## Translation Workflow (Planned)

```mermaid
graph LR
    Dev[Developer] -->|Mark strings| Code[C++ Code]
    Code -->|Extract| POT[Template .pot]
    POT -->|Distribute| Translators[Translators]
    Translators -->|Translate| PO[Language .po files]
    PO -->|Compile| MO[Binary .mo files]
    MO -->|Bundle| App[JusPrin App]
    
    App -->|Feedback| Translators
    
    style POT fill:#bbf
    style MO fill:#bfb
```

### Steps

1. **Extract** strings: `python scripts/extract_i18n.py` (to be created)
2. **Upload** POT to Weblate/Crowdin
3. **Translate** via web interface
4. **Download** updated PO files
5. **Compile** to MO: `msgfmt -o JusPrin.mo JusPrin.po`
6. **Test** in application

---

## Testing Translations

### Command Line

```bash
# Set language via environment variable
export LANG=pt_BR.UTF-8
./build/src/jusprin

# Or for testing
LANG=zh_CN.UTF-8 ./build/src/jusprin
```

### In Application

**GUI**: Settings → Preferences → Language → Select language → Restart

---

## Common Issues

### Issue: Translation not showing

**Causes**:
1. MO file not compiled
2. Language files not in correct path
3. String not marked with `_L()`

**Solution**:
```bash
# Recompile translations
cd localization/i18n/zh_CN
msgfmt -o JusPrin.mo JusPrin.po

# Check file location
ls -la resources/localization/zh_CN/JusPrin.mo
```

### Issue: Formatting errors

**Cause**: Placeholder mismatch between original and translation

**Fix**:
```po
# ❌ Wrong - placeholder order changed
msgid "Loaded %1% objects in %2% seconds"
msgstr "%2% segundos para carregar %1% objetos"  # Boost format won't work

# ✅ Correct - preserve parameter order
msgstr "Carregados %1% objetos em %2% segundos"
```

---

## Contributing Translations

1. **Fork** the repository
2. **Add/update** your language PO file
3. **Test** thoroughly in the application
4. **Submit** Pull Request with:
   - Language code and native name
   - Translation completion percentage
   - Screenshots of translated UI (if new language)

---

## Resources

- **gettext Manual**: https://www.gnu.org/software/gettext/manual/
- **wxWidgets i18n**: https://docs.wxwidgets.org/3.0/overview_i18n.html
- **Poedit**: https://poedit.net/
- **Weblate**: https://weblate.org/ (planned)

---

*Document Version: 1.0 (P2.3)*  
*Last Updated: 2024*
