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
        

    static std::unordered_set<std::string> NonCasePreservingVariantsList{
            { "4_27" },
    };

    static std::unordered_set<std::string> CasePreservingVariantsList{
            { "4_27_CasePreserving" },
    };

    

    static inline std::unordered_set<std::string> valid_udt_names{
            ("UScriptStruct::ICppStructOps"),
            ("UObjectBase"),
            ("UObjectBaseUtility"),
            ("UObject"),
            ("UStruct"),
            ("UGameViewportClient"),
            ("UScriptStruct"),
            ("FOutputDevice"),
            //("UConsole"),
            ("FMalloc"),
            ("FArchive"),
            ("FArchiveState"),
            ("AGameModeBase"),
            ("AGameMode"),
            ("AActor"),
            ("AHUD"),
            ("UPlayer"),
            ("ULocalPlayer"),
            ("FExec"),
            ("UField"),
            ("FField"),
            ("FProperty"),
            ("UProperty"),
            ("FNumericProperty"),
            ("UNumericProperty"),
            ("FMulticastDelegateProperty"),
            ("UMulticastDelegateProperty"),
            ("FObjectPropertyBase"),
            ("UObjectPropertyBase"),
            ("UStructProperty"),
            ("FStructProperty"),
            ("UArrayProperty"),
            ("FArrayProperty"),
            ("UMapProperty"),
            ("FMapProperty"),
            ("UWorld"),
            ("UFunction"),
            ("FBoolProperty"),
            ("UClass"),
            ("UEnum"),
            ("UBoolProperty"),
            ("FByteProperty"),
            ("UByteProperty"),
            ("FEnumProperty"),
            ("UEnumProperty"),
            ("FClassProperty"),
            ("UClassProperty"),
            ("FSoftClassProperty"),
            ("USoftClassProperty"),
            ("FDelegateProperty"),
            ("UDelegateProperty"),
            ("FInterfaceProperty"),
            ("UInterfaceProperty"),
            ("FFieldPathProperty"),
            ("FSetProperty"),
            ("USetProperty"),
    };

        static inline std::vector<std::string> s_types_to_not_dump{
        ("FUnversionedStructSchema"),
        ("ELifetimeCondition"),
        ("UAISystemBase"),
        ("FLevelCollection"),
        ("FThreadSafeCounter"),
        ("FWorldAsyncTraceState"),
        ("FDelegateHandle"),
        ("UAvoidanceManager"),
        ("FOnBeginTearingDownEvent"),
        ("UBlueprint"),
        ("UCanvas"),
        ("UActorComponent"),
        ("AController"),
        ("ULevel"),
        ("FPhysScene_Chaos"),
        ("APhysicsVolume"),
        ("UDemoNetDriver"),
        ("FEndPhysicsTickFunction"),
        ("FFXSystemInterface"),
        ("ERHIFeatureLevel"),
        ("EFlushLevelStreamingType"),
        ("ULineBatchComponent"),
        ("AGameState"),
        ("FOnGameStateSetEvent"),
        ("AAudioVolume"),
        ("FLatentActionManager"),
        ("FOnLevelsChangedEvent"),
        ("AParticleEventManager"),
        ("UNavigationSystem"),
        ("UNetDriver"),
        ("AGameNetworkManager"),
        ("ETravelType"),
        ("FDefaultDelegateUserPolicy"),
        ("TMulticastDelegate"),
        ("FActorsInitializedParams"),
        ("FOnBeginPostProcessSettings"),
        ("FIntVector"),
        ("FWorldPSCPool"),
        ("UMaterialParameterCollectionInstance"),
        ("FParticlePerfStats"),
        ("FWorldInGamePerformanceTrackers"),
        ("UPhysicsCollisionHandler"),
        ("UPhysicsFieldComponent"),
        ("FPhysScene"),
        ("APlayerController"),
        ("IInterface_PostProcessVolume"),
        ("FOnTickFlushEvent"),
        ("FSceneInterface"),
        ("FStartAsyncSimulationFunction"),
        ("FStartPhysicsTickFunction"),
        ("FOnNetTickEvent"),
        ("ETickingGroup"),
        ("FTickTaskLevel"),
        ("FTimerManager"),
        ("FURL"),
        ("UWorldComposition"),
        ("EWorldType"),
        ("FSubsystemCollection"),
        ("UWorldSubsystem"),
        ("FStreamingLevelsToConsider"),
        ("ACameraActor"),
        ("EMapPropertyFlags"),
        ("FScriptMapLayout"),
        ("EArrayPropertyFlags"),
        ("ICppClassTypeInfo"),
        ("FNativeFunctionLookup"),
        ("FGCReferenceTokenStream"),
        ("FWindowsCriticalSection"),
        ("FWindowsRWLock"),
        ("FRepRecord"),
        ("EClassCastFlags"),
        ("FAudioDeviceHandle"),
        ("TVector"),
        ("FScriptSetLayout"),
        ("FArchiveSerializedPropertyChain"),
        ("FArchiveCookData"),
        ("FFastPathLoadBuffer"),
        ("FTokenStreamOwner")
    };
        

    static inline std::vector<std::string> uprefix_to_fprefix{
            ("UProperty"),
            ("UMulticastDelegateProperty"),
            ("UObjectPropertyBase"),
            ("UStructProperty"),
            ("UArrayProperty"),
            ("UMapProperty"),
            ("UBoolProperty"),
            ("UByteProperty"),
            ("UNumericProperty"),
            ("UEnumProperty"),
            ("UClassProperty"),
            ("USoftClassProperty"),
            ("UDelegateProperty"),
            ("UInterfaceProperty"),
            ("USetProperty"),
    };

    static inline std::unordered_map<std::string, std::unordered_set<std::string>> s_private_variables{
        {
            ("FField"),
            {
                    ("ClassPrivate"),
                    ("NamePrivate"),
                    ("Next"),
                    ("Owner"),
            }
        },
    };

        class SettingsManager
        {
        public:
                struct UVTDSettings
                {
                        std::vector<ObjectItem> ObjectsToDump = s_object_items;
                        std::unordered_set<std::string> ValidUDTNames = valid_udt_names;
                        std::vector<std::string> TypesToSkip = RC::UVTD::s_types_to_not_dump;
                        std::unordered_map<std::string, std::unordered_set<std::string>> PrivateVariables = s_private_variables;
                        std::vector<std::string> UPrefixToFPrefix = uprefix_to_fprefix;
                        std::unordered_set<std::string> CasePreservingVariants = CasePreservingVariantsList;
                        std::unordered_set<std::string> NonCasePreservingVariants = NonCasePreservingVariantsList;
                };

                static void Read_Settings();

        };

    auto to_string_type(const char* c_str) -> File::StringType;
    auto change_prefix(std::string input, bool is_425_plus) -> std::optional<std::string>;
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