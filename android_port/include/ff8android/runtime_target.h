#pragma once

#include <string>
#include <vector>

namespace ff8android {

enum class RendererBackend {
    Unknown,
    OpenGLES,
    Vulkan,
};

enum class InjectionStrategy {
    Unknown,
    Zygisk,
    Frida,
    ModifiedApk,
    RootPtrace,
};

struct RuntimeTarget {
    std::string packageName;
    std::string buildFingerprint;
    std::string abi;
    RendererBackend renderer = RendererBackend::Unknown;
    InjectionStrategy strategy = InjectionStrategy::Unknown;
};

std::vector<std::string> ValidateRuntimeTarget(const RuntimeTarget& target);

} // namespace ff8android
