#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;

class Inventory {
private:
    vector<unique_ptr<string>> items;

public:
    void addItem(const string& item) {
        items.push_back(make_unique<string>(item));
    }

    void displayInventory() const {
        if (items.empty()) {
            cout << "Инвентарь пуст!" << endl;
            return;
        }
        cout << "=== Инвентарь ===" << endl;
        for (const auto& item : items) {
            cout << "- " << *item << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    Inventory inv;
    inv.addItem("Меч");
    inv.addItem("Щит");
    inv.addItem("Зелье здоровья");
    inv.displayInventory();

    return 0;
}