#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
    string email;
    string address;

public:
    // Геттеры
    string getName() const { return name; }
    int getAge() const { return age; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }

    // Сеттеры
    void setName(const string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            cerr << "Ошибка: Имя не может быть пустым!\n";
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            cerr << "Ошибка: Возраст должен быть от 0 до 120 лет!\n";
        }
    }

    void setEmail(const string& newEmail) {
        if (newEmail.find('@') != string::npos) {
            email = newEmail;
        }
        else {
            cerr << "Ошибка: Email должен содержать символ @!\n";
        }
    }

    void setAddress(const string& newAddress) {
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            cerr << "Ошибка: Адрес не может быть пустым!\n";
        }
    }

    void displayInfo() const {
        cout << "Имя: " << name << "\nВозраст: " << age
            << "\nEmail: " << email << "\nАдрес: " << address << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    Person person;
    person.setName("Иван Иванов");
    person.setAge(30);
    person.setEmail("ivan@example.com");
    person.setAddress("ул. Пушкина, д. 10");

    cout << "=== Информация о человеке ===\n";
    person.displayInfo();

    // Тестирование ошибок
    cout << "\n=== Тестирование ошибок ===\n";
    person.setName("");
    person.setAge(150);
    person.setEmail("invalid-email");
    person.setAddress("");

    //Установка новых валидных значений
    cout << "\n=== Валидные значения" << endl;
    person.setName("Ваня Иванов");
    person.setAge(40);
    person.setEmail("ivanov@example.com");
    person.setAddress("ул. Колотушкина, д. 55");
    person.displayInfo();

    return 0;
}