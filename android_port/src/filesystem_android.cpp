#include "ff8android/filesystem_android.h"

namespace ff8android {

AndroidStorageLayout ResolveStorageLayout(const AndroidStorageConfig& config)
{
    AndroidStorageLayout layout;

    const auto packageSuffix = std::filesystem::path(config.packageName);
    if (config.preferScopedExternalStorage && !config.externalStorageRoot.empty()) {
        layout.appRoot = config.externalStorageRoot / "Android" / "data" / packageSuffix / "files";
    } else {
        layout.appRoot = config.internalDataRoot / packageSuffix / "files";
    }

    layout.modRoot = layout.appRoot / "demaster";
    layout.exportRoot = layout.modRoot / "DEMASTER_EXP";
    layout.hashRoot = layout.exportRoot / "hashOutput";
    return layout;
}

} // namespace ff8android
