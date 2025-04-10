#include <iostream>
#include <string>
using namespace std;

class Entity {
protected:
    string name;
    int health;

public:
    Entity(const string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        cout << "Имя: " << name << ", ОЗ: " << health << endl;
    }
    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience;

public:
    Player(const string& n, int h, int exp) : Entity(n, h), experience(exp) {}
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "Опыт: " << experience << endl;
    }
};

class Enemy : public Entity {
protected:
    string type;

public:
    Enemy(const string& n, int h, const string& t) : Entity(n, h), type(t) {}
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "Тип: " << type << endl;
    }
};

class Boss : public Enemy {
private:
    string specialAbility;

public:
    Boss(const string& n, int h, const string& t, const string& ability)
        : Enemy(n, h, t), specialAbility(ability) {}
    void displayInfo() const override {
        Enemy::displayInfo();
        cout << "Особая способность: " << specialAbility << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Player hero("Чувачок", 100, 0);
    Enemy monster("Гоблин-разведчик", 50, "Лесовик");
    Boss goblinWarlord("Варлорд", 200, "Лидер клана", "Боевой клич");

    hero.displayInfo();
    monster.displayInfo();
    goblinWarlord.displayInfo();

    return 0;
}