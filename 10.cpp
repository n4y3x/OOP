#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>

// ===== Базовый класс User =====
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (accessLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = level;
    }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id << ", Доступ: " << accessLevel << "\n";
    }
};

// ===== Производные классы =====

class Student : public User {
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        std::cout << "[Студент] ";
        User::displayInfo();
        std::cout << "Группа: " << group << "\n";
    }
};

class Teacher : public User {
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& dept)
        : User(name, id, accessLevel), department(dept) {}

    void displayInfo() const override {
        std::cout << "[Преподаватель] ";
        User::displayInfo();
        std::cout << "Кафедра: " << department << "\n";
    }
};

class Administrator : public User {
    std::string role;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        std::cout << "[Администратор] ";
        User::displayInfo();
        std::cout << "Роль: " << role << "\n";
    }
};

// ===== Класс ресурса =====

class Resource {
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int level)
        : name(name), requiredAccessLevel(level) {
        if (name.empty()) throw std::invalid_argument("Имя ресурса не может быть пустым");
    }

    std::string getName() const { return name; }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void display() const {
        std::cout << "Ресурс: " << name << ", Требуемый доступ: " << requiredAccessLevel << "\n";
    }
};

// ===== Шаблонная система контроля доступа =====

template <typename T>
class AccessControlSystem {
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Resource>> resources;

public:
    void addUser(std::shared_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(std::shared_ptr<Resource> res) {
        resources.push_back(std::move(res));
    }

    void displayAllUsers() const {
        for (const auto& user : users)
            user->displayInfo();
    }

    void checkAccess(int userId, const std::string& resourceName) const {
        auto user = std::find_if(users.begin(), users.end(), [=](const std::shared_ptr<User>& u) {
            return u->getId() == userId;
            });

        auto resource = std::find_if(resources.begin(), resources.end(), [=](const std::shared_ptr<Resource>& r) {
            return r->getName() == resourceName;
            });

        if (user != users.end() && resource != resources.end()) {
            if ((*resource)->checkAccess(**user))
                std::cout << "Доступ разрешён\n";
            else
                std::cout << "Доступ запрещён\n";
        }
        else {
            std::cout << "Пользователь или ресурс не найден\n";
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream out(filename);
        for (const auto& user : users) {
            out << user->getId() << "," << user->getName() << "," << user->getAccessLevel() << "\n";
        }
        out.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string name;
            int id, level;
            char delim;
            if (iss >> id >> delim >> name >> delim >> level) {
                users.push_back(std::make_shared<Student>(name, id, level, "DefaultGroup"));
            }
        }
        in.close();
    }

    void searchUserByName(const std::string& query) const {
        for (const auto& user : users) {
            if (user->getName() == query) {
                user->displayInfo();
                return;
            }
        }
        std::cout << "Пользователь не найден\n";
    }

    void sortUsersByAccess() {
        std::sort(users.begin(), users.end(), [](const std::shared_ptr<User>& a, const std::shared_ptr<User>& b) {
            return a->getAccessLevel() > b->getAccessLevel();
            });
    }
};

// ===== main =====

int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        AccessControlSystem<User> system;

        system.addUser(std::make_shared<Student>("Иван", 1, 1, "ИС-101"));
        system.addUser(std::make_shared<Teacher>("Петров", 2, 3, "Математика"));
        system.addUser(std::make_shared<Administrator>("Сидоров", 3, 5, "Главный админ"));

        system.addResource(std::make_shared<Resource>("Библиотека", 1));
        system.addResource(std::make_shared<Resource>("Серверная", 5));

        system.displayAllUsers();
        system.checkAccess(1, "Библиотека");
        system.checkAccess(1, "Серверная");

        system.sortUsersByAccess();
        std::cout << "\nПосле сортировки:\n";
        system.displayAllUsers();

        system.saveToFile("users.txt");
        std::cout << "\nДанные сохранены в файл.\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}
