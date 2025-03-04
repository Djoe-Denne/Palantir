# GitHub Actions Workflow

This document describes how our GitHub Actions CI/CD pipeline works.

## Workflow Triggers

The workflow runs on:
- Push to `master` branch
- Pull requests targeting `master` branch

## Jobs

### 1. Code Quality (`code-quality`)

Runs only on macOS to ensure consistent code quality checks.

Steps:
1. Check out repository with full history (for PR comparisons)
2. For pull requests:
   - Get list of changed C++ files (`.cpp`, `.hpp`, `.h`, `.cc`)
   - Pass changed files to CMake for selective linting
3. Configure CMake in Release mode
   - Uses `LINT_FILES` option for PR builds to limit scope
   - Uses all files for direct pushes to master
4. Check code formatting (`format-check`)
5. Run static analysis (`lint-check`)
6. Analyze lint results (`lint-analyze`)
   - Fails if more than 5 warnings are found

### 2. Build (`build`)

Runs after code quality checks pass on both Windows and macOS platforms.

Steps:
1. Configure CMake in Release mode
2. Build the project
3. Upload artifacts:
   - Windows: `Palantir.exe`
   - macOS: `Palantir.app`

## Job Dependencies

- `build` job depends on `code-quality`
- Code quality must pass before builds start
- Builds run in parallel across platforms once quality checks pass

## Artifacts

Each successful build produces platform-specific artifacts:
- Windows: `Palantir-Windows`
  - Path: `build/Release/Palantir.exe`
- macOS: `Palantir-macOS`
  - Path: `build/Palantir.app`

## Concurrency

- Only one workflow runs at a time per branch/PR
- New workflows cancel in-progress workflows on the same ref

## Environment Settings

Global environment variables:
- `FORCE_COLOR=1` - Enables colored output
- `TERM=xterm-256color` - Better formatting support

## Selective Linting

The workflow implements selective linting for pull requests:

1. **File Detection**:
   ```bash
   CHANGED_FILES=$(git diff --name-only origin/${{ github.base_ref }}...HEAD | grep -E '\.(cpp|hpp|h|cc)$')
   ```

2. **CMake Integration**:
   ```bash
   cmake -B build -DLINT_FILES="${CHANGED_FILES}" -DCMAKE_BUILD_TYPE=Release
   ```

3. **Behavior**:
   - Pull Requests: Only lint changed C++ files
   - Direct Pushes: Lint all files
   - Empty Changes: Falls back to full linting

## Platform Strategy

- **Code Quality**:
  - Runs exclusively on macOS
  - Ensures consistent formatting and lint results
  - Leverages macOS-native clang tools

- **Builds**:
  - Runs on both Windows and macOS
  - Produces platform-specific binaries
  - Uses matrix strategy for parallel execution 