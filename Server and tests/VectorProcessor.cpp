#include "VectorProcessor.h"
#include <limits>

/**
 * @file VectorProcessor.cpp
 * @brief Реализация класса VectorProcessor для обработки векторов целых чисел.
 * @author Тришкин В.Д.
 */

/**
 * @brief Вычисляет произведение элементов вектора.
 * 
 * Этот метод принимает вектор целых чисел типа int16_t и вычисляет его произведение. Если в процессе вычисления происходит переполнение (значение выходит за пределы диапазона int16_t),
 * возвращается максимальное или минимальное значение для типа int16_t.
 * 
 * @param vector Вектор целых чисел типа int16_t, элементы которого необходимо перемножить.
 * 
 * @return Возвращает результат произведения элементов вектора, приведенный к типу int16_t.
 *         В случае переполнения возвращается максимально возможное значение для типа int16_t
 *         или минимально возможное, в зависимости от направления переполнения.
 */
int16_t VectorProcessor::computeProduct(const std::vector<int16_t>& vector) {
    int32_t product = 1;  

    // Перебор всех элементов вектора
    for (const auto& elem : vector) {
        product *= elem; 

        // Проверка на переполнение (большее значение, чем максимально возможное для int16_t)
        if (product > std::numeric_limits<int16_t>::max()) {
            return std::numeric_limits<int16_t>::max();  
        }

        // Проверка на переполнение (меньшее значение, чем минимально возможное для int16_t)
        if (product < std::numeric_limits<int16_t>::min()) {
            return std::numeric_limits<int16_t>::min(); 
        }
    }
    return static_cast<int16_t>(product);
}
