# GitHub Actions Workflow

This document describes our GitHub Actions CI/CD pipeline.

## Workflow Triggers

The workflows run on:
- Push to `master` branch (build and code quality)
- Pull requests targeting any branch (build only)
- Issue comments with "run code quality" (code quality for PRs)

## Workflows

We have multiple workflows to optimize CI/CD performance:

1. **Build Workflow** (`build.yml`)
   - Builds the project (currently Windows only)
   - Runs tests after successful build
   - Runs SonarQube analysis after tests
   - Cleans up caches after completion

4. **Cache Management** (`clear-caches-master.yml`, `clear-caches-pr.yml`)
   - Manages workflow caches for both master and PR builds
   - Helps maintain clean state and prevent cache bloat

## Jobs

### 1. Build (`build`)

Runs on Windows platform (macOS support is commented out for cost reasons).

Steps:
1. Check out repository
2. Build using custom build action
3. Run tests after successful build
4. Clear run caches after completion

## Job Dependencies

- Test job depends on successful build
- Cache clearing runs after tests complete
- PR code quality checks only run when requested with a comment

## Environment Settings

Global environment variables used across workflows:
- `FORCE_COLOR=1` - Enables colored output
- `TERM=xterm-256color` - Better formatting support
- `CMAKE_BUILD_DIR` - Standard build directory path
- `CACHE_KEY_PREFIX` - Version for cache invalidation (currently v1)

## Permissions

Workflows use these GitHub permissions:
- `contents: read` - For repository access
- `pull-requests: write` - For commenting on PRs
- `issues: write` - For issue interactions
- `checks: write` - For check run status updates (PR quality checks)

## Concurrency

- Build workflow cancels in-progress runs on same ref
- Code quality master workflow prevents concurrent runs but doesn't cancel in-progress ones
- PR-based workflows handle one PR at a time

## Custom Actions

Workflows use several custom actions located in `.github/workflows/actions/`:
- `build` - Handles build process
- `tests` - Runs test suite
- `sonar` - Performs SonarQube analysis
- `clear-run-caches` - Manages workflow caches

## Cache Management

The workflow implements sophisticated cache management:
1. Each job type maintains its own cache
2. Caches are keyed by:
   - Cache version prefix (v1)
   - Runner OS
   - CMake configuration hash
   - Git ref
3. Dedicated jobs clean caches after workflow completion 