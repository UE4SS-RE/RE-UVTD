#include <format>
#include <DynamicOutput/Output.hpp>
#include <glaze/glaze.hpp>
#include <UVTD/Helpers.hpp>
#include <Helpers/String.hpp>

namespace RC::UVTD
{
    auto to_string_type(const char* c_str) -> File::StringType
    {
#if RC_IS_ANSI == 1
        return File::StringType(c_str);
#else
        size_t count = strlen(c_str) + 1;
        wchar_t* converted_method_name = new wchar_t[count];

        size_t num_of_char_converted = 0;
        mbstowcs_s(&num_of_char_converted, converted_method_name, count, c_str, count);

        auto converted = File::StringType(converted_method_name);

        delete[] converted_method_name;

        return converted;
#endif
    }

    auto change_prefix(File::StringType input, bool is_425_plus) -> std::optional<File::StringType>
    {
        for (const auto& prefixed : uprefix_to_fprefix)
        {
            for (size_t index = input.find(prefixed); index != input.npos; index = input.find(prefixed))
            {
                if (is_425_plus) return {};
                input.replace(index, 1, STR("F"));
                index++;
            }
        }

        return input;
    }



    void SettingsManager::Read_Settings()
    {
        UVTDSettings s{};
        std::string buffer{};
        Output::send<LogLevel::Error>(STR("Finding Settings.\n"));
        auto file = std::ofstream("./UVTD-Settings.json", std::ios::in);
        if (file)
        {
            Output::send<LogLevel::Error>(STR("Settings found.\n"));
            glz::parse_error pe = glz::read_file<>(s, "./UVTD-Settings.json", buffer);
            if (pe)
            {
                std::string descriptive_error = glz::format_error(pe, buffer);

                size_t count = descriptive_error.size() + 1;
                wchar_t* converted_method_name = new wchar_t[count];

                size_t num_of_char_converted = 0;
                mbstowcs_s(&num_of_char_converted, converted_method_name, count, descriptive_error.data(), count);

                auto converted = File::StringViewType(converted_method_name);

                delete[] converted_method_name;
                
                Output::send<LogLevel::Error>(STR("{}\n\nError parsing settings file, please fix the file.\n"), converted);
            }
            Output::send<LogLevel::Default>(STR("Settings read.\n"));
        }
        else
        {
            glz::write<glz::opts{.prettify = true}>(s, buffer);
            glz::error_code ec = glz::buffer_to_file(buffer, "./UVTD-Settings.json");
            if (ec != glz::error_code::none)
            {
                auto arr = glz::detail::make_enum_to_string_array<glz::error_code>();
                auto error_type_str = arr[static_cast<uint32_t>(ec)];
                
                size_t count = error_type_str.size() + 1;
                wchar_t* converted_method_name = new wchar_t[count];

                size_t num_of_char_converted = 0;
                mbstowcs_s(&num_of_char_converted, converted_method_name, count, error_type_str.data(), count);

                auto converted = File::StringViewType(converted_method_name);

                delete[] converted_method_name;
                
                Output::send<LogLevel::Error>(STR("\nError {} when writing new settings file.\n"), converted);
            }
            Output::send<LogLevel::Default>(STR("Settings created. Please close the program and add settings options.\n"));
        }   
    }
    
}