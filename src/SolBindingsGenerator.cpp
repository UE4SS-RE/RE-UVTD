#include <UVTD/SolBindingsGenerator.hpp>
#include <UVTD/VTableDumper.hpp>
#include <UVTD/MemberVarsDumper.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::UVTD
{
	auto SolBindingsGenerator::generate_code() -> void
	{
// 		TypeContainer container;
// 
// 		VTableDumper vtable_dumper{ symbols };
// 		vtable_dumper.generate_code();
// 		container.join(vtable_dumper.get_type_container());
// 
// 		MemberVarsDumper member_vars_dumper{ symbols };
// 		member_vars_dumper.generate_code();
// 		container.join(member_vars_dumper.get_type_container());
// 
// 		type_container = std::move(container);
	}

	auto SolBindingsGenerator::generate_files() -> void
	{
		static std::filesystem::path sol_bindings_output_path = "SolBindings";

		if (std::filesystem::exists(sol_bindings_output_path))
		{
			for (const auto& item : std::filesystem::directory_iterator(sol_bindings_output_path))
			{
				if (item.is_directory()) { continue; }
				if (item.path().extension() != STR(".hpp") && item.path().extension() != STR(".cpp")) { continue; }

				File::delete_file(item.path());
			}
		}

		for (const auto& [class_name, enum_entry] : type_container.get_enum_entries())
		{
			if (enum_entry.variables.empty()) { continue; }

			auto final_class_name_clean = enum_entry.name_clean;
			// Skipping UObject/UObjectBase because it needs to be manually implemented.
			if (enum_entry.name_clean == STR("UObjectBase")) { continue; }

			auto final_class_name = class_name;

			auto wrapper_header_file = sol_bindings_output_path / std::format(STR("SolBindings_{}.hpp"), final_class_name_clean);
			Output::send(STR("Generating file '{}'\n"), wrapper_header_file.wstring());
			Output::Targets<Output::NewFileDevice> header_wrapper_dumper;
			auto& wrapper_header_file_device = header_wrapper_dumper.get_device<Output::NewFileDevice>();
			wrapper_header_file_device.set_file_name_and_path(wrapper_header_file);
			wrapper_header_file_device.set_formatter([](File::StringViewType string) {
				return File::StringType{ string };
				});

			header_wrapper_dumper.send(STR("auto sol_class_{} = sol().new_usertype<{}>(\"{}\""), final_class_name, final_class_name, final_class_name);
			for (const auto& [variable_name, variable] : enum_entry.variables)
			{
				if (variable.type.find(STR("TBaseDelegate")) != variable.type.npos) { continue; }
				if (variable.type.find(STR("FUniqueNetIdRepl")) != variable.type.npos) { continue; }
				if (variable.type.find(STR("FPlatformUserId")) != variable.type.npos) { continue; }
				if (variable.type.find(STR("FVector2D")) != variable.type.npos) { continue; }
				if (variable.type.find(STR("FReply")) != variable.type.npos) { continue; }
				if (variable.type.find(STR("FUObjectCppClassStaticFunctions")) != variable.type.npos) { continue; }

				File::StringType final_variable_name = variable.name;

				if (variable.name == STR("EnumFlags"))
				{
					final_variable_name = STR("EnumFlags_Internal");
				}

				header_wrapper_dumper.send(STR(",\n    \"Get{}\", static_cast<{}&({}::*)()>(&{}::Get{})"), final_variable_name, variable.type, final_class_name, final_class_name, final_variable_name);
			}
			header_wrapper_dumper.send(STR("\n);\n"));
		}
	}
}