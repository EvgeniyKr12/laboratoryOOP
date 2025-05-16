#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <fstream>


class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d) {}

    virtual ~Entity() = default;

    void takeDamage(int damage) {
        int actualDamage = std::max(1, damage - defense);
        health -= actualDamage;
        std::cout << name << " получил " << actualDamage << " урона!\n";
    }

    bool isAlive() const { return health > 0; }

    void showStats() const {
        std::cout << name << " [HP: " << health << "/" << maxHealth
                  << " ATK: " << attack << " DEF: " << defense << "]\n";
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};


class Player : public Entity {
private:
    int level;
    int experience;
    int experienceToNextLevel;

public:
    Player(const std::string& name)
        : Entity(name, 100, 10, 5), level(1), experience(0), experienceToNextLevel(100) {}

    void gainExperience(int exp) {
        experience += exp;
        std::cout << "Получено " << exp << " опыта!\n";
        
        if (experience >= experienceToNextLevel) {
            levelUp();
        }
    }

    void levelUp() {
        level++;
        experience -= experienceToNextLevel;
        experienceToNextLevel = static_cast<int>(experienceToNextLevel * 1.5);
        
        maxHealth += 20;
        health = maxHealth;
        attack += 3;
        defense += 2;
        
        std::cout << "Уровень повышен до " << level << "!\n";
    }

    void heal() {
        int healAmount = maxHealth / 2;
        health = std::min(maxHealth, health + healAmount);
        std::cout << name << " восстановил " << healAmount << " HP!\n";
    }

    void showFullStats() const {
        showStats();
        std::cout << "Уровень: " << level << "\n";
        std::cout << "Опыт: " << experience << "/" << experienceToNextLevel << "\n";
    }

    int getLevel() const { return level; }
};


class Goblin : public Entity {
public:
    Goblin() : Entity("Гоблин", 50, 12, 3) {}
};

class Skeleton : public Entity {
public:
    Skeleton() : Entity("Скелет", 30, 8, 2) {}
};


class Inventory {
private:
    std::vector<std::string> items;
    int gold;

public:
    Inventory() : gold(0) {}

    void addItem(const std::string& item) {
        items.push_back(item);
        std::cout << "Добавлен предмет: " << item << "\n";
    }

    void addGold(int amount) {
        gold += amount;
        std::cout << "Получено " << amount << " золота!\n";
    }

    bool useItem(const std::string& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }

    void showInventory() const {
        std::cout << "=== Инвентарь ===\n";
        std::cout << "Золото: " << gold << "\n";
        std::cout << "Предметы:\n";
        for (const auto& item : items) {
            std::cout << "- " << item << "\n";
        }
    }
};


class Game {
private:
    Player player;
    Inventory inventory;
    bool gameRunning;

    std::unique_ptr<Entity> createRandomEnemy() {
        int random = rand() % 100;
        if (random < 70) return std::make_unique<Goblin>();
        return std::make_unique<Skeleton>();
    }

    void battle(Entity& enemy) {
        std::cout << "Бой с " << enemy.getName() << " начался!\n";
        
        while (player.isAlive() && enemy.isAlive()) {
            std::cout << "\nВаш ход:\n";
            player.showFullStats();
            enemy.showStats();
            
            std::cout << "1. Атаковать\n2. Использовать предмет\n3. Попытаться сбежать\n";
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    enemy.takeDamage(player.getAttack());
                    break;
                case 2:
                    if (inventory.useItem("Зелье здоровья")) {
                        player.heal();
                    } else {
                        std::cout << "У вас нет зелий здоровья!\n";
                    }
                    break;
                case 3:
                    if (rand() % 3 == 0) {
                        std::cout << "Вы успешно сбежали!\n";
                        return;
                    }
                    std::cout << "Побег не удался!\n";
                    break;
                default:
                    std::cout << "Неверный выбор!\n";
                    continue;
            }
            
            if (enemy.isAlive()) {
                player.takeDamage(enemy.getAttack());
            }
        }
        
        if (player.isAlive()) {
            int goldEarned = 10 + rand() % 20;
            int expEarned = 25 + rand() % 15;
            
            inventory.addGold(goldEarned);
            player.gainExperience(expEarned);
            
            if (rand() % 3 == 0) {
                inventory.addItem("Зелье здоровья");
            }
        }
    }

    void saveGame() {
        std::ofstream saveFile("save.txt");
        if (saveFile) {
            saveFile << player.getName() << "\n";
            saveFile << player.getLevel() << " " << player.getHealth() << " "
                     << player.getAttack() << " " << player.getDefense() << "\n";
            

            saveFile << "Зелье здоровья\n";
            
            std::cout << "Игра сохранена!\n";
        } else {
            std::cerr << "Ошибка сохранения!\n";
        }
    }

    void loadGame() {
        std::ifstream saveFile("save.txt");
        if (saveFile) {
            std::string name;
            int level, health, attack, defense;
            
            std::getline(saveFile, name);
            saveFile >> level >> health >> attack >> defense;
            
            player = Player(name);

            
            std::cout << "Игра загружена!\n";
            player.showFullStats();
        } else {
            std::cerr << "Файл сохранения не найден!\n";
        }
    }

public:
    Game() : player("Герой"), gameRunning(true) {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    void run() {
        std::cout << "Добро пожаловать в текстовую RPG!\n";
        
        std::cout << "Введите имя персонажа: ";
        std::string playerName;
        std::getline(std::cin, playerName);
        player = Player(playerName);
        inventory.addItem("Зелье здоровья");
        
        while (gameRunning && player.isAlive()) {
            std::cout << "\nВы находитесь в темном лесу...\n";
            std::cout << "1. Исследовать\n2. Открыть инвентарь\n3. Сохранить игру\n4. Загрузить игру\n5. Выйти\n";
            
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1: {
                    auto enemy = createRandomEnemy();
                    battle(*enemy);
                    break;
                }
                case 2:
                    inventory.showInventory();
                    break;
                case 3:
                    saveGame();
                    break;
                case 4:
                    loadGame();
                    break;
                case 5:
                    gameRunning = false;
                    break;
                default:
                    std::cout << "Неверный выбор!\n";
            }
        }
        
        if (!player.isAlive()) {
            std::cout << "Игра окончена! Ваш персонаж погиб.\n";
        }
    }
};

//int main() {
//    Game game;
//    game.run();
//    return 0;
//}
