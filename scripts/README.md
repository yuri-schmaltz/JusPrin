# JusPrin Scripts

This directory contains utility scripts for development, automation, and analysis.

## Available Scripts

### 📊 `analyze_todos.py`

Analyzes and categorizes all TODO and FIXME comments in the codebase.

**Usage:**
```bash
python3 scripts/analyze_todos.py [--root /path/to/src] [--output report.md]
```

**Features:**
- Automatically categorizes by priority (P0/P1/P2/Obsolete)
- Groups by category (validation, performance, UI, etc.)
- Identifies top files with most technical debt
- Generates actionable Markdown report

**Priority Detection:**
- **P0 (Critical)**: Contains keywords like "crash", "security", "validation", "critical"
- **P1 (High)**: Performance, memory, threading, optimization
- **P2 (Medium)**: Improvements, enhancements, nice-to-have
- **Obsolete**: Legacy references (Prusa, Bambu, deprecated)

**Output:**
- Summary statistics
- Breakdown by priority
- Breakdown by category
- Top files with most TODOs
- Recommended immediate actions

**Example:**
```bash
# Scan default src/ directory
python3 scripts/analyze_todos.py

# Custom source directory with custom output
python3 scripts/analyze_todos.py --root /custom/path --output custom_report.md
```

---

### 🐍 Python Scripts (Future)

#### `orca_filament_lib.py`
Filament configuration utilities.

*Documentation pending P2.2 implementation.*

---

## Development Guidelines

### Adding New Scripts

1. **Create the script** in this directory
2. **Add executable permissions**:
   ```bash
   chmod +x scripts/your_script.py
   ```
3. **Add shebang** at the top:
   ```python
   #!/usr/bin/env python3
   ```
4. **Document it here** in this README
5. **Add type hints** (Python 3.7+):
   ```python
   def my_function(param: str) -> bool:
       """
       Brief description.
       
       Args:
           param: Description of parameter
           
       Returns:
           Description of return value
       """
       pass
   ```

### Python Code Standards

- **Type Hints**: Use them for all function signatures
- **Docstrings**: Google-style docstrings for all public functions
- **Error Handling**: Use specific exceptions, not bare `except:`
- **Imports**: Group stdlib, third-party, local imports
- **Formatting**: Follow PEP 8

---

## CI Integration

The `analyze_todos.py` script runs automatically in CI:
- Triggered on every push and PR
- Report uploaded as artifact
- Available for 30 days

See [`.github/workflows/ci.yml`](../.github/workflows/ci.yml) for details.

---

## Future Scripts (P2.2)

Planned additions:
- Type-hinted versions of existing Python utilities
- Automated code quality checks
- Dependency update scripts
- Release automation tools
