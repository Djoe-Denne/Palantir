#pragma once

namespace palantir::prompt {

constexpr auto SAURON_IMPLEM_REQUEST_PROMPT = R"(
Focus on Left side (Editor) where the code is written. you should find a comment that explain an exercise or task.
Implement this in Java 17 way. 
* Code must be robust and functional.
* Use Stream over for when possible.
* Use Optional over null checks.
* Respect any method signature requirements.
)";

constexpr auto SAURON_FIX_ERRORS_REQUEST_PROMPT = R"(
Focus on Right side (Console) where the errors are shown. Read them and fix the code in the left side (Editor).
)";

constexpr auto SAURON_VALIDATE_WITH_TESTS_REQUEST_PROMPT = R"(
Focus on Left side (Editor) where the code is written. Test the code with the tests in main method. 
* Use assertion when possible.
* make failure message meaningful.
* Don't use junit 5.
)";

constexpr auto SAURON_FIX_TEST_FAILURES_REQUEST_PROMPT = R"(
Focus on Right side (Console) where the test failures are shown. Read them and fix the code in the left side (Editor).
* Make failure message meaningful.
* Don't use junit 5.
)";

constexpr auto SAURON_HANDLE_TODOS_REQUEST_PROMPT = R"(
Focus on Left side (Editor) where the code is written. you should find a TODO comment that explain an exercise or task.
* Implement the task.
* Make the code robust and functional.
* Use Stream over for when possible.
* Use Optional over null checks.
* Respect any method signature requirements.
)";

} // namespace palantir::prompt

