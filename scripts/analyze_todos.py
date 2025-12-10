#!/usr/bin/env python3
"""
Analyze TODOs and FIXMEs in the JusPrin codebase.

This script scans all C++ source files for TODO and FIXME comments,
categorizes them by priority, and generates a report.

Usage:
    python3 analyze_todos.py [--output todos_report.md]
"""

import os
import re
import sys
import argparse
from pathlib import Path
from typing import List, Dict, Tuple
from dataclasses import dataclass
from collections import defaultdict


@dataclass
class TodoItem:
    """Represents a single TODO or FIXME item"""
    file_path: str
    line_number: int
    todo_type: str  # 'TODO' or 'FIXME'
    text: str
    priority: str  # 'P0', 'P1', 'P2', or 'OBSOLETE'
    category: str  # e.g., 'validation', 'performance', 'ui', etc.


# Priority keywords mapping
PRIORITY_KEYWORDS = {
    'P0': ['crash', 'security', 'validation', 'data loss', 'critical', 'urgent', 'important'],
    'P1': ['performance', 'memory', 'threading', 'optimization', 'refactor'],
    'P2': ['improve', 'enhance', 'nice to have', 'consider', 'maybe'],
}

# Category keywords mapping
CATEGORY_KEYWORDS = {
    'validation': ['validate', 'check', 'verify', 'input'],
    'performance': ['performance', 'optimize', 'slow', 'speed', 'cache'],
    'memory': ['memory', 'leak', 'allocation', 'free'],
    'threading': ['thread', 'mutex', 'lock', 'race', 'concurrent'],
    'ui': ['ui', 'gui', 'dialog', 'window', 'widget'],
    'error_handling': ['error', 'exception', 'reporting', 'logging'],
    'documentation': ['document', 'doc', 'comment', 'explain'],
    'code_style': ['style', 'format', 'naming', 'cleanup'],
}


def find_source_files(root_dir: str) -> List[Path]:
    """Find all C++ source and header files in the directory"""
    extensions = {'.cpp', '.hpp', '.h', '.cxx', '.cc'}
    files = []
    
    root_path = Path(root_dir)
    for ext in extensions:
        files.extend(root_path.rglob(f'*{ext}'))
    
    # Exclude dependency directories
    files = [f for f in files if 'deps/' not in str(f) and 'deps_src/' not in str(f)]
    
    return files


def categorize_priority(text: str) -> str:
    """Determine priority based on TODO text content"""
    text_lower = text.lower()
    
    # Check for explicit priority markers
    if re.search(r'p0|critical|urgent', text_lower):
        return 'P0'
    if re.search(r'p1|important|high', text_lower):
        return 'P1'
    if re.search(r'p2|nice.?to.?have|low', text_lower):
        return 'P2'
    
    # Check for keywords
    for priority, keywords in PRIORITY_KEYWORDS.items():
        if any(keyword in text_lower for keyword in keywords):
            return priority
    
    # Check for obsolete markers
    if any(marker in text_lower for marker in ['old', 'legacy', 'deprecated', 'prusa', 'bambu']):
        return 'OBSOLETE'
    
    # Default to P2 if not clearly critical
    return 'P2'


def categorize_by_content(text: str) -> str:
    """Determine category based on TODO text content"""
    text_lower = text.lower()
    
    for category, keywords in CATEGORY_KEYWORDS.items():
        if any(keyword in text_lower for keyword in keywords):
            return category
    
    return 'other'


def extract_todos_from_file(file_path: Path) -> List[TodoItem]:
    """Extract all TODO and FIXME items from a file"""
    todos = []
    
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            for line_num, line in enumerate(f, start=1):
                # Match TODO or FIXME comments
                # Patterns: //TODO, // TODO, //FIXME, // FIXME, etc.
                match = re.search(r'//\s*(TODO|FIXME)\s*:?\s*(.+)', line, re.IGNORECASE)
                if match:
                    todo_type = match.group(1).upper()
                    text = match.group(2).strip()
                    
                    priority = categorize_priority(text)
                    category = categorize_by_content(text)
                    
                    todos.append(TodoItem(
                        file_path=str(file_path),
                        line_number=line_num,
                        todo_type=todo_type,
                        text=text,
                        priority=priority,
                        category=category
                    ))
    except Exception as e:
        print(f"Warning: Could not read {file_path}: {e}", file=sys.stderr)
    
    return todos


def generate_markdown_report(todos: List[TodoItem], output_file: str):
    """Generate a Markdown report of all TODOs"""
    
    # Group by priority
    by_priority = defaultdict(list)
    for todo in todos:
        by_priority[todo.priority].append(todo)
    
    # Group by category
    by_category = defaultdict(list)
    for todo in todos:
        by_category[todo.category].append(todo)
    
    # Group by file
    by_file = defaultdict(list)
    for todo in todos:
        by_file[todo.file_path].append(todo)
    
    with open(output_file, 'w') as f:
        f.write("# TODO/FIXME Analysis Report\n\n")
        f.write(f"**Generated**: {os.popen('date').read().strip()}\n\n")
        f.write(f"**Total Items**: {len(todos)}\n\n")
        
        # Summary table
        f.write("## Summary\n\n")
        f.write("| Metric | Count |\n")
        f.write("|--------|-------|\n")
        f.write(f"| Total TODOs | {sum(1 for t in todos if t.todo_type == 'TODO')} |\n")
        f.write(f"| Total FIXMEs | {sum(1 for t in todos if t.todo_type == 'FIXME')} |\n")
        f.write(f"| P0 (Critical) | {len(by_priority['P0'])} |\n")
        f.write(f"| P1 (High) | {len(by_priority['P1'])} |\n")
        f.write(f"| P2 (Medium) | {len(by_priority['P2'])} |\n")
        f.write(f"| Obsolete | {len(by_priority['OBSOLETE'])} |\n\n")
        
        # By priority
        f.write("## Breakdown by Priority\n\n")
        for priority in ['P0', 'P1', 'P2', 'OBSOLETE']:
            items = by_priority[priority]
            if items:
                f.write(f"### {priority} - {len(items)} items\n\n")
                for item in sorted(items, key=lambda x: (x.file_path, x.line_number))[:20]:  # Limit to 20
                    rel_path = item.file_path.replace('/home/yurix/Documentos/JusPrin/', '')
                    f.write(f"- **[{item.todo_type}]** `{rel_path}:{item.line_number}`\n")
                    f.write(f"  - {item.text}\n")
                    f.write(f"  - Category: `{item.category}`\n\n")
                
                if len(items) > 20:
                    f.write(f"_... and {len(items) - 20} more_\n\n")
        
        # By category
        f.write("## Breakdown by Category\n\n")
        f.write("| Category | Count |\n")
        f.write("|----------|-------|\n")
        for category in sorted(by_category.keys(), key=lambda k: len(by_category[k]), reverse=True):
            f.write(f"| {category} | {len(by_category[category])} |\n")
        f.write("\n")
        
        # Top files with most TODOs
        f.write("## Top Files with Most TODOs\n\n")
        top_files = sorted(by_file.items(), key=lambda x: len(x[1]), reverse=True)[:15]
        f.write("| File | Count |\n")
        f.write("|------|-------|\n")
        for file_path, items in top_files:
            rel_path = file_path.replace('/home/yurix/Documentos/JusPrin/', '')
            f.write(f"| `{rel_path}` | {len(items)} |\n")
        f.write("\n")
        
        # Action items
        f.write("## Recommended Actions\n\n")
        f.write(f"### Immediate (P0 - {len(by_priority['P0'])} items)\n\n")
        f.write("These should be converted to GitHub issues and addressed ASAP:\n\n")
        for i, item in enumerate(sorted(by_priority['P0'], key=lambda x: (x.file_path, x.line_number))[:10], 1):
            rel_path = item.file_path.replace('/home/yurix/Documentos/JusPrin/', '')
            f.write(f"{i}. [{item.todo_type}] `{rel_path}:{item.line_number}` - {item.text[:80]}...\n")
        f.write("\n")
        
        f.write(f"### High Priority (P1 - {len(by_priority['P1'])} items)\n\n")
        f.write("Should be addressed in next sprint.\n\n")
        
        f.write(f"### Medium Priority (P2 - {len(by_priority['P2'])} items)\n\n")
        f.write("Nice to have improvements.\n\n")
        
        f.write(f"### Obsolete ({len(by_priority['OBSOLETE'])} items)\n\n")
        f.write("These should be removed or updated:\n\n")
        for i, item in enumerate(sorted(by_priority['OBSOLETE'], key=lambda x: x.file_path)[:10], 1):
            rel_path = item.file_path.replace('/home/yurix/Documentos/JusPrin/', '')
            f.write(f"{i}. `{rel_path}:{item.line_number}` - {item.text[:80]}...\n")
    
    print(f"Report written to: {output_file}")


def main():
    parser = argparse.ArgumentParser(description='Analyze TODO/FIXME items in JusPrin codebase')
    parser.add_argument('--root', default='/home/yurix/Documentos/JusPrin/src', 
                       help='Root directory to scan')
    parser.add_argument('--output', default='todos_report.md',
                       help='Output Markdown file')
    
    args = parser.parse_args()
    
    print(f"Scanning {args.root} for TODO/FIXME items...")
    
    files = find_source_files(args.root)
    print(f"Found {len(files)} source files")
    
    all_todos = []
    for file_path in files:
        todos = extract_todos_from_file(file_path)
        all_todos.extend(todos)
    
    print(f"Found {len(all_todos)} TODO/FIXME items")
    print(f"  - TODOs: {sum(1 for t in all_todos if t.todo_type == 'TODO')}")
    print(f"  - FIXMEs: {sum(1 for t in all_todos if t.todo_type == 'FIXME')}")
    
    generate_markdown_report(all_todos, args.output)
    print("Done!")


if __name__ == '__main__':
    main()
