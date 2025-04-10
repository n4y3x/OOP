#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

class Entity {
protected:
    string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.health -= damage;
            cout << name << " атакует " << target.name << " и наносит " << damage << " урона!\n";
        } else {
            cout << name << " атакует " << target.name << ", но атака блокирована!\n";
        }
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        cout << "Имя: " << name << ", ОЗ: " << health
             << ", Атака: " << attack << ", Защита: " << defense << endl;
    }

    // Виртуальный метод для лечения
    virtual void heal(int amount) {
        health += amount;
        cout << name << " восстанавливает " << amount << " ОЗ. Текущее здоровье: " << health << endl;
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на критический удар (20%)
            if (rand() % 100 < 20) {
                damage *= 2;
                cout << "Критический удар! ";
            }
            target.health -= damage;
            cout << name << " атакует " << target.name << " и наносит " << damage << " урона!\n";
        } else {
            cout << name << " атакует " << target.name << ", но промахивается!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        cout << "[Персонаж] ";
        Entity::displayInfo();
    }

    // Переопределение метода heal
    void heal(int amount) override {
        health += amount;
        cout << name << " использует аптечку (+" << amount << " ОЗ). Текущее здоровье: " << health << endl;
    }
};

class Monster : public Entity {
public:
    Monster(const string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на ядовитую атаку (30%)
            if (rand() % 100 < 30) {
                damage += 5;
                cout << "Ядовитый укус! ";
            }
            target.health -= damage;
            cout << name << " атакует " << target.name << " и наносит " << damage << " урона!\n";
        } else {
            cout << name << " атакует " << target.name << ", но не может пробить защиту!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        cout << "[Монстр] ";
        Entity::displayInfo();
    }
};

class Boss : public Monster {
private:
    string specialAbility;

public:
    Boss(const string& n, int h, int a, int d, const string& ability)
        : Monster(n, h, a, d), specialAbility(ability) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // 40% шанс на особую атаку
            if (rand() % 100 < 40) {
                damage *= 3;
                cout << specialAbility << "! ";
            }
            target.health -= damage;
            cout << name << " атакует " << target.name << " и наносит " << damage << " урона!\n";
        } else {
            cout << name << " атакует " << target.name << ", но атака неэффективна!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        cout << "[БОСС] ";
        Entity::displayInfo();
        cout << "Способность: " << specialAbility << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    srand(static_cast<unsigned>(time(0)));

    // Создание объектов
    Character hero("Герой", 100, 20, 10);
    Monster goblin("Гоблин", 50, 15, 5);
    Boss goblinWarlord("Варлорд", 200, 30, 20, "Боевой клич");

    // Массив указателей на базовый класс
    Entity* entities[] = { &hero, &goblin, &goblinWarlord };

    // Демонстрация полиморфизма
    cout << "\n=== Информация о сущностях ===" << endl;
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    // Боевая демонстрация
    cout << "\n=== Бой ===" << endl;
    hero.attack(goblin);
    goblin.attack(hero);
    goblinWarlord.attack(hero);

    // Лечение
    cout << "\n=== Лечение ===" << endl;
    hero.heal(25);

    return 0;
}