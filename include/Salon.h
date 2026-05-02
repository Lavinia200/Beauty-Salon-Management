#ifndef SALON_H
#define SALON_H

#include <iostream>
#include <string>
#include <vector>

class Serviciu {
    std::string nume;
    double pret;
public:
    explicit Serviciu(std::string nume_serv = "Tuns", double pret_serv = 0.0);
    [[nodiscard]] double getPret() const;
    [[nodiscard]] const std::string& getNume() const {return nume; }

    friend std::ostream& operator<<(std::ostream& os, const Serviciu& s);

};

class Angajat {
    std::string nume;
    std::string specializare;
public:
    explicit Angajat(std::string nume_ang = "Anonim", std::string spec = "General");

    friend std::ostream& operator<<(std::ostream& os, const Angajat& a);
};

class Programare {
    std::string numeClient;
    Angajat stilist;
    std::vector<Serviciu> servicii;
    std::string* observatii;
public:
    Programare( const std::string& client,const Angajat& a, const std::string& obs = "");

    ~Programare();
    Programare(const Programare& other);
    Programare& operator=(const Programare& other);

    void adaugaServiciu(const Serviciu& s);
    [[nodiscard]] double calculeazaTotal() const;
    [[nodiscard]] bool estePremium() const;

    friend std::ostream& operator<<(std::ostream& os, const Programare& p);
};

class Salon {
    std::string numeSalon;
    std:: vector<Programare> listaProgramari;
public:
    explicit Salon(std:: string nume = "Beauty Salon");

    void adaugaProgramare(const Programare& p);
    void afiseazaRaportZilnic() const;
    [[nodiscard]] double calculeazaIncasariTotale() const;
    friend std::ostream& operator<<(std::ostream& os, const Salon& s);

};
#endif
