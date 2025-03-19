# Testing Pure Template HPP Classes

## Overview

This document outlines best practices for testing pure template classes defined in header files (`.hpp`) in C++. Testing template classes presents unique challenges, particularly for code coverage tools.

## Challenges with Template Classes

Template classes pose the following challenges for testing:

1. **Code is generated at compile-time**: Template code isn't compiled until it's instantiated with specific types.
2. **Coverage tools limitations**: Most coverage tools only track code that's compiled into binary modules.
3. **Implicit instantiations**: Template functions may be implicitly instantiated, making it difficult to ensure complete coverage.

## Solutions

### Explicit Template Instantiation

To ensure template classes are properly included in code coverage reports, use explicit template instantiations in your test files:

```cpp
// In your test file
#include "your_template_class.hpp"

// Explicitly instantiate the template with test types
template class YourTemplateClass<TestType>;

// Now write your tests...
```

Example from our codebase:

```cpp
// From content_manager_test.cpp
// necessary for testing coverage, as this class is templated, coverage tool will not count it if not present in one of the modules binary
template class ContentManager<MockView>;
```

### Coverage Tool Configuration

When using coverage tools like OpenCppCoverage, ensure:

1. You specify all relevant modules with the `--modules` parameter
2. You include both the main library and the test executable 
3. You use `--cover_children` if your tests spawn child processes

Example from our workflow:

```yaml
& "C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe" --export_type=binary:${{ inputs.build-dir }}\bin\coverage\palantir-core.cov --modules="${{ github.workspace }}\build\bin\palantir-core.dll" --modules="${{ github.workspace }}\build\bin\palantir_core_tests.exe" --cover_children ${{ inputs.build-dir }}\bin\palantir_core_tests.exe
```

## Best Practices

1. **Create mock types for testing**: Create simple mock implementations of the template parameters.
2. **Test with multiple types**: Instantiate and test templates with different types to ensure flexibility.
3. **Separate interface tests from implementation tests**: Test the public interface separately from implementation details.
4. **Use static assertions**: Where possible, use compile-time assertions to verify template constraints.

## Conclusion

Testing template classes requires careful attention to ensure code coverage tools properly track the instantiated template code. By using explicit instantiations and properly configuring coverage tools, we can achieve accurate coverage measurements for template code. 