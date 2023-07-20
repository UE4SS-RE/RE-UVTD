#ifndef UNREALVTABLEDUMPER_HELPERS_HPP
#define UNREALVTABLEDUMPER_HELPERS_HPP

#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <vector>

#include <glaze/glaze.hpp>
#include <glaze/core/macros.hpp>
#include <UVTD/Symbols.hpp>
#include <File/File.hpp>

#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

namespace RC::UVTD
{
    static std::filesystem::path vtable_gen_output_path = "GeneratedVTables";
    static std::filesystem::path vtable_gen_output_include_path = vtable_gen_output_path / "generated_include";
    static std::filesystem::path vtable_gen_output_src_path = vtable_gen_output_path / "generated_src";
    static std::filesystem::path vtable_gen_output_function_bodies_path = vtable_gen_output_include_path / "FunctionBodies";
    static std::filesystem::path vtable_templates_output_path = "VTableLayoutTemplates";
    static std::filesystem::path virtual_gen_output_path = "GeneratedVirtualImplementations";
    static std::filesystem::path virtual_gen_output_include_path = virtual_gen_output_path / "generated_include";
    static std::filesystem::path virtual_gen_function_bodies_path = virtual_gen_output_include_path / "FunctionBodies";

    static std::filesystem::path member_variable_layouts_gen_output_path = "GeneratedMemberVariableLayouts";
    static std::filesystem::path member_variable_layouts_gen_output_include_path = member_variable_layouts_gen_output_path / "generated_include";
    static std::filesystem::path member_variable_layouts_gen_output_src_path = member_variable_layouts_gen_output_path / "generated_src";
    static std::filesystem::path member_variable_layouts_gen_function_bodies_path = member_variable_layouts_gen_output_include_path / "FunctionBodies";
    static std::filesystem::path member_variable_layouts_templates_output_path = "MemberVarLayoutTemplates";

        
    struct ObjectItem
    {
        File::StringType ObjectName{};
        bool ValidForVTable{ true };
        bool ValidForMemberVar{ true };
    };

    class SettingsManager
    {
    public:
            struct UVTDSettings
            {
                    std::vector<ObjectItem> ObjectsToDump{};
                    /*std::unordered_set<File::StringType> ValidUDTNames{};
                    std::vector<File::StringType> TypesToSkip{};
                    std::unordered_map<File::StringType, std::unordered_set<File::StringType>> PrivateVariables{};
                    std::vector<File::StringType> UPrefixToFPrefix{};*/
            };

            static void Read_Settings();

    };

    


    // TODO: UConsole isn't found in all PDBs by this tool for some reason. Fix it.
    //       For now, to avoid problems, let's not generate for UConsole.
    static inline std::vector<ObjectItem> s_object_items{
            {STR("UObjectBase"), true, true },
            {STR("UObjectBaseUtility"), true, false },
            {STR("UObject"), true, false },
            {STR("UScriptStruct::ICppStructOps"), true, true },
            {STR("FOutputDevice"), true, true },
            {STR("UStruct"), true, true },
            {STR("UGameViewportClient"), true, true },
            {STR("UConsole"), true, true },
            {STR("FMalloc"), true, true },
            {STR("FArchive"), true, true },
            {STR("FArchiveState"), true, true },
            {STR("AGameModeBase"), true, true },
            {STR("AGameMode"), true, true },
            {STR("AActor"), true, true },
            {STR("AHUD"), true, true },
            {STR("UPlayer"), true, true },
            {STR("ULocalPlayer"), true, true },
            {STR("FExec"), true, false },
            {STR("FField"), true, true },
            {STR("UField"), true, true },
            {STR("FProperty"), true, true },
            {STR("UProperty"), true, true },
            {STR("FNumericProperty"), true, true },
            {STR("UNumericProperty"), true, true },
            {STR("FMulticastDelegateProperty"), true, true },
            {STR("UMulticastDelegateProperty"), true, true },
            {STR("FObjectPropertyBase"), true, true },
            {STR("UObjectPropertyBase"), true, true },
            {STR("UGameEngine"), true, true },
            {STR("UEngine"), true, true },
            /*{STR("FConsoleManager"), true, true },
            {STR("UDataTable"), true, true },
            {STR("FConsoleVariableBase"), true, true },
            {STR("FConsoleCommandBase"), true, true },*/

            {STR("UScriptStruct"), false, true },
            {STR("UWorld"), false, true },
            {STR("UFunction"), false, true },
            {STR("UClass"), false, true },
            {STR("UEnum"), false, true },
            {STR("FStructProperty"), false, true },
            {STR("UStructProperty"), false, true },
            {STR("FArrayProperty"), false, true },
            {STR("UArrayProperty"), false, true },
            {STR("FMapProperty"), false, true },
            {STR("UMapProperty"), false, true },
            {STR("FBoolProperty"), false, true },
            {STR("UBoolProperty"), false, true },
            {STR("FByteProperty"), false, true },
            {STR("UByteProperty"), false, true },
            {STR("FEnumProperty"), false, true },
            {STR("UEnumProperty"), false, true },
            {STR("FClassProperty"), false, true },
            {STR("UClassProperty"), false, true },
            {STR("FSoftClassProperty"), false, true },
            {STR("USoftClassProperty"), false, true },
            {STR("FDelegateProperty"), false, true },
            {STR("UDelegateProperty"), false, true },
            {STR("FInterfaceProperty"), false, true },
            {STR("UInterfaceProperty"), false, true },
            {STR("FFieldPathProperty"), false, true },
            {STR("FSetProperty"), false, true },
            {STR("USetProperty"), false, true },
    };

    static inline std::unordered_map<File::StringType, std::unordered_set<File::StringType>> s_private_variables{
            {
                    STR("FField"),
                    {
                            STR("ClassPrivate"),
                            STR("NamePrivate"),
                            STR("Next"),
                            STR("Owner"),
                    }
            },
    };

    static std::unordered_set<File::StringType> NonCasePreservingVariants{
            {STR("4_27")},
    };

    static std::unordered_set<File::StringType> CasePreservingVariants{
            {STR("4_27_CasePreserving")},
    };

    static inline std::vector<File::StringType> UPrefixToFPrefix{
            STR("UProperty"),
            STR("UMulticastDelegateProperty"),
            STR("UObjectPropertyBase"),
            STR("UStructProperty"),
            STR("UArrayProperty"),
            STR("UMapProperty"),
            STR("UBoolProperty"),
            STR("UByteProperty"),
            STR("UNumericProperty"),
            STR("UEnumProperty"),
            STR("UClassProperty"),
            STR("USoftClassProperty"),
            STR("UDelegateProperty"),
            STR("UInterfaceProperty"),
            STR("USetProperty"),
    };

    static inline std::unordered_set<File::StringType> valid_udt_names{
            STR("UScriptStruct::ICppStructOps"),
            STR("UObjectBase"),
            STR("UObjectBaseUtility"),
            STR("UObject"),
            STR("UStruct"),
            STR("UGameViewportClient"),
            STR("UScriptStruct"),
            STR("FOutputDevice"),
            //STR("UConsole"),
            STR("FMalloc"),
            STR("FArchive"),
            STR("FArchiveState"),
            STR("AGameModeBase"),
            STR("AGameMode"),
            STR("AActor"),
            STR("AHUD"),
            STR("UPlayer"),
            STR("ULocalPlayer"),
            STR("FExec"),
            STR("UField"),
            STR("FField"),
            STR("FProperty"),
            STR("UProperty"),
            STR("FNumericProperty"),
            STR("UNumericProperty"),
            STR("FMulticastDelegateProperty"),
            STR("UMulticastDelegateProperty"),
            STR("FObjectPropertyBase"),
            STR("UObjectPropertyBase"),
            STR("UStructProperty"),
            STR("FStructProperty"),
            STR("UArrayProperty"),
            STR("FArrayProperty"),
            STR("UMapProperty"),
            STR("FMapProperty"),
            STR("UWorld"),
            STR("UFunction"),
            STR("FBoolProperty"),
            STR("UClass"),
            STR("UEnum"),
            STR("UBoolProperty"),
            STR("FByteProperty"),
            STR("UByteProperty"),
            STR("FEnumProperty"),
            STR("UEnumProperty"),
            STR("FClassProperty"),
            STR("UClassProperty"),
            STR("FSoftClassProperty"),
            STR("USoftClassProperty"),
            STR("FDelegateProperty"),
            STR("UDelegateProperty"),
            STR("FInterfaceProperty"),
            STR("UInterfaceProperty"),
            STR("FFieldPathProperty"),
            STR("FSetProperty"),
            STR("USetProperty"),
    };

    static inline std::vector<File::StringType> uprefix_to_fprefix{
            STR("UProperty"),
            STR("UMulticastDelegateProperty"),
            STR("UObjectPropertyBase"),
            STR("UStructProperty"),
            STR("UArrayProperty"),
            STR("UMapProperty"),
            STR("UBoolProperty"),
            STR("UByteProperty"),
            STR("UNumericProperty"),
            STR("UEnumProperty"),
            STR("UClassProperty"),
            STR("USoftClassProperty"),
            STR("UDelegateProperty"),
            STR("UInterfaceProperty"),
            STR("USetProperty"),
    };

    auto to_string_type(const char* c_str) -> File::StringType;
    auto change_prefix(File::StringType input, bool is_425_plus) -> std::optional<File::StringType>;
}

template <>
struct glz::meta<RC::UVTD::SettingsManager::UVTDSettings>
{
        using T = RC::UVTD::SettingsManager::UVTDSettings;
        static constexpr auto value = object(
                                             "ObjectsToDump", &T::ObjectsToDump/*,
                                             "ValidUDTNames", &T::ValidUDTNames,
                                             "TypesToSkip", &T::TypesToSkip,
                                             "PrivateVariables", &T::PrivateVariables,
                                             "UPrefixToFPrefix", &T::UPrefixToFPrefix*/
                                             );
};

#endif