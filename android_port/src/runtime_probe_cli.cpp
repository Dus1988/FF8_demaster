#include "ff8android/address_map.h"
#include "ff8android/archive_parity.h"
#include "ff8android/filesystem_android.h"
#include "ff8android/runtime_target.h"

#include <iostream>

using namespace ff8android;

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Usage: ff8android_probe <main.zzz> <other.zzz> [address_map.csv]\n";
        return 1;
    }

    const auto parity = ProbeRuntimeParity(argv[1], argv[2]);

    std::cout << "Archive parity: " << (parity.archiveFormatMatches ? "OK" : "FAILED") << "\n";
    for (const auto& note : parity.notes) {
        std::cout << " - " << note << "\n";
    }

    if (argc >= 4) {
        AddressMap map;
        std::vector<std::string> errors;
        const bool loaded = map.LoadCsv(argv[3], &errors);
        std::cout << "Address map load: " << (loaded ? "OK" : "FAILED") << "\n";
        for (const auto& error : errors) {
            std::cout << " - " << error << "\n";
        }
    }

    AndroidStorageConfig storageCfg;
    storageCfg.packageName = "com.square_enix.android_googleplay.FF8R";
    storageCfg.externalStorageRoot = "/storage/emulated/0";
    storageCfg.internalDataRoot = "/data/data";
    const auto storage = ResolveStorageLayout(storageCfg);

    std::cout << "Storage app root: " << storage.appRoot.string() << "\n";
    std::cout << "Storage export root: " << storage.exportRoot.string() << "\n";

    RuntimeTarget target;
    target.packageName = storageCfg.packageName;
    target.abi = "arm64-v8a";
    target.renderer = RendererBackend::OpenGLES;
    target.strategy = InjectionStrategy::Zygisk;

    const auto issues = ValidateRuntimeTarget(target);
    if (!issues.empty()) {
        std::cout << "Runtime target incomplete:\n";
        for (const auto& issue : issues) {
            std::cout << " - " << issue << "\n";
        }
    }

    return parity.archiveFormatMatches ? 0 : 2;
}
