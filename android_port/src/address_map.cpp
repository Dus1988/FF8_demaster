#include "ff8android/address_map.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace ff8android {
namespace {

std::string Trim(const std::string& value)
{
    auto begin = value.begin();
    auto end = value.end();
    while (begin != end && std::isspace(static_cast<unsigned char>(*begin))) {
        ++begin;
    }
    while (begin != end && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
        --end;
    }
    return std::string(begin, end);
}

bool ParseAddress(const std::string& input, uint64_t* out)
{
    if (out == nullptr) {
        return false;
    }

    std::string normalized = Trim(input);
    int base = 10;
    if (normalized.rfind("0x", 0) == 0 || normalized.rfind("0X", 0) == 0) {
        normalized = normalized.substr(2);
        base = 16;
    }

    try {
        *out = std::stoull(normalized, nullptr, base);
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace

bool AddressMap::LoadCsv(const std::filesystem::path& filePath, std::vector<std::string>* errors)
{
    records_.clear();

    std::ifstream input(filePath);
    if (!input.is_open()) {
        if (errors != nullptr) {
            errors->emplace_back("Failed to open address map file");
        }
        return false;
    }

    std::string line;
    size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        const std::string trimmed = Trim(line);
        if (trimmed.empty() || trimmed.rfind("#", 0) == 0) {
            continue;
        }

        std::stringstream ss(trimmed);
        std::string build;
        std::string abi;
        std::string symbol;
        std::string address;

        if (!std::getline(ss, build, ',') ||
            !std::getline(ss, abi, ',') ||
            !std::getline(ss, symbol, ',') ||
            !std::getline(ss, address)) {
            if (errors != nullptr) {
                errors->push_back("Invalid CSV row format at line " + std::to_string(lineNumber));
            }
            continue;
        }

        AddressRecord record;
        record.build = Trim(build);
        record.abi = Trim(abi);
        record.symbol = Trim(symbol);
        if (!ParseAddress(address, &record.address)) {
            if (errors != nullptr) {
                errors->push_back("Invalid address value at line " + std::to_string(lineNumber));
            }
            continue;
        }

        records_.push_back(std::move(record));
    }

    return !records_.empty();
}

std::optional<uint64_t> AddressMap::Lookup(const std::string& build,
                                           const std::string& abi,
                                           const std::string& symbol) const
{
    const auto it = std::find_if(records_.begin(), records_.end(), [&](const AddressRecord& record) {
        return record.build == build && record.abi == abi && record.symbol == symbol;
    });

    if (it == records_.end()) {
        return std::nullopt;
    }
    return it->address;
}

const std::vector<AddressRecord>& AddressMap::Records() const
{
    return records_;
}

} // namespace ff8android
