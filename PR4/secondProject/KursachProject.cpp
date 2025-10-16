#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <limits>

using namespace std;

// Структура для валют
struct Currency {
    string code;
    double exchangeRateToBase;  // Курс валюты относительно базовой (например, USD или EUR)
    Currency(string code, double exchangeRateToBase)
        : code(code), exchangeRateToBase(exchangeRateToBase) {}
};

// Класс Category для категорий доходов и расходов
class Category {
public:
    string name;
    string type; // "Income" или "Expense"

    Category(string name, string type) : name(name), type(type) {}

    void display() const {
        cout << name;
    }

    string getType() const {
        return type;
    }
};

// Абстрактный класс Transaction для общих операций с транзакциями
class Transaction {
protected:
    double amount;
    time_t date;
    Category category;
    Currency currency;

public:
    // Конструктор для инициализации всех полей
    Transaction(double amount, time_t date, Category category, Currency currency)
        : amount(amount), date(date), category(category), currency(currency) {}

    virtual void display() const = 0;

    double getAmountInBaseCurrency() const {
        return amount * currency.exchangeRateToBase;
    }

    double getAmount() const {
        return amount;
    }

    const Category& getCategory() const {
        return category;
    }

    time_t getDate() const {
        return date;
    }

    string getFormattedDate() const {
        struct tm timeinfo;
        localtime_s(&timeinfo, &date); // Используем безопасную версию
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
        return string(buffer);
    }
};

// Класс Income (Доход)
class Income : public Transaction {
private:
    string source;

public:
    Income(double amount, time_t date, Category category, Currency currency, string source)
        : Transaction(amount, date, category, currency), source(source) {}

    void display() const override {
        cout << "Доход: " << amount << " " << currency.code << " от " << source << " ";
        cout << "Дата: " << getFormattedDate() << " ";
        cout << "Категория: " << category.name << endl;
    }
};

// Класс Expense (Расход)
class Expense : public Transaction {
private:
    string description;

public:
    Expense(double amount, time_t date, Category category, Currency currency, string description)
        : Transaction(amount, date, category, currency), description(description) {}

    void display() const override {
        cout << "Расход: " << amount << " " << currency.code << " на " << description << " ";
        cout << "Дата: " << getFormattedDate() << " ";
        cout << "Категория: " << category.name << endl;
    }
};

// Класс FinanceManager для управления транзакциями
class FinanceManager {
private:
    vector<shared_ptr<Transaction>> transactions;  // Используем умные указатели для управления памятью
    string username;
    Currency baseCurrency;
    map<string, double> categoryLimits; // Лимиты по категориям расходов
    map<string, double> categoryExpenses; // Потраченные средства по категориям

public:
    // Конструктор с параметрами для указания имени пользователя и валюты
    FinanceManager(string username, Currency baseCurrency)
        : username(username), baseCurrency(baseCurrency) {}

    // Конструктор по умолчанию
    FinanceManager() : username(""), baseCurrency(Currency("USD", 1.0)) {}  // Значение по умолчанию для валюты

    // Метод для добавления дохода
    void addIncome(double amount, time_t date, Category category, Currency currency, string source) {
        shared_ptr<Income> income = make_shared<Income>(amount, date, category, currency, source);
        transactions.push_back(income);
    }

    // Метод для добавления расхода
    void addExpense(double amount, time_t date, Category category, Currency currency, string description) {
        shared_ptr<Expense> expense = make_shared<Expense>(amount, date, category, currency, description);
        transactions.push_back(expense);

        // Обновляем потраченные средства по категориям
        if (category.getType() == "Expense") {
            categoryExpenses[category.name] += amount;
        }
    }

    // Метод для подсчета общего дохода
    double totalIncome() const {
        double total = 0;
        for (const auto& transaction : transactions) {
            if (transaction->getCategory().getType() == "Income") {
                total += transaction->getAmountInBaseCurrency();
            }
        }
        return total;
    }

    // Метод для подсчета общих расходов
    double totalExpenses() const {
        double total = 0;
        for (const auto& transaction : transactions) {
            if (transaction->getCategory().getType() == "Expense") {
                total += transaction->getAmountInBaseCurrency();
            }
        }
        return total;
    }

    // Метод для подсчета баланса (доходы - расходы)
    double balance() const {
        return totalIncome() - totalExpenses();
    }

    // Метод для отображения всех транзакций
    void displayTransactions() const {
        cout << "\nВсе транзакции для пользователя " << username << ":\n";
        for (const auto& transaction : transactions) {
            transaction->display();
        }
    }

    // Метод для фильтрации транзакций по категории
    void displayTransactionsByCategory(const string& categoryName) const {
        cout << "\nТранзакции по категории " << categoryName << ":\n";
        for (const auto& transaction : transactions) {
            if (transaction->getCategory().name == categoryName) {
                transaction->display();
            }
        }
    }

    // Метод для фильтрации транзакций по дате (начало и конец периода)
    void displayTransactionsByDateRange(time_t startDate, time_t endDate) const {
        cout << "\nТранзакции за выбранный период:\n";
        for (const auto& transaction : transactions) {
            if (transaction->getDate() >= startDate && transaction->getDate() <= endDate) {
                transaction->display();
            }
        }
    }

    // Установка лимита на расходы по категории
    void setCategoryLimit(const string& categoryName, double limit) {
        categoryLimits[categoryName] = limit;
        cout << "Лимит на категорию " << categoryName << " установлен: " << limit << " " << baseCurrency.code << endl;
    }

    // Отслеживание выполнения бюджета
    void trackBudget() const {
    cout << "\nОтслеживание выполнения бюджета:\n";
    for (auto it = categoryLimits.begin(); it != categoryLimits.end(); ++it) {
        const string& category = it->first;
        double limit = it->second;

        double spent = categoryExpenses.count(category) ? categoryExpenses.at(category) : 0.0;
        cout << "Категория: " << category << " Потрачено: " << spent << " Лимит: " << limit;
        if (spent > limit) {
            cout << " (Превышение лимита!)";
        }
        cout << endl;
    }
}


    string getUsername() const {
        return username;
    }

    Currency getBaseCurrency() const {
        return baseCurrency;
    }
};

// Класс UserManager для работы с несколькими пользователями
class UserManager {
private:
    map<string, FinanceManager> users; // Ключ: имя пользователя, значение: объект менеджера финансов

public:
    void addUser(string username, Currency baseCurrency) {
        if (users.find(username) != users.end()) {
            throw runtime_error("Пользователь с таким именем уже существует.");
        }
        users[username] = FinanceManager(username, baseCurrency);  // Создаем пользователя с указанным именем и валютой
    }

    FinanceManager& getUser(string username) {
        if (users.find(username) == users.end()) {
            throw runtime_error("Пользователь не найден.");
        }
        return users[username];
    }

    void deleteUser(string username) {
        auto it = users.find(username);
        if (it != users.end()) {
            users.erase(it);
        }
        else {
            throw runtime_error("Пользователь не найден.");
        }
    }

    void displayUsers() const {
        cout << "\nСписок пользователей:\n";
        for (const auto& user : users) {
            cout << user.first << endl;
        }
    }

    vector<string> getAllUsers() const {
        vector<string> usernames;
        for (const auto& user : users) {
            usernames.push_back(user.first);
        }
        return usernames;
    }
};

// Функция для добавления новой валюты
Currency createCurrency() {
    string code;
    double exchangeRate;
    cout << "Введите код валюты (например, USD): ";
    cin >> code;
    while (true) {
        cout << "Введите курс валюты относительно базовой (например, 1.0 для USD): ";
        cin >> exchangeRate;
        if (cin.fail() || exchangeRate <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректное значение. Попробуйте снова." << endl;
        }
        else {
            break;
        }
    }
    return Currency(code, exchangeRate);
}

// Главное меню программы
void showMainMenu() {
    cout << "1. Создать пользователя\n";
    cout << "2. Удалить пользователя\n";
    cout << "3. Добавить транзакцию\n";
    cout << "4. Просмотреть транзакции\n";
    cout << "5. Подсчитать баланс\n";
    cout << "6. Установить лимит на расходы по категории\n";
    cout << "7. Отслеживать выполнение бюджета\n";
    cout << "8. Выход\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    UserManager userManager;
    Currency baseCurrency("USD", 1.0);

    while (true) {
        showMainMenu();

        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод, попробуйте снова.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            string username;
            cout << "Введите имя пользователя: ";
            cin >> username;
            try {
                userManager.addUser(username, baseCurrency);
                cout << "Пользователь " << username << " успешно добавлен." << endl;
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 2: {
            string username;
            cout << "Введите имя пользователя для удаления: ";
            cin >> username;
            try {
                userManager.deleteUser(username);
                cout << "Пользователь " << username << " успешно удален." << endl;
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 3: {
            vector<string> users = userManager.getAllUsers();
            if (users.empty()) {
                cout << "Нет пользователей!" << endl;
                break;
            }
            cout << "Выберите пользователя:\n";
            for (int i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i] << endl;
            }

            int userChoice;
            cin >> userChoice;
            if (userChoice < 1 || userChoice > users.size()) {
                cout << "Неверный выбор!" << endl;
                break;
            }
            string selectedUser = users[userChoice - 1];
            FinanceManager& manager = userManager.getUser(selectedUser);

            Currency currency = createCurrency();
            double amount;
            int categoryChoice;
            string descriptionOrSource;
            time_t now = time(0);

            cout << "Введите сумму транзакции: ";
            cin >> amount;
            if (cin.fail() || amount <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Некорректная сумма!" << endl;
                break;
            }

            cout << "Выберите тип транзакции:\n1. Доход\n2. Расход\n";
            cin >> categoryChoice;

            cout << "Введите название категории: ";
            cin.ignore();
            string categoryName;
            getline(cin, categoryName);

            string categoryType = (categoryChoice == 1) ? "Income" : "Expense";
            Category category(categoryName, categoryType);

            if (categoryChoice == 1) {
                cout << "Введите источник дохода: ";
                getline(cin, descriptionOrSource);
                manager.addIncome(amount, now, category, currency, descriptionOrSource);
            }
            else if (categoryChoice == 2) {
                cout << "Введите описание расхода: ";
                getline(cin, descriptionOrSource);
                manager.addExpense(amount, now, category, currency, descriptionOrSource);
            }
            else {
                cout << "Некорректный выбор!" << endl;
            }
            break;
        }
        case 4: {
            vector<string> users = userManager.getAllUsers();
            if (users.empty()) {
                cout << "Нет пользователей!" << endl;
                break;
            }
            cout << "Выберите пользователя:\n";
            for (int i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i] << endl;
            }

            int userChoice;
            cin >> userChoice;
            if (userChoice < 1 || userChoice > users.size()) {
                cout << "Неверный выбор!" << endl;
                break;
            }
            string selectedUser = users[userChoice - 1];
            FinanceManager& manager = userManager.getUser(selectedUser);
            manager.displayTransactions();
            break;
        }
        case 5: {
            vector<string> users = userManager.getAllUsers();
            if (users.empty()) {
                cout << "Нет пользователей!" << endl;
                break;
            }
            cout << "Выберите пользователя:\n";
            for (int i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i] << endl;
            }

            int userChoice;
            cin >> userChoice;
            if (userChoice < 1 || userChoice > users.size()) {
                cout << "Неверный выбор!" << endl;
                break;
            }
            string selectedUser = users[userChoice - 1];
            FinanceManager& manager = userManager.getUser(selectedUser);

            cout << "Баланс пользователя " << selectedUser << ": " << manager.balance() << " " << manager.getBaseCurrency().code << endl;
            break;
        }
        case 6: {
            vector<string> users = userManager.getAllUsers();
            if (users.empty()) {
                cout << "Нет пользователей!" << endl;
                break;
            }
            cout << "Выберите пользователя:\n";
            for (int i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i] << endl;
            }

            int userChoice;
            cin >> userChoice;
            if (userChoice < 1 || userChoice > users.size()) {
                cout << "Неверный выбор!" << endl;
                break;
            }
            string selectedUser = users[userChoice - 1];
            FinanceManager& manager = userManager.getUser(selectedUser);

            cout << "Введите название категории: ";
            cin.ignore();
            string categoryName;
            getline(cin, categoryName);

            double limit;
            cout << "Введите лимит для категории: ";
            cin >> limit;

            if (cin.fail() || limit <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Некорректное значение лимита!" << endl;
                break;
            }

            manager.setCategoryLimit(categoryName, limit);
            break;
        }
        case 7: {
            vector<string> users = userManager.getAllUsers();
            if (users.empty()) {
                cout << "Нет пользователей!" << endl;
                break;
            }
            cout << "Выберите пользователя:\n";
            for (int i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i] << endl;
            }

            int userChoice;
            cin >> userChoice;
            if (userChoice < 1 || userChoice > users.size()) {
                cout << "Неверный выбор!" << endl;
                break;
            }
            string selectedUser = users[userChoice - 1];
            FinanceManager& manager = userManager.getUser(selectedUser);

            manager.trackBudget();
            break;
        }
        case 8:
            cout << "Выход из программы...\n";
            return 0;
        default:
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    }

    return 0;
}