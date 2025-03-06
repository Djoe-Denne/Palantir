#pragma once

#include <gmock/gmock.h>
namespace palantir::test {

class PalantirMock {
public:
    PalantirMock() = default;
    virtual ~PalantirMock() {
        reset();
    }

    virtual auto reset() -> void
    {
        EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(this));
    }
};

} // namespace palantir::test 