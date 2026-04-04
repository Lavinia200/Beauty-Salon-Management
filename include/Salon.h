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
    [[nodiscard]] std::string getNume() const {return nume; }
    friend std::ostream& operator<(std::ostream& os, const Serviciu& s);

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
    int* idProgramare;
public:
    Programare(std::string client, Angajat a, int id);
    ~Programare();
    Programare(const Programare& other);
    Programare& operator=(const Programare& other);

    void adaugaServiciu(const Serviciu& s);
    [[nodiscard]] double calculeazaTotal() const;
    [[nodiscard]] bool estePremium() const;
    friend std::ostream& operator<<(std::ostream& os, const Programare& p);
};
#endif // EXAMPLE_H
