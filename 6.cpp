#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class SafeQueue {
private:
    queue<T> items;

public:
    void push(const T& item) {
        items.push(item);
    }

    T pop() {
        if (items.empty()) {
            throw runtime_error("Ошибка: попытка извлечь из пустой очереди!");
        }
        T item = items.front();
        items.pop();
        return item;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    SafeQueue<int> queue;

    try {
        queue.push(10);
        queue.push(20);
        cout << "Извлечено: " << queue.pop() << endl;
        cout << "Извлечено: " << queue.pop() << endl;
        cout << "Извлечено: " << queue.pop() << endl; // Вызовет исключение
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}