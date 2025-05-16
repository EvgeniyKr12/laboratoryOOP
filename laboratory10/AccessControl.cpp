#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>


class User {
protected:
    std::string name;
    int id;
    int accessLevel;  // (1-студент, 2-преподаватель, 3-админ)

public:
    User(const std::string& n, int id, int aL) : name(n), id(id), accessLevel(aL) {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (accessLevel < 1 || accessLevel > 3) throw std::invalid_argument("Некорректный уровень доступа");
    }

    virtual ~User() {}

    virtual void displayInfo() const = 0;  // делает класс абстрактным

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }
};


class Student : public User {
    std::string group;

public:
    Student(const std::string& n, int i, int aL, const std::string& g)
        : User(n, i, aL), group(g) {
        if (group.empty()) throw std::invalid_argument("Группа не может быть пустой");
    }

    void displayInfo() const override {
        std::cout << "Студент: " << name << "\nID: " << id
                  << "\nГруппа: " << group << "\nУровень доступа: "
                  << accessLevel << "\n\n";
    }

    std::string getGroup() const { return group; }
};

class Teacher : public User {
    std::string department;

public:
    Teacher(const std::string& n, int i, int aL, const std::string& d)
        : User(n, i, aL), department(d) {
        if (department.empty()) throw std::invalid_argument("Кафедра не может быть пустой");
    }

    void displayInfo() const override {
        std::cout << "Преподаватель: " << name << "\nID: " << id
                  << "\nКафедра: " << department << "\nУровень доступа: "
                  << accessLevel << "\n\n";
    }

    std::string getDepartment() const { return department; }
};


class Administrator : public User {
public:
    Administrator(const std::string& n, int i, int aL) : User(n, i, aL) {}

    void displayInfo() const override {
        std::cout << "Администратор: " << name << "\nID: " << id
                  << "\nУровень доступа: " << accessLevel << "\n\n";
    }
};

class Resource {
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& n, int rAL) : name(n), requiredAccessLevel(rAL) {}

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    std::string getName() const { return name; }
};

// Система контроля доступа
class AccessControlSystem {
    std::vector<std::unique_ptr<User>> users;
    std::vector<Resource> resources;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(const Resource& resource) {
        resources.push_back(resource);
    }

    std::vector<User*> findUsers(const std::string& query) const {
        std::vector<User*> results;
        for (const auto& user : users) {
            if (user->getName() == query || std::to_string(user->getId()) == query) {
                results.push_back(user.get());
            }
        }
        return results;
    }

    void checkResourceAccess(const Resource& resource) const {
        std::cout << "Проверка доступа к ресурсу '" << resource.getName() << "':\n";
        for (const auto& user : users) {
            std::cout << user->getName() << " - "
                      << (resource.checkAccess(*user) ? "Доступ разрешен" : "Доступ запрещен")
                      << "\n";
        }
    }


    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка: не удалось открыть файл '" + filename + "' для записи");
        }

        std::cout << "Сохранение данных в файл: " << filename << std::endl;
        
        for (const auto& user : users) {
            file << user->getName() << ","
                 << user->getId() << ","
                 << user->getAccessLevel();

            if (auto student = dynamic_cast<const Student*>(user.get())) {
                file << "," << student->getGroup() << ",student";
            }
            else if (auto teacher = dynamic_cast<const Teacher*>(user.get())) {
                file << "," << teacher->getDepartment() << ",teacher";
            }
            else {
                file << ",admin";
            }
            file << "\n";  // Важно: добавляем перевод строки
        }

        file.flush();  // Принудительная запись
        std::cout << "Данные успешно сохранены!" << std::endl;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Ошибка открытия файла для чтения");

        users.clear();
        std::string line;
        
        while (getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            size_t pos3 = line.find(',', pos2 + 1);
            size_t pos4 = line.find(',', pos3 + 1);

            std::string name = line.substr(0, pos1);
            int id = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int accessLevel = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            std::string type = line.substr(line.find_last_of(',') + 1);

            if (type == "student") {
                std::string group = line.substr(pos3 + 1, pos4 - pos3 - 1);
                addUser(std::make_unique<Student>(name, id, accessLevel, group));
            }
            else if (type == "teacher") {
                std::string department = line.substr(pos3 + 1, pos4 - pos3 - 1);
                addUser(std::make_unique<Teacher>(name, id, accessLevel, department));
            }
            else {
                addUser(std::make_unique<Administrator>(name, id, accessLevel));
            }
        }
    }

    void sortUsersByAccessLevel() {
        sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
            return a->getAccessLevel() < b->getAccessLevel();
        });
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }
};

int main() {
    try {
        AccessControlSystem system;

        system.addUser(std::make_unique<Student>("Иван Иванов", 1, 1, "ИВБО-01-21"));
        system.addUser(std::make_unique<Teacher>("Петр Петров", 2, 2, "Информатика"));
        system.addUser(std::make_unique<Administrator>("Сидор Сидоров", 3, 3));
        
        Resource library("Библиотека", 1);
        Resource lab("Компьютерный класс", 2);
        Resource server("Сервер", 3);
        
        system.addResource(library);
        system.addResource(lab);
        system.addResource(server);


        system.checkResourceAccess(library);
        system.checkResourceAccess(server);

        system.saveToFile("users.csv");
        system.loadFromFile("users.csv");

        system.sortUsersByAccessLevel();
        system.displayAllUsers();

        std::string query;
        std::cout << "Введите имя или ID для поиска: ";
        std::cin >> query;
        
        auto foundUsers = system.findUsers(query);
        if (!foundUsers.empty()) {
            std::cout << "Найденные пользователи:\n";
            for (auto user : foundUsers) {
                user->displayInfo();
            }
        } else {
            std::cout << "Пользователи не найдены\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

//int main() {
//    try {
//        AccessControlSystem system;
//        system.addUser(std::make_unique<Student>("Иван Иванов", 1, 1, "ИВБО-01-21"));
//        system.addUser(std::make_unique<Teacher>("Петр Петров", 2, 2, "Информатика"));
//
//        // Проверяем текущую директорию
//        std::cout << "Текущая директория: " << std::filesystem::current_path() << std::endl;
//
//        // Сохраняем
//        system.saveToFile("users.csv");
//
//        // Проверяем, что записалось
//        std::ifstream testFile("users.csv");
//        if (testFile) {
//            std::cout << "Содержимое файла:\n";
//            std::string line;
//            while (std::getline(testFile, line)) {
//                std::cout << "> " << line << std::endl;
//            }
//        } else {
//            std::cerr << "❌ Файл не найден!" << std::endl;
//        }
//    } catch (const std::exception& e) {
//        std::cerr << "Ошибка: " << e.what() << std::endl;
//    }
//    return 0;
//}
