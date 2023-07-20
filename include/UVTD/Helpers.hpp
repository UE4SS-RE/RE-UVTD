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
#include <Helpers/String.hpp>
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
        std::string ObjectName{};
        bool ValidForVTable{ true };
        bool ValidForMemberVar{ true };
    };

    

    


    // TODO: UConsole isn't found in all PDBs by this tool for some reason. Fix it.
    //       For now, to avoid problems, let's not generate for UConsole.
    static inline std::vector<ObjectItem> s_object_items{
            {("UObjectBase"), true, true },
            {("UObjectBaseUtility"), true, false },
            {("UObject"), true, false },
            {("UScriptStruct::ICppStructOps"), true, true },
            {("FOutputDevice"), true, true },
            {("UStruct"), true, true },
            {("UGameViewportClient"), true, true },
            {("UConsole"), true, true },
            {("FMalloc"), true, true },
            {("FArchive"), true, true },
            {("FArchiveState"), true, true },
            {("AGameModeBase"), true, true },
            {("AGameMode"), true, true },
            {("AActor"), true, true },
            {("AHUD"), true, true },
            {("UPlayer"), true, true },
            {("ULocalPlayer"), true, true },
            {("FExec"), true, false },
            {("FField"), true, true },
            {("UField"), true, true },
            {("FProperty"), true, true },
            {("UProperty"), true, true },
            {("FNumericProperty"), true, true },
            {("UNumericProperty"), true, true },
            {("FMulticastDelegateProperty"), true, true },
            {("UMulticastDelegateProperty"), true, true },
            {("FObjectPropertyBase"), true, true },
            {("UObjectPropertyBase"), true, true },
            {("UGameEngine"), true, true },
            {("UEngine"), true, true },
            /*{("FConsoleManager"), true, true },
            {("UDataTable"), true, true },
            {("FConsoleVariableBase"), true, true },
            {("FConsoleCommandBase"), true, true },*/

            {("UScriptStruct"), false, true },
            {("UWorld"), false, true },
            {("UFunction"), false, true },
            {("UClass"), false, true },
            {("UEnum"), false, true },
            {("FStructProperty"), false, true },
            {("UStructProperty"), false, true },
            {("FArrayProperty"), false, true },
            {("UArrayProperty"), false, true },
            {("FMapProperty"), false, true },
            {("UMapProperty"), false, true },
            {("FBoolProperty"), false, true },
            {("UBoolProperty"), false, true },
            {("FByteProperty"), false, true },
            {("UByteProperty"), false, true },
            {("FEnumProperty"), false, true },
            {("UEnumProperty"), false, true },
            {("FClassProperty"), false, true },
            {("UClassProperty"), false, true },
            {("FSoftClassProperty"), false, true },
            {("USoftClassProperty"), false, true },
            {("FDelegateProperty"), false, true },
            {("UDelegateProperty"), false, true },
            {("FInterfaceProperty"), false, true },
            {("UInterfaceProperty"), false, true },
            {("FFieldPathProperty"), false, true },
            {("FSetProperty"), false, true },
            {("USetProperty"), false, true },
    };

        class SettingsManager
        {
        public:
                struct UVTDSettings
                {
                        std::vector<ObjectItem> ObjectsToDump = s_object_items;
                        /*std::unordered_set<File::StringType> ValidUDTNames{};
                        std::vector<File::StringType> TypesToSkip{};
                        std::unordered_map<File::StringType, std::unordered_set<File::StringType>> PrivateVariables{};
                        std::vector<File::StringType> UPrefixToFPrefix{};*/
                };

                static void Read_Settings();

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
struct glz::meta<RC::UVTD::ObjectItem>
{
        using T = RC::UVTD::ObjectItem;
        static constexpr auto value = object(
                                             "ObjectName", &T::ObjectName,
                                             "ValidForVTable", &T::ValidForVTable,
                                             "ValidForMemberVar", &T::ValidForMemberVar
                                             );
};

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