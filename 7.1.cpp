#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
using namespace std;

class Entity {
public:
    virtual ~Entity() = default;
    virtual string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getLevel() const = 0;
    virtual string serialize() const = 0;
    virtual void deserialize(const string& data) = 0;
    virtual void displayInfo() const = 0;
};

class Player : public Entity {
private:
    string name;
    int health;
    int level;

public:
    Player(const string& name = "", int health = 0, int level = 0)
        : name(name), health(health), level(level) {}

    string getName() const override { return name; }
    int getHealth() const override { return health; }
    int getLevel() const override { return level; }

    string serialize() const override {
        return name + "," + to_string(health) + "," + to_string(level);
    }

    void deserialize(const string& data) override {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);

        if (pos1 == string::npos || pos2 == string::npos) {
            throw runtime_error("Неверный формат данных");
        }

        name = data.substr(0, pos1);
        health = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
        level = stoi(data.substr(pos2 + 1));
    }

    void displayInfo() const override {
        cout << "Игрок: " << name << " (Здоровье: " << health
            << ", Уровень: " << level << ")" << endl;
    }
};

class Monster : public Entity {
private:
    string name;
    int health;
    int dangerLevel;

public:
    Monster(const string& name = "", int health = 0, int dangerLevel = 0)
        : name(name), health(health), dangerLevel(dangerLevel) {}

    string getName() const override { return name; }
    int getHealth() const override { return health; }
    int getLevel() const override { return dangerLevel; }

    string serialize() const override {
        return name + "," + to_string(health) + "," + to_string(dangerLevel);
    }

    void deserialize(const string& data) override {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);

        if (pos1 == string::npos || pos2 == string::npos) {
            throw runtime_error("Неверный формат данных");
        }

        name = data.substr(0, pos1);
        health = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
        dangerLevel = stoi(data.substr(pos2 + 1));
    }

    void displayInfo() const override {
        cout << "Монстр: " << name << " (Здоровье: " << health
            << ", Уровень угрозы: " << dangerLevel << ")" << endl;
    }
};

template <typename T>
class GameManager {
private:
    vector<unique_ptr<T>> entities;

public:
    void addEntity(unique_ptr<T> entity) {
        entities.push_back(move(entity));
    }

    void displayAll() const {
        cout << "\n=== Текущее состояние ===" << endl;
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Ошибка открытия файла для записи");
        }

        for (const auto& entity : entities) {
            file << entity->serialize() << "\n";
        }
        cout << "Данные успешно сохранены в " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            throw runtime_error("Ошибка открытия файла для чтения");
        }

        entities.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            auto entity = make_unique<T>();
            entity->deserialize(line);
            entities.push_back(move(entity));
        }
        cout << "Данные успешно загружены из " << filename << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    // Менеджер для игроков
    GameManager<Player> playerManager;
    playerManager.addEntity(make_unique<Player>("Герой", 100, 1));
    playerManager.addEntity(make_unique<Player>("Маг", 80, 3));
    playerManager.addEntity(make_unique<Player>("Воин", 120, 2));

    // Менеджер для монстров
    GameManager<Monster> monsterManager;
    monsterManager.addEntity(make_unique<Monster>("Гоблин", 50, 2));
    monsterManager.addEntity(make_unique<Monster>("Дракон", 200, 5));

    try {
        playerManager.saveToFile("players_save.txt");
        monsterManager.saveToFile("monsters_save.txt");

        GameManager<Player> loadedPlayers;
        GameManager<Monster> loadedMonsters;

        loadedPlayers.loadFromFile("players_save.txt");
        loadedMonsters.loadFromFile("monsters_save.txt");

        loadedPlayers.displayAll();
        loadedMonsters.displayAll();
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}