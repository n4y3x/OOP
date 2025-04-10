#include <iostream>
#include <string>
using namespace std;

class Character {
private:
    string name;
    int health;
    int attack;
    int defense;
    const int maxHealth = 100;

public:
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    int getHealth() const {
        return health;
    }

    void displayInfo() const {
        cout << "Имя: " << name << ", ОЗ: " << health
                  << ", Атака: " << attack << ", Защита: " << defense << endl;
    }

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            cout << name << " атакует " << enemy.name << " на " << damage << " урона!" << endl;
        } else {
            cout << name << " атакует " << enemy.name << ", но это не сработало!" << endl;
        }
    }

    // метод для лечения
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
        cout << name << " полечился на " << amount << " ОЗ. Текущее ОЗ: " << health << endl;
    }

    // метод для получения урона
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
        cout << name << " получил " << amount << " урона. Текущее ОЗ: " << health << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Character hero("Чувачок", 100, 20, 10);
    Character monster("Гоблин", 50, 15, 5);

    hero.displayInfo();
    monster.displayInfo();

    hero.attackEnemy(monster);
    monster.displayInfo();

    monster.takeDamage(10);
    hero.heal(30);
    hero.displayInfo();

    return 0;
}