#define NOMINMAX

#include <stdexcept>
#include <format>
#include <thread>
#include <unordered_set>
#include <numeric>

#include <UVTD/UVTD.hpp>
#include <UVTD/VTableDumper.hpp>
#include <UVTD/MemberVarsDumper.hpp>
#include <UVTD/SolBindingsGenerator.hpp>
#include <UVTD/Helpers.hpp>
#include <UVTD/ExceptionHandling.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Input/Handler.hpp>
#include <Helpers/String.hpp>

#include <Windows.h>
#include <Psapi.h>
#include <dbghelp.h>

namespace RC::UVTD
{
    bool processing_events{false};
    Input::Handler input_handler{L"ConsoleWindowClass", L"UnrealWindow"};

    auto static event_loop_update() -> void
    {
        for (processing_events = true; processing_events;)
        {
            input_handler.process_event();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    auto main(DumpMode dump_mode) -> void
    {
        static std::vector<std::filesystem::path> pdbs_to_dump{
            "PDBs/4_11.pdb",
            "PDBs/4_12.pdb",
            "PDBs/4_13.pdb",
            "PDBs/4_14.pdb",
            "PDBs/4_15.pdb",
            "PDBs/4_16.pdb",
            "PDBs/4_17.pdb",
            "PDBs/4_18.pdb",
            "PDBs/4_19.pdb",
            "PDBs/4_20.pdb",
            "PDBs/4_21.pdb",
            "PDBs/4_22.pdb",
            "PDBs/4_23.pdb",
            "PDBs/4_24.pdb",
            "PDBs/4_25.pdb",
            "PDBs/4_26.pdb",
            "PDBs/4_27.pdb",
            // WITH_CASE_PRESERVING_NAMES
            "PDBs/4_27_CasePreserving.pdb",
            "PDBs/5_00.pdb",
            "PDBs/5_01.pdb",
            "PDBs/5_02.pdb",
        };

        CoInitializeEx(nullptr, COINIT_MULTITHREADED);

        for (const std::filesystem::path& pdb : pdbs_to_dump) {
            TRY([&] {
                {
                    switch (dump_mode) {
                    case DumpMode::VTable: {
                        Symbols symbols{ pdb };

                        VTableDumper dumper{ std::move(symbols) };
                        dumper.generate_code();
                        dumper.generate_files();
                        break;
                    }
                    case DumpMode::MemberVars: {
                        Symbols symbols{ pdb };

                        MemberVarsDumper dumper{ std::move(symbols) };
                        dumper.generate_code();
                        dumper.generate_files();
                        break;
                    }
                    case DumpMode::SolBindings: {
                        Symbols symbols{ pdb };

                        SolBindingsGenerator generator{ std::move(symbols) };
                        generator.generate_code();
                        generator.generate_files();
                    }
                    }

                    Output::send(STR("Code generated.\n"));
                }
            });
        }

        CoUninitialize();
    }
}