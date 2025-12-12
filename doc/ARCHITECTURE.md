# JusPrin Architecture

## System Overview

JusPrin is an advanced 3D printing slicer built on the OrcaSlicer/PrusaSlicer codebase, providing intelligent slicing with AI-assisted parameter optimization.

```mermaid
graph TB
    subgraph "User Interface Layer"
        GUI[wxWidgets GUI]
        ImGui[ImGui 3D Viewer]
        WebUI[Web Interface]
    end
    
    subgraph "Core Slicing Engine"
        Model[3D Model Loader]
        Slicer[Slicing Engine]
        GCode[G-Code Generator]
        Config[Configuration Manager]
    end
    
    subgraph "Advanced Features"
        AI[AI Parameter Optimizer]
        Calib[Calibration Tools]
        Preview[3D Preview]
    end
    
    subgraph "I/O Layer"
        FileIO[File I/O - 3MF/STL/OBJ]
        Network[Network/Cloud]
        Printer[Printer Communication]
    end
    
    GUI --> Model
    WebUI --> Config
    ImGui --> Preview
    
    Model --> Slicer
    Config --> Slicer
    Slicer --> GCode
    
    AI --> Config
    Calib --> Slicer
    
    GCode --> FileIO
    GCode --> Printer
    Network --> Config
    
    Preview --> Slicer
    
    style AI fill:#f9f,stroke:#333,stroke-width:2px
    style Slicer fill:#bbf,stroke:#333,stroke-width:2px
```

---

## Component Breakdown

### 1. libslic3r (Core Library)

**Purpose**: Platform-independent slicing algorithms and data structures.

**Key Modules**:
- `Model`: 3D geometry representation
- `TriangleMesh`: Mesh processing
- `Print`: Slicing orchestration
- `GCode`: G-code generation
- `Config`: Configuration management

**Dependencies**: Boost, Eigen, OpenVDB, TBB

---

### 2. slic3r/GUI (User Interface)

**Purpose**: wxWidgets-based graphical user interface.

**Key Components**:
- `Plater`: Main 3D workspace
- `Tab`: Settings panels
- `GLCanvas3D`: OpenGL 3D viewer (uses ImGui)
- `Field`: Input validation widgets
- `ConfigWizard`: Setup assistant

**Dependencies**: wxWidgets 3.0+, OpenGL, ImGui, GLew

---

### 3. resources/web (Web Interface)

**Purpose**: Optional web-based control interface.

**Tech Stack**:
- HTML5 + TailwindCSS
- JavaScript (vanilla)
- RESTful API backend (planned)

**Status**: UI implemented, backend integration pending

---

## Data Flow - Slicing Pipeline

```mermaid
sequenceDiagram
    participant User
    participant GUI
    participant Model
    participant Config
    participant Slicer
    participant GCode
    participant File

    User->>GUI: Load 3D Model
    GUI->>Model: Parse STL/3MF
    Model->>Model: Validate geometry
    
    User->>GUI: Adjust settings
    GUI->>Config: Update parameters
    
    User->>GUI: Click "Slice"
    GUI->>Slicer: Start slicing
    
    Slicer->>Model: Get geometry
    Slicer->>Config: Get parameters
    
    loop For each layer
        Slicer->>Slicer: Generate perimeters
        Slicer->>Slicer: Generate infill
        Slicer->>Slicer: Generate support
    end
    
    Slicer->>GCode: Generate instructions
    GCode->>GCode: Post-process
    GCode->>File: Write .gcode
    
    File-->>User: Download/Send to printer
```

---

## AI Integration Points (Planned)

```mermaid
graph LR
    subgraph "Input Analysis"
        Geom[3D Geometry Analysis]
        Mat[Material Properties]
        Printer[Printer Capabilities]
    end
    
    subgraph "AI Decision Engine"
        LLM[Large Language Model]
        Rules[Expert Rules]
        ML[ML Optimizer]
    end
    
    subgraph "Output Parameters"
        Speed[Print Speed]
        Temp[Temperatures]
        Support[Support Strategy]
        Infill[Infill Pattern]
    end
    
    Geom --> LLM
    Mat --> LLM
    Printer --> Rules
    
    LLM --> ML
    Rules --> ML
    
    ML --> Speed
    ML --> Temp
    ML --> Support
    ML --> Infill
    
    style LLM fill:#f96,stroke:#333,stroke-width:2px
```

**Note**: AI features are documented in [`doc/AI_ARCHITECTURE.md`](file:///home/yurix/Documentos/JusPrin/doc/AI_ARCHITECTURE.md) but not yet implemented.

---

## Configuration System

```mermaid
graph TD
    subgraph "Config Hierarchy"
        System[System Defaults]
        Vendor[Vendor Presets]
        User[User Presets]
        Project[Project Settings]
    end
    
    subgraph "Config Types"
        Print[Print Config]
        Filament[Filament Config]
        PrinterCfg[Printer Config]
    end
    
    System --> Vendor
    Vendor --> User
    User --> Project
    
    Project --> Print
    Project --> Filament
    Project --> PrinterCfg
    
    Print --> Validate[Config Validator]
    Filament --> Validate
    PrinterCfg --> Validate
    
    Validate --> Slicer[Slicing Engine]
```

**Validation**: See `src/slic3r/Utils/ValidationUtils.hpp` for safe input parsing.

---

## Threading Model

```mermaid
graph LR
    subgraph "Main Thread"
        UI[UI Event Loop<br/>wxWidgets]
        Render[OpenGL Rendering<br/>ImGui]
    end
    
    subgraph "Worker Threads"
        Slice[Slicing Worker<br/>TBB]
        GCodeGen[G-Code Generator]
        Background[Background Tasks]
    end
    
    subgraph "Thread-Safe Components"
        Queue[Task Queue]
        Logger[Logger<br/>mutex-protected]
        Progress[Progress Indicator]
    end
    
    UI --> Queue
    Queue --> Slice
    Queue --> Background
    
    Slice --> Logger
    GCodeGen --> Logger
    
    Slice --> Progress
    Progress --> UI
    
    Render -.->|Read-only| Slice
```

**Thread Safety**: Logger uses `std::mutex` for thread-safe logging. See `src/slic3r/Utils/Logger.hpp`.

---

## Plugin System (Future)

```mermaid
graph TB
    subgraph "Core Application"
        Core[JusPrin Core]
        PluginMgr[Plugin Manager]
        API[Plugin API]
    end
    
    subgraph "Plugins"
        Plugin1[Custom Infill Plugin]
        Plugin2[Cloud Integration]
        Plugin3[AI Parameter Tuner]
    end
    
    Core --> PluginMgr
    PluginMgr --> API
    
    Plugin1 -.->|Implements| API
    Plugin2 -.->|Implements| API
    Plugin3 -.->|Implements| API
    
    PluginMgr --> Plugin1
    PluginMgr --> Plugin2
    PluginMgr --> Plugin3
```

**Status**: Plugin system architecture defined but not implemented. See P2.5 in implementation plan.

---

## Build System

**Build Tool**: CMake 3.13+

**Key Options**:
- `BUILD_TESTS`: Enable unit tests (ON by default, P1.1)
- `SLIC3R_GUI`: Build with GUI (default ON)
- `SLIC3R_STATIC`: Static linking (platform-dependent)
- `SLIC3R_ENABLE_LTO`: Link-time optimization (P2.1)

**Dependencies Management**:
- Linux: System packages or `build_linux.sh -d`
- Windows: `build_win.bat -d`
- macOS: Homebrew + manual deps

---

## Testing Strategy

```mermaid
graph TD
    subgraph "Test Levels"
        Unit[Unit Tests<br/>Catch2]
        Integration[Integration Tests<br/>End-to-end]
        Manual[Manual Testing<br/>Real prints]
    end
    
    subgraph "CI Pipeline"
        Linux[Linux Build]
        Windows[Windows Build]
        macOS[macOS Build]
    end
    
    Unit --> Linux
    Unit --> Windows
    Unit --> macOS
    
    Integration --> Manual
    
    Linux --> Artifacts[Test Artifacts]
    Windows --> Artifacts
    macOS --> Artifacts
```

**CI Configuration**: See [`.github/workflows/ci.yml`](file:///home/yurix/Documentos/JusPrin/.github/workflows/ci.yml)

---

## File Formats

### Input Formats
- **STL**: Triangle mesh (ASCII/Binary)
- **OBJ**: Wavefront object
- **3MF**: 3D Manufacturing Format (preferred)
- **AMF**: Additive Manufacturing Format

### Output Formats
- **G-Code**: Printer instructions
- **3MF**: Project files with embedded settings

### Configuration Formats
- **INI**: Legacy config format
- **JSON**: Vendor bundle format

---

## Error Handling & Logging

**Strategy**:
1. **Input Validation**: `ValidationUtils.hpp` for safe parsing
2. **Structured Logging**: `Logger.hpp` for debugging
3. **User Feedback**: wxWidgets dialogs for errors
4. **Crash Reports**: Optional telemetry (not yet implemented)

**Error Levels**:
- `ERROR`: Critical failures
- `WARNING`: Potential issues
- `INFO`: Normal operation
- `DEBUG`: Development info

---

## Performance Considerations

**Optimization Techniques**:
- TBB for parallel slicing
- OpenVDB for efficient volume operations
- Link-Time Optimization (LTO) - P2.1
- Lazy evaluation of geometry
- Spatial acceleration structures (KD-tree, BVH)

**Profiling**:
- Built-in profiler option: `SLIC3R_PROFILE`
- PGO support planned (P3.4)

---

## Resources

- **API Documentation**: Generate with `doxygen .doxygen` (P1.5)
- **User Manual**: See `doc/` directory
- **Developer Guide**: This document
- **AI Architecture**: [`doc/AI_ARCHITECTURE.md`](file:///home/yurix/Documentos/JusPrin/doc/AI_ARCHITECTURE.md)
- **Web UI Docs**: [`resources/web/README.md`](file:///home/yurix/Documentos/JusPrin/resources/web/README.md)

---

*Last Updated: 2024 (P2.4)*
