
#ifndef OOP_PROGRAMARE_H
#define OOP_PROGRAMARE_H
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "servicii.h"
#include "angajat.h"

struct IntervalOrar {
    int oraStart, minutStart;
    int oraFinal, minutFinal;

    bool seSuprapuneCu(const IntervalOrar& altul) const;
};

class Programare {
    std::string numeClient;
    std::shared_ptr<Angajat> stilist;
    std::vector<Serviciu> servicii;
    std::string* observatii;
    IntervalOrar interval;
    int zi, luna, an;
public:
    Programare( const std::string& client,const Angajat& a, int z, int l , int an_pr, IntervalOrar inter, const std::string& obs = "");
    ~Programare();
    Programare(const Programare& other);
    Programare& operator=(const Programare& other);

    void setStilist(std::shared_ptr<Angajat> s) {stilist = s;}
    void setData (int d, int l, int a) {zi =d; luna= l; an = a ;}
    void setInterval(const IntervalOrar& inter) {interval = inter;}
    void setObservatii(const std::string& obs);

    [[nodiscard]] IntervalOrar getInterval() const {return interval; }
    [[nodiscard]] int getZi() const {return zi;}
    [[nodiscard]] int getLuna() const {return luna;}
    [[nodiscard]] int getAn() const {return an;}
    [[nodiscard]] const Angajat& getStilist() const { return *stilist; }
    [[nodiscard]] const std::string& getNumeClient() const {return numeClient; }

    void adaugaServiciu(const Serviciu& s);
    [[nodiscard]] double calculeazaTotal() const;
    [[nodiscard]] bool estePremium() const;
    [[nodiscard]] double aplicaDiscount() const;
    [[nodiscard]] int calculeazaDurataTotala() const;

    friend std::ostream& operator<<(std::ostream& os, const Programare& p);
};

#endif