#ifndef SERVICIU_H
#define SERVICIU_H

#include <string>
#include <iostream>



class Serviciu {
    std::string nume;
    double pret;
    int durataMinute;
    std::string tip;
public:
    explicit Serviciu(std::string nume_serv = "Tuns", double pret_serv = 0.0, int durata_serv = 30, std::string t_serv = "Tuns");

    [[nodiscard]] int getDurata() const {return durataMinute;}
    [[nodiscard]] const std::string& getTip() const {return tip;}
    [[nodiscard]] double getPret() const;
    [[nodiscard]] const std::string& getNume() const {return nume; }
    [[nodiscard]] bool esteComplex() const {return durataMinute > 60;}

    friend std::ostream& operator<<(std::ostream& os, const Serviciu& s);
};
#endif