# Contributing Guide

Thank you for considering a contribution to uC‑MicroLab Shield!

This guide explains how to propose changes, what standards to follow, and the licensing terms that apply to your contributions.

## Table of Contents
1. Code of Conduct
2. Types of Contributions
3. Hardware Change Workflow
4. Firmware/Examples Workflow
5. Commit Style (Conventional Commits)
6. Coding Style
7. Testing and CI
8. Preparing Manufacturing Artifacts (Gerbers, BOM, STEP)
9. Reporting Bugs
10. Safety
11. Licensing of Contributions
12. Review and Merge

---

## 1) Code of Conduct
By participating, you agree to follow the [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## 2) Types of Contributions
- PCB fixes (silkscreen, footprint, clearance adjustments)
- New hardware revisions (R1.1, R2, …)
- Drivers and multi‑platform abstractions
- New or improved examples
- Documentation (pinouts, setup guides)

## 3) Hardware Change Workflow (CERN‑OHL‑W‑2.0)
Key principles:
- Past revisions are immutable. Do not modify `hardware/R1/` once published. Create a new revision folder (e.g., `hardware/R1.1/`) for changes.
- Each revision must include the “Complete Source” needed to rebuild/modify the design (KiCad editable sources, custom libraries, BOM, Pick & Place, STEP, and essential instructions).
- Preserve license notices and include SPDX where possible: `SPDX-License-Identifier: CERN-OHL-W-2.0`.

Recommended steps:
1. Open an Issue using the “Hardware Change” template and discuss the proposal (problem, impact, approach, risks).
2. Create a branch: `feature/hw-R1.1-short-desc`.
3. Create the new revision folder (e.g., `hardware/R1.1/`) and copy forward only what is needed; do not overwrite old revisions.
4. Edit KiCad sources (schematics and PCB). Run ERC/DRC and resolve all errors.
5. Export manufacturing artifacts (see Section 8).
6. Update:
   - `hardware/R1.1/README_R1.1.md` (what changed, limitations, assembly notes).
   - `hardware/R1.1/compliance/license-notice.txt` (include Source Location).
7. Open a Pull Request referencing the Issue.

Hardware PR checklist:
- [ ] ERC/DRC are clean
- [ ] BOM updated and consistent with schematics
- [ ] Pick & Place generated
- [ ] STEP model exported
- [ ] Renders/photos updated (if available)
- [ ] `license-notice.txt` present in the revision
- [ ] Revision README updated
- [ ] SPDX headers where applicable (KiCad/editable files)

## 4) Firmware/Examples Workflow (MIT)
1. Open an Issue (bug/feature).
2. Create a branch: `feat/fw-short-desc` or `fix/fw-short-desc`.
3. Follow coding style (see Section 6) and add/adjust tests if applicable.
4. Ensure builds succeed for relevant platforms (Arduino/ESP32/…).
5. Update README(s) for examples (purpose, requirements, pins, build steps).
6. Open a Pull Request with clear description and testing steps.

Firmware PR checklist:
- [ ] Builds pass locally (and in CI, if enabled)
- [ ] Formatting/linting applied
- [ ] SPDX header in new source files: `SPDX-License-Identifier: MIT`
- [ ] Example README updated
- [ ] Changelog entry proposed (if applicable)

## 5) Commit Style (Conventional Commits)
Format: `type(scope): description`

Common types:
- `feat:` new feature
- `fix:` bug fix
- `docs:` documentation only
- `chore:` maintenance tasks
- `refactor:` code refactoring
- `test:` tests only
- `hardware:` hardware‑specific changes

Examples:
- `hardware(R1.1): adjust USB-C footprint clearance`
- `feat(arduino): add diagnostics example`
- `docs: update R1 pinout table`

## 6) Coding Style
- C/C++: keep consistent formatting (a `.clang-format` may be added later).
- Python: consistent style (pep8/black‑like).
- Keep dependencies minimal.
- Add a short license header (SPDX) at the top of each source file.

## 7) Testing and CI
- Add unit/integration tests where feasible.
- If CI is enabled in this repository, PRs should pass all checks (lint/build/tests).
- If CI is not available, provide clear local reproduction/build steps in the PR description.

## 8) Preparing Manufacturing Artifacts (Gerbers, BOM, STEP)
No scripts are required. Please export using KiCad’s UI (or your preferred tool workflow):

- Gerbers and Drill files (include board outline and all fabrication layers required by your manufacturer).
- BOM (CSV) with consistent headers (e.g., Designator, Qty, Manufacturer, MPN, Description, Package, Supplier, Supplier P/N, Notes).
- Pick & Place (component positions/orientation).
- STEP 3D model of the board (and enclosure if applicable).
- Assembly/mechanical PDFs if available.
- Place all outputs inside the corresponding revision folder (e.g., `hardware/R1.1/gerbers/`, `hardware/R1.1/bom/`, `hardware/R1.1/pickplace/`, `hardware/R1.1/step/`).

Before opening the PR:
- Run ERC/DRC in KiCad and resolve issues.
- Check silkscreen legibility and clearances.
- Ensure the BOM matches schematic references.

## 9) Reporting Bugs
Please include:
- Hardware revision (e.g., R1, R1.1)
- Host platform (e.g., ESP32 DevKit v1)
- Logs/photos when relevant
- Exact reproduction steps

## 10) Safety
If you identify an electrical/thermal safety concern, label the issue clearly (e.g., “safety”) and describe severity, symptoms, and mitigation. For serious risks, consider contacting maintainers privately before public disclosure.

## 11) Licensing of Contributions
By contributing, you agree:
- Hardware design contributions are licensed under CERN‑OHL‑W‑2.0.
- Firmware/examples/scripts contributions are licensed under MIT.
- You have the right to submit the contributed material and to license it under these terms.
- Preserve third‑party notices and licenses.

## 12) Review and Merge
- We aim to review PRs for technical quality, documentation completeness, and licensing compliance.
- Smaller, focused PRs are easier to review and merge.
- Squash & merge may be used to keep history clean.

Thank you for contributing!
