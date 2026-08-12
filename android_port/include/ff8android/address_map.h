#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace ff8android {

struct AddressRecord {
    std::string build;
    std::string abi;
    std::string symbol;
    uint64_t address = 0;
};

class AddressMap {
public:
    bool LoadCsv(const std::filesystem::path& filePath, std::vector<std::string>* errors = nullptr);
    std::optional<uint64_t> Lookup(const std::string& build,
                                   const std::string& abi,
                                   const std::string& symbol) const;
    const std::vector<AddressRecord>& Records() const;

private:
    std::vector<AddressRecord> records_;
};

} // namespace ff8android
