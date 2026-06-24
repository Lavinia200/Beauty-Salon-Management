

#ifndef OOP_ANGAJATFACTORY_H
#define OOP_ANGAJATFACTORY_H

#include <memory>
#include <string>
#include "angajat.h"

class AngajatFactory {
public:
    static std::shared_ptr<Angajat> creeazaAngajat(const std::string& rang, const std::string& nume, const std::string& specializare) {
        if (rang == "Junior" || rang == "junior") {
            return std::make_shared<StilistJunior>(nume, specializare, 30.0);
        } else if (rang == "Senior" || rang == "senior") {
            return std::make_shared<StilistSenior>(nume, specializare, 0.15, 0);
        }
        return nullptr;
    }
};
#endif //OOP_ANGAJATFACTORY_H