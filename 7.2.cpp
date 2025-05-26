#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
using namespace std;

class Character {
private:
    string name;
    int health;
    int attack;
    int defense;
    mutable mutex mtx;

public:
    Character(string name, int health, int attack, int defense)
        : name(move(name)), health(health), attack(attack), defense(defense) {}

    void takeDamage(int damage) {
        lock_guard<mutex> lock(mtx);
        health -= max(1, damage - defense);
        health = max(0, health);
    }

    bool isAlive() const {
        lock_guard<mutex> lock(mtx);
        return health > 0;
    }

    int getAttack() const {
        lock_guard<mutex> lock(mtx);
        return attack;
    }

    void displayInfo() const {
        lock_guard<mutex> lock(mtx);
        cout << name << " [ОЗ: " << health << ", АТК: " << attack << ", ЗАЩ: " << defense << "]\n";
    }

    void heal(int amount) {
        lock_guard<mutex> lock(mtx);
        health += amount;
        cout << name << " восстанавливает " << amount << " ОЗ\n";
    }
};

class Monster {
private:
    string name;
    int health;
    int attack;
    int defense;
    mutable mutex mtx;

public:
    Monster(string name, int health, int attack, int defense)
        : name(move(name)), health(health), attack(attack), defense(defense) {}

    // Запрещаем копирование
    Monster(const Monster&) = delete;
    Monster& operator=(const Monster&) = delete;

    // Разрешаем перемещение
    Monster(Monster&& other) noexcept {
        lock_guard<mutex> lock(other.mtx);
        name = move(other.name);
        health = other.health;
        attack = other.attack;
        defense = other.defense;
    }

    void takeDamage(int damage) {
        lock_guard<mutex> lock(mtx);
        health -= max(1, damage - defense);
        health = max(0, health);
    }

    bool isAlive() const {
        lock_guard<mutex> lock(mtx);
        return health > 0;
    }

    int getAttack() const {
        lock_guard<mutex> lock(mtx);
        return attack;
    }

    void displayInfo() const {
        lock_guard<mutex> lock(mtx);
        cout << name << " [ОЗ: " << health << ", АТК: " << attack << ", ЗАЩ: " << defense << "]\n";
    }

    string getName() const {
        lock_guard<mutex> lock(mtx);
        return name;
    }
};

class MonsterSpawner {
private:
    vector<unique_ptr<Monster>> monsters;
    mutable mutex monstersMutex;
    condition_variable cv;
    atomic<bool> stopFlag{ false };
    thread spawnThread;
    thread battleThread;

    vector<string> names = { "Гоблин", "Орк", "Тролль", "Скелет", "Зомби" };

    unique_ptr<Monster> generateRandomMonster() {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> healthDist(30, 80);
        uniform_int_distribution<> attackDist(5, 20);
        uniform_int_distribution<> defenseDist(0, 10);
        uniform_int_distribution<> nameDist(0, names.size() - 1);

        return make_unique<Monster>(
            names[nameDist(gen)],
            healthDist(gen),
            attackDist(gen),
            defenseDist(gen)
        );
    }

    void spawnMonsters() {
        while (!stopFlag) {
            this_thread::sleep_for(chrono::seconds(3));

            auto newMonster = generateRandomMonster();
            {
                lock_guard<mutex> lock(monstersMutex);
                monsters.push_back(move(newMonster));
                cout << "Появился новый монстр: ";
                monsters.back()->displayInfo();
            }
            cv.notify_one();
        }
    }

    void processBattles(Character& hero) {
        while (!stopFlag && hero.isAlive()) {
            unique_ptr<Monster> currentMonster;

            {
                unique_lock<mutex> lock(monstersMutex);
                cv.wait(lock, [this] { return !monsters.empty() || stopFlag; });

                if (!monsters.empty()) {
                    currentMonster = move(monsters.back());
                    monsters.pop_back();
                }
            }

            if (currentMonster) {
                cout << "\n=== НАЧАЛО БОЯ ===" << endl;
                cout << "Герой вступает в бой с: ";
                currentMonster->displayInfo();

                while (hero.isAlive() && currentMonster->isAlive()) {
                    // Герой атакует
                    currentMonster->takeDamage(hero.getAttack());
                    if (!currentMonster->isAlive()) {
                        cout << "Монстр побежден!\n";
                        hero.heal(5);
                        break;
                    }

                    // Монстр атакует
                    hero.takeDamage(currentMonster->getAttack());
                    if (!hero.isAlive()) {
                        cout << "Герой пал в бою!\n";
                        break;
                    }

                    this_thread::sleep_for(chrono::seconds(1));
                }
                cout << "=== КОНЕЦ БОЯ ===\n\n";
            }
        }
    }

public:
    MonsterSpawner(Character& hero) {
        spawnThread = thread(&MonsterSpawner::spawnMonsters, this);
        battleThread = thread(&MonsterSpawner::processBattles, this, ref(hero));
    }

    ~MonsterSpawner() {
        stopFlag = true;
        cv.notify_all();
        if (spawnThread.joinable()) spawnThread.join();
        if (battleThread.joinable()) battleThread.join();
    }

    void displayStatus(const Character& hero) const {
        lock_guard<mutex> lock(monstersMutex);
        cout << "\n=== ТЕКУЩЕЕ СОСТОЯНИЕ ===" << endl;
        hero.displayInfo();
        cout << "Монстров в очереди: " << monsters.size() << endl;
        for (const auto& m : monsters) {
            m->displayInfo();
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Character hero("Герой", 100, 20, 10);

    {
        MonsterSpawner spawner(hero);

        while (hero.isAlive()) {
            this_thread::sleep_for(chrono::seconds(2));
            spawner.displayStatus(hero);
        }
    }

    cout << "=== ИГРА ОКОНЧЕНА ===" << endl;
    return 0;
}