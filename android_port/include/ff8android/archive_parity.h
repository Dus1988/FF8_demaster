#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace ff8android {

struct ArchiveEntryInfo {
    std::string fileName;
    uint64_t offset = 0;
    uint32_t size = 0;
};

struct ArchiveProbeResult {
    bool valid = false;
    std::string path;
    uint32_t fileCount = 0;
    uint64_t archiveSize = 0;
    std::vector<ArchiveEntryInfo> entries;
    std::vector<std::string> issues;
};

struct RuntimeParityResult {
    bool archiveFormatMatches = false;
    ArchiveProbeResult mainArchive;
    ArchiveProbeResult otherArchive;
    std::vector<std::string> notes;
};

ArchiveProbeResult ProbeZzzArchive(const std::filesystem::path& archivePath);
RuntimeParityResult ProbeRuntimeParity(const std::filesystem::path& mainArchivePath,
                                       const std::filesystem::path& otherArchivePath);

} // namespace ff8android
