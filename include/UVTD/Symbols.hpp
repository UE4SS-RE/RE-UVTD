#ifndef UNREALVTABLEDUMPER_SYMBOLS_HPP
#define UNREALVTABLEDUMPER_SYMBOLS_HPP

#include <unordered_map>
#include <map>
#include <vector>
#include <format>

#include <File/File.hpp>

#include <PDB_RawFile.h>
#include <PDB_DBIStream.h>
#include <PDB_TPIStream.h>

namespace RC::UVTD
{
    enum class DumpMode { VTable, MemberVars, SolBindings };

    struct SymbolNameInfo
    {
        bool valid_for_vtable{};
        bool valid_for_member_vars{};

        explicit SymbolNameInfo(bool valid_for_vtable, bool valid_for_member_vars) :
            valid_for_vtable(valid_for_vtable),
            valid_for_member_vars(valid_for_member_vars)
        {
        }
    };

    struct MemberVariable
    {
        std::string type;
        std::string name;
        int32_t offset;
    };

    struct FunctionParam
    {
        std::string type;

        auto to_string() const -> std::string
        {
            return std::format(("{}"), type);
        }
    };

    struct MethodSignature
    {
        std::string return_type;
        std::string name;
        std::vector<FunctionParam> params;
        bool const_qualifier;

        auto to_string() const -> std::string
        {
            std::string params_string{};

            for (size_t i = 0; i < params.size(); i++)
            {
                bool should_add_comma = i < params.size() - 1;
                params_string.append(std::format(std::string("{}{}"), params[i].to_string(), should_add_comma ? (", ") : ("")));
            }

            return std::format(("{} {}({}){};"), return_type, name, params_string, const_qualifier ? ("const") : (""));
        }
    };

    struct MethodBody
    {
        std::string name;
        MethodSignature signature;
        uint32_t offset;
        bool is_overload;
    };

    struct Class
    {
        std::string class_name;
        std::string class_name_clean;
        std::map<uint32_t, MethodBody> functions;
        // Key: Variable name
        std::map<std::string, MemberVariable> variables;
        uint32_t last_virtual_offset{};
        struct SymbolNameInfo validities{ false, false };
    };

    class Symbols {
    public:
        struct MemberVariable
        {
            std::string type;
            int32_t offset;
        };

        struct EnumEntry
        {
            std::string name;
            std::string name_clean;
            std::map<std::string, MemberVariable> variables;
        };		
        
        struct Class
        {
            std::string class_name;
            std::string class_name_clean;
            std::map<uint32_t, MethodBody> functions;
            // Key: Variable name
            std::map<std::string, MemberVariable> variables;
            uint32_t last_virtual_offset;
            struct SymbolNameInfo validities{ false, false };
        };

    public:
        std::filesystem::path pdb_file_path;
        File::Handle pdb_file_handle;
        std::span<uint8_t> pdb_file_map;

        PDB::RawFile pdb_file;
        PDB::DBIStream dbi_stream;
        bool is_425_plus;

        std::unordered_map<std::string, EnumEntry> enum_entries;
        std::unordered_map<std::string, Class> class_entries;

    public:
        Symbols() = delete;

        explicit Symbols(std::filesystem::path pdb_file_path);

        Symbols(const Symbols& other);

        Symbols& operator=(const Symbols& other);

    public:
        auto get_or_create_enum_entry(const std::string& symbol_name, const std::string& symbol_name_clean) -> EnumEntry&;
        auto get_or_create_class_entry(const std::string& symbol_name, const std::string& symbol_name_clean, const SymbolNameInfo& name_info) -> Class&;

        auto generate_method_signature(const PDB::TPIStream& tpi_stream, const PDB::CodeView::TPI::Record* function_record, std::string method_name) -> MethodSignature;

    public:
        auto static get_type_name(const PDB::TPIStream& tpi_stream, uint32_t record_index) -> std::string;
        auto static get_method_name(const PDB::CodeView::TPI::FieldList* method_record) -> std::string;
        auto static get_leaf_name(const char* data, PDB::CodeView::TPI::TypeRecordKind kind) -> std::string;

        auto static clean_name(std::string name) -> std::string;

        auto static is_virtual(PDB::CodeView::TPI::MemberAttributes attributes) -> bool;

    private:
        auto setup_symbol_loader() -> void;
    };
}

#endif