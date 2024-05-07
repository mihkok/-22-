#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>

std::mutex mtx;


// Функция для нахождения суммы трех самых больших элементов в подмассиве
void findTop3Sum(const std::vector<int>& arr, int start, int end, std::vector<int>& top3) {
    std::vector<int> localTop3(3, std::numeric_limits<int>::min());

    for (int i = start; i < end; ++i) {
        if (arr[i] > localTop3[0]) {
            localTop3[2] = localTop3[1];
            localTop3[1] = localTop3[0];
            localTop3[0] = arr[i];
        } else if (arr[i] > localTop3[1]) {
            localTop3[2] = localTop3[1];
            localTop3[1] = arr[i];
        } else if (arr[i] > localTop3[2]) {
            localTop3[2] = arr[i];
        }
    }

    // Захватываем мьютекс для обновления общего массива с тремя наибольшими элементами
    std::lock_guard<std::mutex> guard(mtx);
    for (int i = 0; i < 3; ++i) {
        if (localTop3[i] > top3[0]) {
            top3[2] = top3[1];
            top3[1] = top3[0];
            top3[0] = localTop3[i];
        } else if (localTop3[i] > top3[1]) {
            top3[2] = top3[1];
            top3[1] = localTop3[i];
        } else if (localTop3[i] > top3[2]) {
            top3[2] = localTop3[i];
        }
    }
}

int main() {
    const int arraySize = 100;
    std::vector<int> array(arraySize);
    
    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 999); // Диапазон случайных чисел от 0 до 999

        // Заполняем массив случайными числами
    for (int i = 0; i < arraySize; ++i) {
        array[i] = dis(gen);
    }

    std::vector<int> top3(3, std::numeric_limits<int>::min());

    const int numThreads = 4; // Количество потоков
    std::vector<std::thread> threads(numThreads);

    // Запускаем потоки для обработки подмассивов
    int chunkSize = arraySize / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? arraySize : (i + 1) * chunkSize;
        threads[i] = std::thread(findTop3Sum, std::ref(array), start, end, std::ref(top3));
    }

    // Ждем завершения всех потоков
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    // Находим сумму трех самых больших элементов
    int sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum += top3[i];
    }

    std::cout << "Сумма трех самых больших элементов массива: " << sum << std::endl;

    return 0;
}
