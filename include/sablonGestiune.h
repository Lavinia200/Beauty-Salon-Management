
#ifndef OOP_SABLONGESTIUNE_H
#define OOP_SABLONGESTIUNE_H

#include <vector>
#include <iostream>
#include <string>

template <typename T>
class GestiuneColectie {
private:
    std::vector<T> elemente;
public:
    void adauga(const T& elem) {
        elemente.push_back(elem);
    }
    const std::vector<T>& getElemente() const {
        return elemente;
    }
    size_t dimensiune() const {
        return elemente.size();
    }
};

template <typename T>
void afiseazaElementeGeneric(const std::string& titlu, const std::vector<T>& colectie) {
    std::cout << "\n=== " << titlu << " ===\n";
    for (size_t i = 0; i < colectie.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << colectie[i] << "\n";
    }
}
#endif
