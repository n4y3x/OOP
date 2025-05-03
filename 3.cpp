#include <iostream>
#include <string>
using namespace std;

class Weapon {
private:
    string name;
    int damage;
    float weight;

public:
    Weapon(const string& n, int d, float w) : name(n), damage(d), weight(w) {}

    // Перегрузка оператора +
    Weapon operator+(const Weapon& other) const {
        return Weapon(name + "+" + other.name, damage + other.damage, weight + other.weight);
    }

    // Перегрузка оператора >
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Перегрузка оператора <<
    friend ostream& operator<<(ostream& os, const Weapon& w) {
        os << "Оружие: " << w.name << " (Урон: " << w.damage << ", Вес: " << w.weight << ")";
        return os;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    Weapon sword("Меч", 30, 3.5);
    Weapon bow("Лук", 20, 1.2);

    cout << sword << endl;
    cout << bow << endl;

    Weapon combined = sword + bow;
    cout << "Комбинированное оружие: " << combined << endl;

    if (sword > bow) {
        cout << sword.name << " сильнее чем " << bow.name << endl;
    } else {
        cout << bow.name << " сильнее чем " << sword.name << endl;
    }

    return 0;
}