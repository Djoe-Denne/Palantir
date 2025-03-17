#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <algorithm>
#include <array>
#include <cwchar>
#include <locale>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "core_export.hpp"

namespace palantir::utils {

class PALANTIR_CORE_API StringUtils {
public:
#pragma warning(push)
#pragma warning(disable : 4251)
    static inline const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
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

#ifdef _WIN32
    /**
     * @brief Convert a wide string to a UTF-8 string.
     * @param wstr The wide string to convert.
     * @return The UTF-8 string.
     */
    static auto wToStr(const std::wstring& wstr) -> std::string {
        if (wstr.empty()) {
            return std::string();
        }
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), nullptr, 0,
                                              nullptr, nullptr);
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), str.data(), size_needed, nullptr,
                            nullptr);
        return str;
    }
#endif

    // Named constants for bitwise operations
    static constexpr unsigned char MASK_FC = 0xfcU;
    static constexpr unsigned char MASK_03 = 0x03U;
    static constexpr unsigned char MASK_F0 = 0xf0U;
    static constexpr unsigned char MASK_0F = 0x0fU;
    static constexpr unsigned char MASK_C0 = 0xc0U;
    static constexpr unsigned char MASK_3F = 0x3fU;
    static constexpr uint16_t SHIFT_2 = 2;
    static constexpr uint16_t SHIFT_4 = 4;
    static constexpr uint16_t SHIFT_6 = 6;
    static constexpr uint16_t ARRAY_SIZE_3 = 3;
    static constexpr uint16_t ARRAY_SIZE_4 = 4;

    static auto base64_encode(const std::vector<unsigned char>& data) -> std::string {
        std::string ret;
        size_t index = 0;
        size_t jndex = 0;
        std::array<unsigned char, ARRAY_SIZE_3> char_array_3{};
        std::array<unsigned char, ARRAY_SIZE_4> char_array_4{};
        size_t data_len = data.size();

        while (data_len > 0) {
            char_array_3[index++] = data[jndex++];
            data_len--;

            if (index == ARRAY_SIZE_3) {
                char_array_4[0] = (char_array_3[0] & MASK_FC) >> SHIFT_2;  // NOLINT
                char_array_4[1] =
                    ((char_array_3[0] & MASK_03) << SHIFT_4) | ((char_array_3[1] & MASK_F0) >> SHIFT_4);  // NOLINT
                char_array_4[2] =
                    ((char_array_3[1] & MASK_0F) << SHIFT_2) | ((char_array_3[2] & MASK_C0) >> SHIFT_6);  // NOLINT
                char_array_4[3] = char_array_3[2] & MASK_3F;

                for (size_t idx = 0; idx < ARRAY_SIZE_4; idx++) {
                    ret += base64_chars[char_array_4[idx]];
                }
                index = 0;
            }
        }

        if (index > 0) {
            for (size_t jdx = index; jdx < ARRAY_SIZE_3; jdx++) {
                char_array_3[jdx] = '\0';
            }

            char_array_4[0] = (char_array_3[0] & MASK_FC) >> SHIFT_2;  // NOLINT
            char_array_4[1] =
                ((char_array_3[0] & MASK_03) << SHIFT_4) | ((char_array_3[1] & MASK_F0) >> SHIFT_4);  // NOLINT
            char_array_4[2] =
                ((char_array_3[1] & MASK_0F) << SHIFT_2) | ((char_array_3[2] & MASK_C0) >> SHIFT_6);  // NOLINT
            char_array_4[3] = char_array_3[2] & MASK_3F;

            for (size_t jdx = 0; jdx < index + 1; jdx++) {
                ret += base64_chars[char_array_4[jdx]];
            }

            while (index++ < ARRAY_SIZE_3) {
                ret += '=';
            }
        }

        return ret;
    }

    struct StringHash {
        using is_transparent = void;  // Enables heterogeneous operations.

        std::size_t operator()(std::string_view sv) const {
            std::hash<std::string_view> hasher;
            return hasher(sv);
        }
    };

#pragma warning(pop)
};

}  // namespace palantir::utils

#endif  // STRING_UTILS_HPP
