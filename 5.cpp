#include <iostream>
#include <vector>
#include <queue>
using namespace std;

template <typename T>
class GameManager {
private:
    vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity.displayInfo();
        }
    }
};

template <typename T>
class Queue {
private:
    queue<T> items;

public:
    void push(const T& item) {
        items.push(item);
    }

    void pop() {
        if (items.empty()) {
            throw runtime_error("Очередь пуста!");
        }
        items.pop();
    }

    void display() const {
        queue<T> temp = items;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    //тест GameManager
    GameManager<Character> manager;
    manager.addEntity(Character("Герой", 100, 20, 10));
    manager.addEntity(Character("Враг", 50, 15, 5));
    cout << "=== Персонажи ===" << endl;
    manager.displayAll();

    //тест Queue
    Queue<string> stringQueue;
    stringQueue.push("Предмет1");
    stringQueue.push("Предмет2");
    cout << "Очередь строк: ";
    stringQueue.display();

    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    cout << "Очередь чисел: ";
    intQueue.display();

    return 0;
}