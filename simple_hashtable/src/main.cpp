#include <iostream>
#include "Hashmap.hpp"


int main()
{
    Hashmap<std::string, std::string> idols;
    idols.insert("Arisu", "Tachibana"); 
    idols.insert("Momoka", "Sakurai");
    idols.insert("Rika", "Jougasaki");
    idols.insert("An", "Futaba");
    idols.insert("Mami", "Futami");
    idols.insert("Yayoi", "Takatsuki");
    idols.insert("Momoko", "Suou");

    std::cout << "\nLet's Check!\n\n";

    auto name = idols.find("Momoko");
    if (name != nullptr) {
        std::cout << name->key << " " << name->value << "\n\n";
    }

    name = idols.find("Arisu");
    if (name != nullptr) {
        std::cout << name->key << " " << name->value << "\n\n";
    }

    name = idols.find("An");
    if (name != nullptr) {
        std::cout << name->key << " " << name->value << "\n\n";
    }

    // Chie is not in the hashtable, so this should not print anything (other than the index)
    name = idols.find("Chie");
    if (name != nullptr) {
        std::cout << name->key << " " << name->value << "\n\n";
    }

    name = idols.find("Yayoi");
    if (name != nullptr) {
        std::cout << name->key << " " << name->value << "\n\n";
    }

}
