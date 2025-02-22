#ifndef KEY_REGISTER_HPP
#define KEY_REGISTER_HPP

#include <map>
#include <string>

namespace interview_cheater::input {

    class KeyRegister {
    private:
        static std::map<std::string, int> keyMap;

    public:
        KeyRegister() = delete;

        static void registerKey(const std::string& key, int value);
        [[nodiscard]] static auto get(const std::string& key) -> int;
        [[nodiscard]] static auto hasKey(const std::string& key) -> bool;
    };

} // namespace interview_cheater::input

#endif // KEY_REGISTER_HPP