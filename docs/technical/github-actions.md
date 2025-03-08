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
   - Cleans up caches after completion

2. **Code Quality Master** (`code-quality-master.yml`)
   - Runs code quality checks on the master branch
   - Triggered on pushes to master

3. **Code Quality PR** (`code-quality-pr.yml`)
   - Runs code quality checks on PRs when requested via comment
   - Creates and updates check runs for visibility
   - Posts lint reports as PR comments

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

### 2. Code Quality (`code-quality`)

Runs on Ubuntu to ensure consistent code quality checks.

Steps:
1. For PRs:
   - Triggered by "run code quality" comment
   - Creates in-progress check status
   - Checks out PR code with full history
   - Runs quality checks
   - Posts lint report as comment
   - Updates check status with results

2. For master:
   - Runs automatically on push
   - Performs quality checks
   - Uses same quality action as PR workflow

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
- `quality` - Performs code quality checks
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