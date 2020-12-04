#pragma once

class NonCopyable {

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(NonCopyable &&) = default;
    NonCopyable &operator=(NonCopyable &&) = default;

    NonCopyable(NonCopyable const &) = delete;
    NonCopyable &operator=(NonCopyable const &) = delete;
};

