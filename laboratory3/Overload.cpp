#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
           << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {}

    Weapon operator+(const Weapon& other) const {
        std::string newName = name + "+" + other.name;
        int newDamage = damage + other.damage;
        float newWeight = weight + other.weight;
        return Weapon(newName, newDamage, newWeight);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    void displayInfo() const {
        std::cout << "Weapon: " << name << ", Damage: " <<
        damage << ", Weight: " << weight << " kg\n";
    }
};

//int main() {
//    Character hero1("Hero", 100, 20, 10);
//    Character hero2("Hero", 100, 20, 10);
//    Character hero3("Warrior", 150, 25, 15);
//
//    if (hero1 == hero2) {
//        std::cout << "Hero1 and Hero2 are the same!\n";
//    }
//    if (!(hero1 == hero3)) {
//        std::cout << "Hero1 and Hero3 are different!\n";
//    }
//
//    std::cout << hero1 << std::endl;
//    
//    
//    Weapon sword("Sword", 30, 3.5f);
//    Weapon bow("Bow", 20, 1.2f);
//    
//    Weapon combined = sword + bow;
//    combined.displayInfo();
//    
//    if (sword > bow) {
//            std::cout << "\nSword is stronger than Bow\n";
//        } else {
//            std::cout << "\nBow is stronger than Sword\n";
//        }
//    
//    return 0;
//}

