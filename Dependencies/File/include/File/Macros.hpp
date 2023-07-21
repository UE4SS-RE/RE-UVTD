#ifndef RC_FILE_MACROS_HPP
#define RC_FILE_MACROS_HPP

// Set this to 1 to use ANSI (char*) instead of wide strings (wchar_t*)
#ifndef RC_IS_ANSI
#define RC_IS_ANSI 0
#endif





#if RC_IS_ANSI == 1
#define STR(str) str
#else
#define STR(str) L##str
#endif

#ifdef S
static_assert(false, "UE4SS define 'S' is already defined, please solve this");
#else
#define S(str) STR(str)
#endif

#define THROW_INTERNAL_FILE_ERROR(msg) \
                                   RC::File::Internal::StaticStorage::internal_error = true;\
                                   throw std::runtime_error{msg};

// This macro needs to be moved into the DynamicOutput system
/*
#define ASSERT_DEFAULT_OUTPUT_DEVICE_IS_VALID(param_device) \
                if (DefaultTargets::get_default_devices_ref().empty() || !param_device){                       \
                THROW_INTERNAL_OUTPUT_ERROR("[Output::send] Attempted to send but there were no default devices. Please specify at least one default device or construct a Targets object and supply your own devices.") \
                }
//*/


#include <string>

namespace RC::File
{
#if RC_IS_ANSI == 1
    using StringType = std::string;
    using StringViewType = std::string_view;
    using CharType = char;
    using StreamType = std::ifstream;

    constexpr auto ToString = [](auto&& numeric_value) constexpr -> decltype(auto) { return std::to_string(std::forward<decltype(numeric_value)>(numeric_value)); };
#else
    using StringType = std::wstring;
    using StringViewType = std::wstring_view;
    using CharType = wchar_t;
    using StreamType = std::wifstream;

    constexpr auto ToString = [](auto&& numeric_value) constexpr -> decltype(auto) { return std::to_wstring(std::forward<decltype(numeric_value)>(numeric_value)); };

    auto inline to_generic_string(const auto& input) -> StringType
    {
        if constexpr (std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<std::remove_cvref_t<decltype(input)>>>, StringType> || std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<std::remove_cvref_t<decltype(input)>>>, CharType>)
        {
            return input;
        }
        else
        {
            return to_string(input);
        }
    }
#endif
}

namespace RC
{
    using StringType = File::StringType;
    using StringViewType = File::StringViewType;
    using CharType = File::CharType;
    using StreamType = File::StreamType;

    constexpr auto ToString = File::ToString;
}

#endif //RC_FILE_MACROS_HPP
