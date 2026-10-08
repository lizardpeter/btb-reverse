#include "btb/bobs_band_persistence.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

int main() {
    using namespace btb::bobs_band;
    namespace fs = std::filesystem;

    const auto root = fs::temp_directory_path() / "btb_band_reconstruction_io";
    fs::create_directories(root);

    auto missing = load_sequence_file(root, Conductor::Bob, 2);
    assert(!missing.opened);
    assert(missing.composition.structurally_valid());
    assert(missing.composition.at({0,0}) == -1);

    Composition original;
    assert(original.place({0,0},1));
    assert(original.place({4,23},8));
    assert(save_sequence_file(root, Conductor::Wendy,3, original));
    const auto loaded = load_sequence_file(root, Conductor::Wendy,3);
    assert(loaded.opened);
    assert(loaded.complete_read);
    assert(loaded.structurally_valid);
    assert(loaded.composition.cells() == original.cells());
    assert(fs::file_size(root / "musicwendy4.txt") == 480);

    // Native reads one signed int32 at a time. A file that ends after the
    // first cell leaves every unfilled entry at the initializer's -1.
    {
        std::ofstream truncated(root / "musicbob2.txt",
                                std::ios::binary | std::ios::trunc);
        const std::uint8_t long_roley[]{1,0,0,0};
        truncated.write(reinterpret_cast<const char*>(long_roley),4);
    }
    const auto partial=load_sequence_file(root, Conductor::Bob,1);
    assert(partial.opened && !partial.complete_read);
    assert(partial.composition.at({0,0}) == 1);
    assert(partial.composition.at({0,1}) == -1);
    assert(!partial.structurally_valid); // no required continuation
    assert(!partial.error.empty());

    assert(write_native_last_file(root, Conductor::FarmerPickles));
    std::ifstream in(root / "last.txt", std::ios::binary);
    const std::vector<std::uint8_t> sidecar(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    assert(sidecar.size() == 20);
    for (std::size_t i=0; i<sidecar.size(); i+=4) {
        assert(sidecar[i] == 2);
        assert(sidecar[i+1] == 0);
        assert(sidecar[i+2] == 0);
        assert(sidecar[i+3] == 0);
    }

    fs::remove_all(root);
}
