# GitHub Actions Workflow

This document describes how our GitHub Actions CI/CD pipeline works.

## Workflow Triggers

The workflow runs on:
- Push to `master` branch
- Pull requests targeting `master` branch

## Jobs

### 1. Code Quality (`code-quality`)

Runs on both Windows and macOS to catch platform-specific issues.

Steps:
1. Configure CMake in Release mode
2. Check code formatting (`format-check`)
3. Run static analysis (`lint-check`)
4. Analyze lint results (`lint-analyze`)
   - Fails if more than 5 warnings are found

### 2. Build (`build`)

Runs in parallel with code quality checks on both platforms.

Steps:
1. Configure CMake in Release mode
2. Build the project
3. Upload artifacts:
   - Windows: `InterviewCheater.exe`
   - macOS: `InterviewCheater.app`

## Artifacts

Each successful build produces platform-specific artifacts:
- Windows: `InterviewCheater-Windows`
- macOS: `InterviewCheater-macOS`

## Concurrency

- Only one workflow runs at a time per branch/PR
- New workflows cancel in-progress workflows on the same ref

## Environment Settings

Global environment variables:
- `FORCE_COLOR=1` - Enables colored output
- `TERM=xterm-256color` - Better formatting support 