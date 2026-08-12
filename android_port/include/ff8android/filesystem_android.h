#pragma once

#include <filesystem>
#include <string>

namespace ff8android {

struct AndroidStorageConfig {
    std::string packageName;
    std::filesystem::path internalDataRoot;
    std::filesystem::path externalStorageRoot;
    bool preferScopedExternalStorage = true;
};

struct AndroidStorageLayout {
    std::filesystem::path appRoot;
    std::filesystem::path modRoot;
    std::filesystem::path exportRoot;
    std::filesystem::path hashRoot;
};

AndroidStorageLayout ResolveStorageLayout(const AndroidStorageConfig& config);

} // namespace ff8android
