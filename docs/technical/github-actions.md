# GitHub Actions Workflow

This document describes how our GitHub Actions CI/CD pipeline works.

## Workflow Triggers

The workflows run on:
- Push to `master` branch (build and code quality)
- Pull requests targeting any branch (build only)
- Issue comments with "run code quality" (code quality for PRs)

## Workflows

We have multiple workflows to optimize CI/CD performance:

1. **Build Workflow** - Builds the project on multiple platforms
2. **Code Quality Master** - Runs code quality checks on the master branch
3. **Code Quality PR** - Runs code quality checks on PRs when requested

## Jobs

### 1. Setup (`setup`)

Prepares the build environment and caches dependencies.

Steps:
1. Check out repository
2. Configure CMake in Release mode
3. Cache CMake configuration for later use

### 2. Code Quality (`code-quality`)

Runs on Ubuntu to ensure consistent code quality checks.

Steps:
1. Check out repository with full history (for PR comparisons)
2. Configure CMake with `QUALITY_ONLY=ON` to skip dependency fetching
3. Check code formatting (`format-check`)
4. Run static analysis (`lint-check`)
5. Analyze lint results (`lint-analyze`)
   - Fails if more than 5 warnings are found
6. For PRs, post results as a comment

### 3. Build (`build`)

Runs after setup on both Windows and macOS platforms.

Steps:
1. Restore CMake cache from setup job
2. Build the project
3. Save updated CMake cache

## Job Dependencies

- `build` job depends on `setup`
- Code quality jobs run independently
- PR code quality checks only run when requested with a comment

## Caching Strategy

The workflow implements caching to speed up builds:

1. **CMake Configuration Cache**:
   ```yaml
   - name: Restore CMake cache
     uses: actions/cache/restore@v3
     with:
       path: |
         ${{ env.CMAKE_BUILD_DIR }}
         ${{ env.CMAKE_BUILD_DIR }}/CMakeCache.txt
         ${{ env.CMAKE_BUILD_DIR }}/CMakeFiles
         ${{ env.CMAKE_BUILD_DIR }}/_deps
       key: ${{ env.CACHE_KEY_PREFIX }}-${{ runner.os }}-cmake-${{ hashFiles('**/CMakeLists.txt', '**/*.cmake') }}-${{ github.run_id }}
       restore-keys: |
         ${{ env.CACHE_KEY_PREFIX }}-${{ runner.os }}-cmake-
   ```

2. **Cache Invalidation**:
   - Cache keys include a prefix that can be incremented to invalidate all caches
   - File hashes ensure caches are updated when CMake files change

## Artifacts

Each successful build produces platform-specific artifacts:
- Windows: `Palantir.exe`
- macOS: `Palantir.app`

## Concurrency

- Only one workflow runs at a time per branch/PR
- New workflows cancel in-progress workflows on the same ref

## Environment Settings

Global environment variables:
- `FORCE_COLOR=1` - Enables colored output
- `TERM=xterm-256color` - Better formatting support
- `CMAKE_BUILD_DIR` - Standard build directory path
- `CACHE_KEY_PREFIX` - Version for cache invalidation

## Selective Linting

The workflow implements selective linting for pull requests:

1. **File Detection**:
   ```bash
   CHANGED_FILES=$(git diff --name-only origin/${{ github.base_ref }}...HEAD | grep -E '\.(cpp|hpp|h|cc)$')
   ```

2. **CMake Integration**:
   ```bash
   cmake -B build -DFILES_TO_LINT="${CHANGED_FILES}" -DQUALITY_ONLY=ON
   ```

3. **Behavior**:
   - Pull Requests: Only lint changed C++ files
   - Direct Pushes: Lint all files
   - Empty Changes: Falls back to full linting

## Optimized Build Process

The workflow includes several optimizations to reduce build time:

1. **Pre-built Dependencies**:
   - Uses release artifacts for Sauron SDK instead of building from source
   - Significantly reduces build time

2. **Quality-Only Mode**:
   - Code quality jobs use `QUALITY_ONLY=ON` to skip dependency fetching
   - Speeds up linting and formatting checks

3. **Optional Tests**:
   - Tests are not built by default (`BUILD_TESTS=OFF`)
   - Can be enabled when needed for test-focused workflows

4. **Caching**:
   - CMake configuration is cached between runs
   - Dependencies are cached to avoid repeated downloads 