#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <algorithm>

namespace interview_cheater::utils {

class StringUtils {
public:
    StringUtils() = delete;

    /**
     * @brief Convert a string to uppercase.
     * @param str The string to convert.
     * @return The uppercase string.
     */
    static auto toUpper(std::string str) -> std::string {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
};

}  // namespace interview_cheater::utils

#endif  // STRING_UTILS_HPP
