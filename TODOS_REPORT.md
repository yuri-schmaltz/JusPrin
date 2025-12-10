# TODO/FIXME Analysis Report

**Generated**: qua 10 dez 2025 02:00:25 -03

**Total Items**: 642

## Summary

| Metric | Count |
|--------|-------|
| Total TODOs | 215 |
| Total FIXMEs | 427 |
| P0 (Critical) | 3 |
| P1 (High) | 14 |
| P2 (Medium) | 614 |
| Obsolete | 11 |

## Breakdown by Priority

### P0 - 3 items

- **[FIXME]** `src/libslic3r/Fill/FillRectilinear.cpp:1512`
  - consume the left / right connecting segments at the other end of this line? Currently it is not critical because a perimeter segment is not followed if the vertical segment at the other side has already been consumed.
  - Category: `other`

- **[FIXME]** `src/libslic3r/Model.cpp:2015`
  - crashes if not satisfied
  - Category: `other`

- **[FIXME]** `src/libslic3r/PrintConfig.cpp:7280`
  - no validation on SLA data?
  - Category: `other`

### P1 - 14 items

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:1669`
  - stretch bead widths and locations of the higher bead count beading to fit in the left over space
  - Category: `other`

- **[TODO]** `src/libslic3r/ExtrusionEntity.cpp:347`
  - the main logic is largly copied from the calculate_polygon_angles_at_vertices function in SeamPlacer file. Need to refactor the code in the future.
  - Category: `other`

- **[FIXME]** `src/libslic3r/Fill/FillBase.cpp:1701`
  - improve the Traveling Salesman problem with 2-opt and 3-opt local optimization.
  - Category: `other`

- **[FIXME]** `src/libslic3r/GCode.cpp:3443`
  - infinite or high skirt does not make sense for sequential print!
  - Category: `other`

- **[FIXME]** `src/libslic3r/GCode/SpiralVase.cpp:89`
  - Performance warning: This copies the GCodeConfig of the reader.
  - Category: `performance`

- **[FIXME]** `src/libslic3r/PNGReadWrite.cpp:188`
  - maybe better to use tdefl_write_image_to_png_file_in_memory() instead?
  - Category: `memory`

- **[TODO]** `src/libslic3r/SLA/SupportTree.hpp:110`
  - Part of future refactor
  - Category: `other`

- **[FIXME]** `src/libslic3r/Support/SupportMaterial.cpp:2603`
  - higher expansion_to_slice here? why?
  - Category: `other`

- **[FIXME]** `src/libslic3r/Support/TreeSupportCommon.hpp:144`
  - likely not needed, optimization for clipping of interface layers
  - Category: `other`

- **[FIXME]** `src/libslic3r/Time.cpp:38`
  - Implementations with the cpp11 put_time and get_time either not
  - Category: `other`

- **[FIXME]** `src/slic3r/GUI/GUI_App.cpp:5688`
  - wxWidgets cause havoc if the current locale is deleted. We just forget it causing memory leaks for now.
  - Category: `memory`

- **[FIXME]** `src/slic3r/GUI/ImGuiWrapper.cpp:2684`
  - replace with io.Fonts->AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, m_font_size, nullptr, ranges.Data);
  - Category: `memory`

- **[TODO]** `src/slic3r/GUI/Jobs/EmbossJob.cpp:1103`
  - Refactor to create better way to not set cube at begining
  - Category: `other`

- **[TODO]** `src/slic3r/GUI/Tab.cpp:5663`
  - remove this call currently, after refactor, there is Paint event in the queue
  - Category: `other`

### P2 - 614 items

- **[FIXME]** `src/OrcaSlicer.cpp:1423`
  - should we check the version here? // | LoadStrategy::CheckVersion ?
  - Category: `validation`

- **[TODO]** `src/OrcaSlicer.cpp:2552`
  - support user machine preset's different settings
  - Category: `other`

- **[TODO]** `src/OrcaSlicer.cpp:2689`
  - support system process preset
  - Category: `other`

- **[TODO]** `src/OrcaSlicer.cpp:2778`
  - update different settings of filaments
  - Category: `other`

- **[FIXME]** `src/OrcaSlicer.cpp:3670`
  - Vojtech: Who knows why the complete model should be aligned with Z as a single rigid body?
  - Category: `other`

- **[TODO]** `src/OrcaSlicer.cpp:3779`
  - copy less stuff around using pointers
  - Category: `other`

- **[TODO]** `src/OrcaSlicer.cpp:3803`
  - copy less stuff around using pointers
  - Category: `other`

- **[FIXME]** `src/OrcaSlicer.cpp:4824`
  - check for mixing the FFF / SLA parameters.
  - Category: `validation`

- **[FIXME]** `src/libslic3r/AABBTreeIndirect.hpp:263`
  - implement SSE for float AABB trees with float ray queries.
  - Category: `other`

- **[FIXME]** `src/libslic3r/AABBTreeIndirect.hpp:677`
  - do we want to apply an epsilon?
  - Category: `other`

- **[FIXME]** `src/libslic3r/Algorithm/RegionExpansion.cpp:517`
  - offset & merging could be more efficient, for example one does not need to copy the source expolygon
  - Category: `other`

- **[FIXME]** `src/libslic3r/AppConfig.cpp:29`
  - replace the two following includes with <boost/md5.hpp> after it becomes mainstream.
  - Category: `other`

- **[FIXME]** `src/libslic3r/AppConfig.cpp:446`
  - replace the two following includes with <boost/md5.hpp> after it becomes mainstream.
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:179`
  - investigate whether boost:voronoi can produce multiple verts and violates consistency
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:1102`
  - prevent recalculation of these values
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:1164`
  - collapse this parameter into the bool for which it is used here!
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:1206`
  - take transition mids into account
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidation.cpp:1668`
  - don't use toolpath locations past the middle!
  - Category: `other`

- **[TODO]** `src/libslic3r/Arachne/SkeletalTrapezoidationGraph.cpp:417`
  - verify this is always the case.
  - Category: `validation`

- **[TODO]** `src/libslic3r/Arachne/WallToolPaths.cpp:398`
  - Can be made more efficient (for example, use pointer-types for process-/skip-indices, so we can swap them without copy).
  - Category: `other`

_... and 594 more_

### OBSOLETE - 11 items

- **[TODO]** `src/libslic3r/Arachne/WallToolPaths.cpp:483`
  - Open question: Does this indeed fix all (or all-but-one-in-a-million) cases for manifold but otherwise possibly complex polygons?
  - Category: `other`

- **[FIXME]** `src/libslic3r/Config.cpp:605`
  - Special handling of vectors of bools, quick and not so dirty solution before PrusaSlicer 2.3.2 release.
  - Category: `ui`

- **[FIXME]** `src/libslic3r/Format/3mf.cpp:1055`
  - Loading a "will be one day a legacy format" of configuration in a form of a G-code comment.
  - Category: `documentation`

- **[FIXME]** `src/libslic3r/Format/3mf.cpp:3312`
  - provide a version of PrusaSlicer that stored the project file (3MF).
  - Category: `other`

- **[FIXME]** `src/libslic3r/Format/AMF.cpp:718`
  - Loading a "will be one day a legacy format" of configuration in a form of a G-code comment.
  - Category: `documentation`

- **[TODO]** `src/libslic3r/Format/bbs_3mf.cpp:4483`
  - Orca: support legacy text info
  - Category: `other`

- **[FIXME]** `src/libslic3r/Format/bbs_3mf.cpp:8078`
  - provide a version of PrusaSlicer that stored the project file (3MF).
  - Category: `other`

- **[FIXME]** `src/libslic3r/GCode/GCodeProcessor.cpp:620`
  - Workaround and should be handled when do removing-bambu
  - Category: `other`

- **[FIXME]** `src/libslic3r/PresetBundle.cpp:2094`
  - legacy, the keys should not be there after conversion to a Physical Printer profile.
  - Category: `other`

- **[FIXME]** `src/libslic3r/SLA/Concurrency.hpp:4`
  - Deprecated
  - Category: `other`

- **[FIXME]** `src/slic3r/GUI/GUI_ObjectTable.cpp:2223`
  - recycle the old editor and renders
  - Category: `other`

## Breakdown by Category

| Category | Count |
|----------|-------|
| other | 515 |
| ui | 54 |
| validation | 25 |
| performance | 17 |
| threading | 14 |
| documentation | 6 |
| error_handling | 6 |
| memory | 4 |
| code_style | 1 |

## Top Files with Most TODOs

| File | Count |
|------|-------|
| `src/libslic3r/Support/TreeSupport3D.cpp` | 36 |
| `src/libslic3r/GCode.cpp` | 30 |
| `src/libslic3r/Support/SupportMaterial.cpp` | 24 |
| `src/libslic3r/Support/SupportCommon.cpp` | 20 |
| `src/slic3r/GUI/Plater.cpp` | 20 |
| `src/libslic3r/PrintObject.cpp` | 18 |
| `src/slic3r/GUI/MainFrame.cpp` | 13 |
| `src/libslic3r/Fill/FillRectilinear.cpp` | 12 |
| `src/libslic3r/PerimeterGenerator.cpp` | 11 |
| `src/slic3r/GUI/GUI_App.cpp` | 11 |
| `src/slic3r/GUI/Tab.cpp` | 10 |
| `src/libslic3r/TriangleMeshSlicer.cpp` | 9 |
| `src/libslic3r/Print.cpp` | 9 |
| `src/libslic3r/ClipperUtils.cpp` | 9 |
| `src/libslic3r/GCode/AvoidCrossingPerimeters.cpp` | 9 |

## Recommended Actions

### Immediate (P0 - 3 items)

These should be converted to GitHub issues and addressed ASAP:

1. [FIXME] `src/libslic3r/Fill/FillRectilinear.cpp:1512` - consume the left / right connecting segments at the other end of this line? Curr...
2. [FIXME] `src/libslic3r/Model.cpp:2015` - crashes if not satisfied...
3. [FIXME] `src/libslic3r/PrintConfig.cpp:7280` - no validation on SLA data?...

### High Priority (P1 - 14 items)

Should be addressed in next sprint.

### Medium Priority (P2 - 614 items)

Nice to have improvements.

### Obsolete (11 items)

These should be removed or updated:

1. `src/libslic3r/Arachne/WallToolPaths.cpp:483` - Open question: Does this indeed fix all (or all-but-one-in-a-million) cases for ...
2. `src/libslic3r/Config.cpp:605` - Special handling of vectors of bools, quick and not so dirty solution before Pru...
3. `src/libslic3r/Format/3mf.cpp:1055` - Loading a "will be one day a legacy format" of configuration in a form of a G-co...
4. `src/libslic3r/Format/3mf.cpp:3312` - provide a version of PrusaSlicer that stored the project file (3MF)....
5. `src/libslic3r/Format/AMF.cpp:718` - Loading a "will be one day a legacy format" of configuration in a form of a G-co...
6. `src/libslic3r/Format/bbs_3mf.cpp:4483` - Orca: support legacy text info...
7. `src/libslic3r/Format/bbs_3mf.cpp:8078` - provide a version of PrusaSlicer that stored the project file (3MF)....
8. `src/libslic3r/GCode/GCodeProcessor.cpp:620` - Workaround and should be handled when do removing-bambu...
9. `src/libslic3r/PresetBundle.cpp:2094` - legacy, the keys should not be there after conversion to a Physical Printer prof...
10. `src/libslic3r/SLA/Concurrency.hpp:4` - Deprecated...
