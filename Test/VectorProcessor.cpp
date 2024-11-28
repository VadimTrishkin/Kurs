#include "VectorProcessor.h"
#include <limits>
#include <cstdint>

int16_t VectorProcessor::computeProduct(const std::vector<int16_t>& vector) {
    int32_t product = 1;

    for (const auto& elem : vector) {
        product *= elem;

        if (product > std::numeric_limits<int16_t>::max()) {
            return std::numeric_limits<int16_t>::max();
        }

        if (product < std::numeric_limits<int16_t>::min()) {
            return std::numeric_limits<int16_t>::min();
        }
    }
    return static_cast<int16_t>(product);
}
