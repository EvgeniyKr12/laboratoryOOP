#include <iostream>
#include <string>


class Character {
private:
    std::string name;  // имя персонажа
    int health;        // уровень здоровья
    int attack;        // уровень атаки
    int defense;       // уровень защиты

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    int getHealth() const {
        return health;
    }
    // Метод для лечения (не выше maxHealth)
    void heal(int amount) {
        health += amount;
        if (health > 100) {
            health = 100;
        }
        std::cout << name << "cured, now his health " << health << std::endl;
    }

    //  Метод для получения урона (не ниже 0)
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
        std::cout << name << "took damage, now his health " << health << std::endl;
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
    
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        } else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }
};


//int main() {
//    Character hero("Hero", 100, 20, 10);
//    Character monster("Goblin", 50, 15, 5);
//
//    hero.displayInfo();
//    monster.displayInfo();
//
//    hero.attackEnemy(monster);
//    monster.displayInfo();
//
//    hero.heal(50);
//    hero.takeDamage(30);
//    hero.displayInfo();
//    
//    return 0;
//}
