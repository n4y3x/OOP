#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

// Класс для логирования
class Logger {
private:
    ofstream logFile;
public:
    Logger(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile) throw runtime_error("Failed to open log file");
    }

    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    void log(const string& message) {
        time_t now = time(nullptr);
        tm timeinfo;
        localtime_s(&timeinfo, &now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "[%H:%M:%S] ", &timeinfo);
        logFile << timeStr << message << endl;
        cout << timeStr << message << endl;
    }
};

// Базовый класс предмета
class Item {
protected:
    string name;
    string description;
public:
    Item(const string& n, const string& desc) : name(n), description(desc) {}
    virtual ~Item() = default;

    virtual void use() = 0;
    virtual string getType() const = 0;
    virtual string serialize() const = 0;
    virtual void deserialize(const string& data) = 0;

    string getName() const { return name; }
    string getDescription() const { return description; }
};

// Класс оружия
class Weapon : public Item {
private:
    int attackBonus;
public:
    Weapon(const string& n = "", const string& desc = "", int atk = 0)
        : Item(n, desc), attackBonus(atk) {}

    void use() override {
        cout << "Equipped " << name << " (+" << attackBonus << " ATK)" << endl;
    }

    string getType() const override { return "Weapon"; }
    int getAttackBonus() const { return attackBonus; }

    string serialize() const override {
        return "Weapon," + name + "," + description + "," + to_string(attackBonus);
    }

    void deserialize(const string& data) override {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);
        size_t pos3 = data.find(',', pos2 + 1);

        name = data.substr(pos1 + 1, pos2 - pos1 - 1);
        description = data.substr(pos2 + 1, pos3 - pos2 - 1);
        attackBonus = stoi(data.substr(pos3 + 1));
    }
};

// Класс зелья
class Potion : public Item {
private:
    int healAmount;
public:
    Potion(const string& n = "", const string& desc = "", int heal = 0)
        : Item(n, desc), healAmount(heal) {}

    void use() override {
        cout << "Used " << name << " (+" << healAmount << " HP)" << endl;
    }

    string getType() const override { return "Potion"; }
    int getHealAmount() const { return healAmount; }

    string serialize() const override {
        return "Potion," + name + "," + description + "," + to_string(healAmount);
    }

    void deserialize(const string& data) override {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);
        size_t pos3 = data.find(',', pos2 + 1);

        name = data.substr(pos1 + 1, pos2 - pos1 - 1);
        description = data.substr(pos2 + 1, pos3 - pos2 - 1);
        healAmount = stoi(data.substr(pos3 + 1));
    }
};

// Класс инвентаря
class Inventory {
private:
    vector<unique_ptr<Item>> items;
public:
    void addItem(unique_ptr<Item> item) {
        items.push_back(move(item));
    }

    void removeItem(int index) {
        if (index >= 0 && index < static_cast<int>(items.size())) {
            items.erase(items.begin() + index);
        }
    }

    void display() const {
        if (items.empty()) {
            cout << "Inventory is empty" << endl;
            return;
        }

        cout << "Inventory:" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i]->getName()
                << " (" << items[i]->getType() << ") - "
                << items[i]->getDescription() << endl;
        }
    }

    Item* getItem(int index) {
        if (index >= 0 && index < static_cast<int>(items.size())) {
            return items[index].get();
        }
        return nullptr;
    }

    size_t size() const { return items.size(); }

    string serialize() const {
        string result;
        for (const auto& item : items) {
            result += item->serialize() + "\n";
        }
        return result;
    }

    void deserialize(const string& data) {
        items.clear();
        istringstream iss(data);
        string line;

        while (getline(iss, line)) {
            if (line.empty()) continue;

            size_t pos = line.find(',');
            string type = line.substr(0, pos);

            if (type == "Weapon") {
                auto weapon = make_unique<Weapon>();
                weapon->deserialize(line);
                items.push_back(move(weapon));
            }
            else if (type == "Potion") {
                auto potion = make_unique<Potion>();
                potion->deserialize(line);
                items.push_back(move(potion));
            }
        }
    }
};

// Базовый класс персонажа
class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
public:
    Character(const string& n = "", int h = 0, int a = 0, int d = 0)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), level(1), experience(0) {}

    virtual ~Character() = default;

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    void gainExp(int exp) {
        experience += exp;
        if (experience >= 100) {
            levelUp();
        }
    }

    void levelUp() {
        level++;
        maxHealth += 10;
        health = maxHealth;
        attack += 2;
        defense += 1;
        experience = 0;
        cout << name << " leveled up to " << level << "!" << endl;
    }

    bool isAlive() const { return health > 0; }

    void showStats() const {
        cout << name << " (Lvl " << level << ")" << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "ATK: " << attack << " DEF: " << defense << endl;
        cout << "EXP: " << experience << "/100" << endl;
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getMaxHealth() const { return maxHealth; }

    string serialize() const {
        return name + "," + to_string(health) + "," + to_string(maxHealth) + "," +
            to_string(attack) + "," + to_string(defense) + "," +
            to_string(level) + "," + to_string(experience);
    }

    void deserialize(const string& data) {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);
        size_t pos3 = data.find(',', pos2 + 1);
        size_t pos4 = data.find(',', pos3 + 1);
        size_t pos5 = data.find(',', pos4 + 1);
        size_t pos6 = data.find(',', pos5 + 1);

        name = data.substr(0, pos1);
        health = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
        maxHealth = stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));
        attack = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        defense = stoi(data.substr(pos4 + 1, pos5 - pos4 - 1));
        level = stoi(data.substr(pos5 + 1, pos6 - pos5 - 1));
        experience = stoi(data.substr(pos6 + 1));
    }
};

// Класс игрока
class Player : public Character {
private:
    Inventory inventory;
public:
    Player(const string& n = "") : Character(n, 100, 10, 5) {}

    void performAttack(Character& enemy) {
        int damage = attack - enemy.getDefense();
        if (damage < 1) damage = 1;
        enemy.takeDamage(damage);
        cout << name << " attacks " << enemy.getName() << " for " << damage << " damage!" << endl;
    }

    void useItem(int index) {
        Item* item = inventory.getItem(index);
        if (!item) return;

        item->use();

        if (auto potion = dynamic_cast<Potion*>(item)) {
            heal(potion->getHealAmount());
        }
        else if (auto weapon = dynamic_cast<Weapon*>(item)) {
            attack += weapon->getAttackBonus();
        }

        inventory.removeItem(index);
    }

    void showInventory() const {
        inventory.display();
    }

    void addToInventory(unique_ptr<Item> item) {
        inventory.addItem(move(item));
    }

    size_t getInventorySize() const {
        return inventory.size();
    }

    string serialize() const {
        return Character::serialize() + "\n" + inventory.serialize();
    }

    void deserialize(const string& data) {
        size_t pos = data.find('\n');
        Character::deserialize(data.substr(0, pos));
        if (pos != string::npos) {
            inventory.deserialize(data.substr(pos + 1));
        }
    }
};

// Класс монстра
class Monster : public Character {
public:
    Monster(const string& n = "", int h = 0, int a = 0, int d = 0) : Character(n, h, a, d) {}

    void performAttack(Character& target) {
        int damage = attack - target.getDefense();
        if (damage < 1) damage = 1;
        target.takeDamage(damage);
        cout << name << " attacks " << target.getName() << " for " << damage << " damage!" << endl;
    }
};

// Конкретные типы монстров
class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 2) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 10, 5) {}
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 20, 10) {}
};

// Класс игры
class Game {
private:
    Player player;
    Logger logger;
    unique_ptr<Monster> currentEnemy;

    unique_ptr<Monster> createRandomMonster() {
        int type = rand() % 3;
        switch (type) {
        case 0: return make_unique<Goblin>();
        case 1: return make_unique<Skeleton>();
        case 2: return make_unique<Dragon>();
        default: return make_unique<Goblin>();
        }
    }

    void startBattle() {
        currentEnemy = createRandomMonster();
        logger.log("Battle started with " + currentEnemy->getName());

        while (currentEnemy && player.isAlive() && currentEnemy->isAlive()) {
            cout << "\n=== BATTLE ===" << endl;
            player.showStats();
            cout << "\nEnemy: ";
            currentEnemy->showStats();

            cout << "\n1. Attack\n2. Use item\n3. Flee\nChoose action: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                player.performAttack(*currentEnemy);

                if (!currentEnemy->isAlive()) {
                    int exp = 20 + currentEnemy->getAttack() * 2;
                    player.gainExp(exp);
                    logger.log("Defeated " + currentEnemy->getName() + " and gained " + to_string(exp) + " EXP");
                    giveReward();
                    currentEnemy.reset();
                    return;
                }
            }
            else if (choice == 2) {
                if (player.getInventorySize() > 0) {
                    player.showInventory();
                    cout << "Select item (1-" << player.getInventorySize() << "): ";
                    int item;
                    cin >> item;
                    player.useItem(item - 1);
                }
                else {
                    cout << "Inventory is empty!" << endl;
                }
                continue;
            }
            else if (choice == 3) {
                cout << "You fled from the battle!" << endl;
                currentEnemy.reset();
                return;
            }

            if (currentEnemy->isAlive()) {
                currentEnemy->performAttack(player);
            }
        }

        if (!player.isAlive()) {
            logger.log("Player was defeated!");
        }
    }

    void giveReward() {
        int rewardType = rand() % 2;
        if (rewardType == 0) {
            player.addToInventory(make_unique<Weapon>("Steel Sword", "Basic sword", 3));
            logger.log("Received Steel Sword as reward");
        }
        else {
            player.addToInventory(make_unique<Potion>("Health Potion", "Restores 30 HP", 30));
            logger.log("Received Health Potion as reward");
        }
    }

    void saveGame() {
        ofstream file("savegame.txt");
        if (!file) {
            logger.log("Error: Could not save game!");
            return;
        }
        file << player.serialize();
        file.close();
        logger.log("Game saved successfully");
    }

    void loadGame() {
        ifstream file("savegame.txt");
        if (!file) {
            logger.log("Error: Could not load game!");
            return;
        }
        string data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        player.deserialize(data);
        file.close();
        logger.log("Game loaded successfully");
    }

public:
    Game(const string& playerName) : player(playerName), logger("game.log") {
        srand(static_cast<unsigned>(time(nullptr)));
        // Starting items
        player.addToInventory(make_unique<Potion>("Health Potion", "Restores 30 HP", 30));
    }

    void start() {
        logger.log("Game started for " + player.getName());

        while (player.isAlive()) {
            cout << "\n=== MAIN MENU ===" << endl;
            cout << "1. Start battle\n2. Show inventory\n3. Save game\n4. Load game\n5. Exit\nChoose option: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                startBattle();
                break;
            case 2:
                player.showInventory();
                if (player.getInventorySize() > 0) {
                    cout << "Use item? (1-Yes, 2-No): ";
                    int use;
                    cin >> use;
                    if (use == 1) {
                        cout << "Select item (1-" << player.getInventorySize() << "): ";
                        int item;
                        cin >> item;
                        player.useItem(item - 1);
                    }
                }
                break;
            case 3:
                saveGame();
                break;
            case 4:
                loadGame();
                break;
            case 5:
                return;
            default:
                cout << "Invalid choice!" << endl;
            }
        }
    }
};

int main() {
    cout << "Enter your name: ";
    string name;
    getline(cin, name);

    Game game(name);
    game.start();

    return 0;
}