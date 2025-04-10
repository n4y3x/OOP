#include <iostream>
#include <string>
using namespace std;

class Weapon {
private:
    string name;
    int damage;
    float weight;

public:
    Weapon(const string& n, int d, float w) : name(n), damage(d), weight(w) {
        cout << "Оружие " << name << " создано!" << endl;
    }

    ~Weapon() {
        cout << "Оружие " << name << " уничтожено!" << endl;
    }

    void displayInfo() const {
        cout << "Оружие: " << name << ", Урон: " << damage
             << ", Вес: " << weight << " кг" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Weapon sword("Драгонслеер", 60, 20);
    Weapon crossbow("Арбалет", 20, 1.2);

    sword.displayInfo();
    crossbow.displayInfo();

    return 0;
}