#include "ff8android/runtime_target.h"

namespace ff8android {

std::vector<std::string> ValidateRuntimeTarget(const RuntimeTarget& target)
{
    std::vector<std::string> issues;

    if (target.packageName.empty()) {
        issues.emplace_back("packageName is required");
    }
    if (target.abi.empty()) {
        issues.emplace_back("abi is required (e.g. arm64-v8a)");
    }
    if (target.buildFingerprint.empty()) {
        issues.emplace_back("buildFingerprint is required for version-specific address maps");
    }
    if (target.renderer == RendererBackend::Unknown) {
        issues.emplace_back("renderer backend must be detected (OpenGLES or Vulkan)");
    }
    if (target.strategy == InjectionStrategy::Unknown) {
        issues.emplace_back("injection strategy must be chosen (Zygisk/Frida/ModifiedApk/RootPtrace)");
    }

    return issues;
}

} // namespace ff8android
