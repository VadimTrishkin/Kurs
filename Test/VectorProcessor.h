#ifndef VECTORPROCESSOR_H
#define VECTORPROCESSOR_H

#include <vector>
#include <limits>

class VectorProcessor {
public:
    int16_t computeProduct(const std::vector<int16_t>& vector) {
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
};

#endif // VECTORPROCESSOR_H
