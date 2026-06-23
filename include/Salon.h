#ifndef SALON_H
#define SALON_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include "servicii.h"
#include "exceptii.h"
#include "angajat.h"
#include "programare.h"

class Salon {
    std::string numeSalon;
    std:: vector<Programare> listaProgramari;
    std::vector<std::shared_ptr<Angajat>> angajatiSalon;

    std::vector<Serviciu> catalogServicii;
public:
    explicit Salon(std:: string nume = "Beauty Salon");

    ~Salon() = default;
    Salon(const Salon& other) = default;
    Salon& operator=(const Salon& other) = default;

    //metoda pt a popula echipa de angajati
    void adaugaAngajat(std::shared_ptr<Angajat> a);
    void adaugaServiciuInCatalog(const Serviciu& s) { catalogServicii.push_back(s); }
    [[nodiscard]] const std::vector<Serviciu>& getCatalog() const { return catalogServicii; }
    [[nodiscard]] double calculeazaIncasariAngajat(const std::string& numeAngajat) const;

    void afiseazaProgramariDupaZi(int z, int l, int a) const;
    void afiseazaProgramariDupaLuna(int l, int a) const;
    void vizualizeazaSalarii() const;
    bool upgradeAngajatLaSenior(const std::string& numeCautat);
    void schimbaManagerul (const std::string& numeManagerNou, double salariuFixNou);

    void afiseazaDisponibilitateAngajat( const std::string& numeAngajat, int z, int l, int an, int durataNecesara) const;
    void adaugaProgramare(const Programare& p);
    [[nodiscard]] bool esteAngajatDisponibil(const Angajat& ang, int z, int l , int a, IntervalOrar inter) const;
    [[nodiscard]] double calculeazaIncasariTotale() const;
    void afiseazaRaportZilnic() const;

    [[nodiscard]] const std::vector<std::shared_ptr<Angajat>>& getAngajati() const { return angajatiSalon; }
    friend std::ostream& operator<<(std::ostream& os, const Salon& s);

};
#endif
