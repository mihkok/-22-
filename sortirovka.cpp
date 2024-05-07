#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

// Функция для сортировки части массива
void parallel_sort(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
    std::sort(begin, end);
}

int main() {
    // Создаем массив для сортировки
    std::vector<int> arr = {9, 3, 7, 1, 5, 6, 2, 8, 4};

    // Определяем количество потоков, которые будут использоваться
    unsigned int num_threads = std::thread::hardware_concurrency();

    // Вычисляем размер части массива для каждого потока
    std::size_t chunk_size = arr.size() / num_threads;

    // Создаем вектор потоков
    std::vector<std::thread> threads;

    // Запускаем потоки для сортировки каждой части массива
    for (unsigned int i = 0; i < num_threads; ++i) {
        auto start = arr.begin() + i * chunk_size;
        auto end = (i == num_threads - 1) ? arr.end() : start + chunk_size;
        threads.emplace_back(parallel_sort, start, end);
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Объединяем отсортированные части массива
    for (unsigned int i = 1; i < num_threads; ++i) {
        auto middle = arr.begin() + i * chunk_size;
        std::inplace_merge(arr.begin(), middle, arr.end());
    }

    // Выводим отсортированный массив
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
