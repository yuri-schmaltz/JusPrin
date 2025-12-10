# JusPrin AI Architecture Documentation

> [!CAUTION]
> **Critical Finding**: During repository analysis (2025-12-10), comprehensive code search revealed that the GenAI/JusBot features mentioned in the `README.md` are **NOT YET IMPLEMENTED** in the codebase.

## Executive Summary

JusPrin is described in the README as "the first GenAI app for 3D printing" with a "JusBot" that guides users through print requirements. However, investigation of the codebase reveals **no evidence of GenAI or LLM integration**.

### Search Results

**Keywords Searched:**
- "`JusBot`" - Found only in [README.md:36](file:///home/yurix/Documentos/JusPrin/README.md#L36)
- "`GenAI`" - Found in [README.md:4](file:///home/yurix/Documentos/JusPrin/README.md#L4) and `resources/web/homepage/index.html:29`
- "`LLM`" - No occurrences in source code

**No Implementation Found:**
- No AI/ML libraries imported (no TensorFlow, PyTorch, OpenAI SDK, Anthropic SDK, etc.)
- No LLM API calls or inference code
- No model files or weights
- No prompt templates or AI configuration

---

## Current State of the Project

### What IS Implemented

JusPrin is a **fork of OrcaSlicer**, which itself is based on BambuStudio and PrusaSlicer. The actual codebase implements:

1. **Traditional 3D Slicer Functionality**
   - STL/3MF/OBJ model loading
   - Layer-by-layer slicing algorithms
   - G-code generation
   - Support generation
   - Infill patterns

2. **GUI Application (wxWidgets)**
   - 3D model viewer
   - Parameter configuration dialogs
   - Print preview
   - Profile management

3. **Printer Communication**
   - Network protocols for various 3D printers
   - File transfer
   - Print monitoring

### What is NOT Implemented

1. **AI-Powered Parameter Selection** ❌
   - No automated slicer settings recommendation
   - No machine learning models
   - No intelligent defaults based on model analysis

2. **JusBot Conversational Interface** ❌
   - No chat interface
   - No natural language processing
   - No LLM integration

3. **GenAI Features** ❌
   - No generative AI capabilities
   - No model analysis for automatic setting suggestion
   - No AI-powered troubleshooting

---

## Architecture Analysis: Where AI WOULD Be Integrated

If the GenAI features were to be implemented, here's where they would logically fit:

### 1. Parameter Recommendation Engine

**Location:** `src/slic3r/AI/` (does not exist)

**Proposed Architecture:**
```
┌─────────────────────────────────────────┐
│  User Loads 3D Model                    │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│  Model Analyzer                         │
│  - Geometry analysis (volume, holes)    │
│  - Material detection                   │
│  - Print intent extraction              │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│  LLM Integration Layer                  │
│  - API: OpenAI / Anthropic / Local LLM  │
│  - Prompt: Model features + User intent │
│  - Response: Recommended parameters     │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│  Config Generator                       │
│  - Apply AI suggestions to PrintConfig  │
│  - Validate against printer capabilities│
│  - Generate preview                     │
└─────────────────────────────────────────┘
```

**Key Files That Would Need Modification:**
- `src/OrcaSlicer.cpp` - Main application entry point
- `src/libslic3r/PrintConfig.cpp` - Configuration management
- `src/slic3r/GUI/Plater.cpp` - Main GUI coordination

### 2. JusBot Conversational Interface

**Location:** `src/slic3r/GUI/AI/` (does not exist)

**Proposed Components:**

```cpp
// src/slic3r/AI/JusBot.hpp
class JusBot {
public:
    // Start conversation with user
    void startSession();
    
    // Process user message, return bot response
    std::string chat(const std::string& user_message);
    
    // Apply recommended settings to current config
    void applyRecommendations(const AIRecommendation& rec);
    
private:
    // LLM client (OpenAI, Anthropic, or open-source)
    std::unique_ptr<LLMClient> llm_client_;
    
    // Current conversation context
    ConversationContext context_;
    
    // Current model being sliced
    ModelObject* current_model_;
};
```

**GUI Integration Point:**
- Add chat panel to main window (likely in `src/slic3r/GUI/MainFrame.cpp`)
- Could use existing `resources/web/jusprin/` for web-based chat UI
- Would need WebSocket or IPC to communicate between C++ backend and web frontend

### 3. Suggested Technology Stack for AI Features

#### Option A: Cloud-Based LLM
```yaml
Provider: OpenAI / Anthropic
API: REST API calls
Dependencies:
  - libcurl (already used)
  - JSON library (already have nlohmann/json)
Cost: API usage fees
Pros: State-of-the-art models, no local compute
Cons: Requires internet, privacy concerns, ongoing costs
```

#### Option B: Local Open-Source LLM
```yaml
Model: Llama 3, Mistral, or similar
Inference: llama.cpp or ONNX Runtime
Dependencies:
  - llama.cpp (C++ library)
  - Model weights (~4-13GB download)
Cost: One-time download, local compute
Pros: Privacy, offline capability, no recurring costs
Cons: Requires GPU/powerful CPU, larger binary
```

#### Option C: Hybrid Approach
```yaml
Default: Cloud LLM for best experience
Fallback: Local lightweight model
User Choice: Let user select provider
```

---

## Implementation Roadmap

### Phase 1: Foundation (4-6 weeks)

#### Week 1-2: Model Analysis
- [ ] Implement geometry analyzer
  - Volume calculation
  - Surface area
  - Overhang detection
  - Support requirement estimation
- [ ] Create feature extractor
  - Identify small details
  - Detect thin walls
  - Find bridging areas

#### Week 3-4: LLM Integration Layer
- [ ] Design API abstraction
  - Interface for different LLM providers
  - Prompt template system
  - Response parsing
- [ ] Implement OpenAI client (MVP)
  - API key management
  - Request/response handling
  - Error handling and retries

#### Week 5-6: Basic Parameter Recommendation
- [ ] Create prompt engineering templates
  - Model description → parameter suggestions
  - User intent → configuration mapping
- [ ] Implement config application
  - Parse LLM response to PrintConfig
  - Validate suggested parameters
  - Apply to current print job

###  Phase 2: JusBot Interface (3-4 weeks)

#### Week 7-8: Chat Backend
- [ ] Implement conversation management
  - Context tracking
  - Multi-turn conversation
  - History persistence
- [ ] Create AI service
  - Background thread for LLM calls
  - Queue system for requests
  - Cancellation support

#### Week 9-10: Chat Frontend
- [ ] Build chat UI (wxWidgets or web-based)
  - Message input/output
  - Typing indicators
  - Suggestion chips
- [ ] Integrate with main application
  - Show/hide chat panel
  - Apply recommendations button
  - Preview changes

### Phase 3: Advanced Features (4-6 weeks)

#### Week 11-13: Intelligent Defaults
- [ ] Train/tune recommendation model
  - Collect print success data
  - Fine-tune LLM on 3D printing domain
  - A/B test suggestions
- [ ] Implement learning system
  - User accepts/rejects recommendations
  - Feedback loop to improve suggestions

#### Week 14-16: Troubleshooting Assistant
- [ ] Failed print diagnosis
  - Image analysis of failures (optional)
  - Ask diagnostic questions
  - Suggest parameter adjustments
- [ ] Knowledge base integration
  - Common issues → solutions
  - Link to calibration guides

---

## Privacy and Security Considerations

### Data That Would Be Sent to LLM

**Model Data:**
- 3D model geometry (vertices, faces)
- Model metadata (filename, size)
- Calculated features (volume, surface area)

**User Data:**
- Printer capabilities
- Selected material
- User's natural language description of intent
- Previous conversation history

### Privacy Protection Measures

1. **User Consent**
   - Explicit opt-in for AI features
   - Clear data usage policy
   - Option to use local-only model

2. **Data Minimization**
   - Send only features, not full model
   - Anonymize user data
   - No model file upload (analysis done locally)

3. **Local-First Option**
   - Provide offline inference mode
   - Download open-source model weights
   - All processing on-device

---

## Example User Flows

### Flow 1: First-Time User with AI Assistant

```
1. User: Opens JusPrin, loads benchy.stl
2. JusBot: "Hi! I see you've loaded a model. What would you like to print?"
3. User: "I want a detailed benchy for display, not worried about speed"
4. JusBot: Analyzing...
   - Model: 3DBenchy  
   - Detected features: Small details, overhangs, bridging
   - Intent: Display quality, no time constraint
   
   Recommended settings:
   - Layer height: 0.12mm (high detail)
   - Infill: 20% (adequate strength for display)
   - Speed: 40mm/s (quality over speed)
   - Supports: Tree supports (cleaner removal)
   
   Would you like me to apply these settings?
5. User: "Yes, but make it faster if possible"
6. JusBot: Adjusting... Speed increased to 60mm/s with adaptive layer heights
          to maintain quality on detailed areas.
7. User: Clicks "Slice"
8. System: Generates G-code with AI-recommended parameters
```

### Flow 2: Experienced User with Quick Question

```
1. User: Already has a configured print, asks in chat
        "Will this need supports?"
2. JusBot: Analyzes current model
        "Yes, I detected overhangs > 60° on the left side.
         I recommend enabling tree supports."
3. User: Enables supports, proceeds
```

### Flow 3: Troubleshooting Failed Print

```
1. User: "My print failed with stringing"
2. JusBot: "Stringing is often caused by temperature or retraction issues.
            Current settings:
            - Temperature: 220°C
            - Retraction: 5mm @ 45mm/s
            
            Try:
            - Reduce temp to 210°C
            - Increase retraction to 6.5mm
            
            Would you like me to apply these changes?"
3. User: "Yes"
4. JusBot: Settings applied. Also, consider running temperature
           calibration. [Link to calibration guide]
```

---

## Cost and Resource Estimate

### Cloud LLM Costs (OpenAI GPT-4 example)

**Per Print Session:**
- Model analysis: ~500 tokens input
- User conversation: ~200 tokens per exchange × 3 exchanges = 600 tokens
- Recommendations: ~300 tokens output
- **Total: ~1400 tokens per session**
- **Cost: ~$0.02 USD per session** (varies by model)

**Monthly for 1000 users (avg 10 prints/month each):**
- 10,000 sessions/month × $0.02 = **$200/month**

### Local LLM Costs

**One-Time:**
- Model weights download: ~7GB (Llama 3 8B)
- Development time: Same as cloud approach

**Runtime:**
- GPU VRAM: 8-16GB recommended
- CPU: High-end required if no GPU
- Inference time: 1-3 seconds per response (GPU), 5-15 seconds (CPU)

---

## Alternative: Why This Might NOT Be GenAI

**Hypothesis:** The "GenAI" branding might be aspirational marketing rather than referring to current features.

**Possible Interpretations:**
1. **Planned Feature**: AI integration is on the roadmap but not yet implemented
2. **Different Definition**: "GenAI" refers to newer generation of slicing algorithms (not LLM-based)
3. **Marketing Positioning**: Differentiation from OrcaSlicer without technical implementation yet

---

## Recommendations

### For Users

1. **Manage Expectations**: JusPrin currently offers traditional slicing, not AI-powered parameter selection
2. **Use Existing Features**: The core OrcaSlicer functionality is robust and well-tested
3. **Stay Tuned**: AI features may be added in future releases

### For Developers

1. **Clarify README**: Update documentation to reflect current vs. planned features
2. **Create Roadmap**: If AI is planned, publish a clear development timeline
3. **MVP Approach**:START with simple parameter recommendation before full JusBot
4. **User Research**: Survey users to understand which AI features would be most valuable

### For Contributors

If you want to implement the AI features:

1. **Start Small**: Implement basic model analysis first
2. **Prototype**: Build a simple LLM integration proof-of-concept
3. **Gather Feedback**: Share prototype with community for input
4. **Iterate**: Refine based on user testing

---

## Conclusion

JusPrin is currently a well-structured fork of OrcaSlicer with **no GenAI implementation**. The branding suggests ambitious AI features, but these remain to be built.

This document provides a comprehensive blueprint for how such features COULD be implemented, including:
- Architecture design
- Technology choices
- Implementation roadmap
- Privacy considerations
- Cost estimates

**Next Steps:**
1. Clarify project intentions regarding AI features
2. If implementing AI: Follow Phase 1 roadmap
3. If not implementing AI: Update branding and documentation to reflect actual capabilities

---

**Document Status:** Based on code analysis as of 2025-12-10  
**Needs Review By:** Project maintainers  
**Related Issues:** none (should create GitHub issue to discuss AI implementation)
