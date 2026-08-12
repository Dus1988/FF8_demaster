#include "ff8android/archive_parity.h"

#include <algorithm>
#include <fstream>

namespace ff8android {
namespace {

uint64_t FileSizeOrZero(std::ifstream& stream)
{
    const auto current = stream.tellg();
    stream.seekg(0, std::ios::end);
    const auto end = stream.tellg();
    stream.seekg(current);
    if (end < 0) {
        return 0;
    }
    return static_cast<uint64_t>(end);
}

} // namespace

ArchiveProbeResult ProbeZzzArchive(const std::filesystem::path& archivePath)
{
    ArchiveProbeResult result;
    result.path = archivePath.string();

    std::ifstream input(archivePath, std::ios::binary);
    if (!input.is_open()) {
        result.issues.emplace_back("Archive not found or unreadable");
        return result;
    }

    result.archiveSize = FileSizeOrZero(input);

    uint32_t fileCount = 0;
    input.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));
    if (!input || fileCount == 0) {
        result.issues.emplace_back("Invalid or empty archive header");
        return result;
    }

    result.fileCount = fileCount;
    result.entries.reserve(fileCount);

    for (uint32_t i = 0; i < fileCount; ++i) {
        uint32_t nameSize = 0;
        input.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        if (!input) {
            result.issues.emplace_back("Unexpected EOF while reading entry name size");
            return result;
        }

        if (nameSize == 0 || nameSize > 259) {
            result.issues.emplace_back("Entry name size is out of expected range");
            return result;
        }

        std::string fileName(nameSize, '\0');
        input.read(fileName.data(), static_cast<std::streamsize>(nameSize));

        ArchiveEntryInfo entry;
        entry.fileName = std::move(fileName);

        input.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        input.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size));
        if (!input) {
            result.issues.emplace_back("Unexpected EOF while reading entry metadata");
            return result;
        }

        if (entry.offset + entry.size > result.archiveSize) {
            result.issues.emplace_back("Entry data range is outside archive size");
        }

        result.entries.push_back(std::move(entry));
    }

    if (result.entries.empty()) {
        result.issues.emplace_back("Archive contains zero parsed entries");
        return result;
    }

    const bool hasRangeErrors = std::any_of(result.issues.begin(), result.issues.end(), [](const std::string& issue) {
        return issue.find("outside archive size") != std::string::npos;
    });

    result.valid = !hasRangeErrors;
    return result;
}

RuntimeParityResult ProbeRuntimeParity(const std::filesystem::path& mainArchivePath,
                                       const std::filesystem::path& otherArchivePath)
{
    RuntimeParityResult parity;
    parity.mainArchive = ProbeZzzArchive(mainArchivePath);
    parity.otherArchive = ProbeZzzArchive(otherArchivePath);

    if (!parity.mainArchive.valid) {
        parity.notes.emplace_back("main archive is not compatible with expected ZZZ layout");
    }
    if (!parity.otherArchive.valid) {
        parity.notes.emplace_back("other archive is not compatible with expected ZZZ layout");
    }

    parity.archiveFormatMatches = parity.mainArchive.valid && parity.otherArchive.valid;
    if (parity.archiveFormatMatches) {
        parity.notes.emplace_back("archive layout matches the Windows demaster parser assumptions");
    }

    return parity;
}

} // namespace ff8android
