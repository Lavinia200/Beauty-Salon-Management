#ifndef SALON_H
#define SALON_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>


struct IntervalOrar {
    int oraStart, minutStart;
    int oraFinal, minutFinal;

    bool seSuprapuneCu(const IntervalOrar& altul) const;
};

enum class TipServiciu{TUNS, VOPSIT, COSMETICA, TRATAMENT, MANICHIURA};

class Serviciu {
    std::string nume;
    double pret;
    int durataMinute;
    TipServiciu tip;
public:
    explicit Serviciu(std::string nume_serv = "Tuns", double pret_serv = 0.0, int durata_serv = 30, TipServiciu t_serv = TipServiciu::TUNS);


    [[nodiscard]] int getDurata() const {return durataMinute;}
    [[nodiscard]] TipServiciu getTip() const {return tip;}
    [[nodiscard]] double getPret() const;
    [[nodiscard]] const std::string& getNume() const {return nume; }
    [[nodiscard]] bool esteComplex() const {return durataMinute > 60;}

    friend std::ostream& operator<<(std::ostream& os, const Serviciu& s);

};

class Angajat {
    std::string nume;
    std::string specializare;
    std::vector <TipServiciu> competente;
public:
    explicit Angajat(std::string nume_ang = "Anonim", std::string spec = "General");

    void adaugaCompetenta(TipServiciu tip) {competente.push_back(tip);} // metoda pt adaugare serviciu prestat
    bool poatePresta(TipServiciu tipCautat) const {
        for (auto tip : competente) {
            if (tip == tipCautat) return true;
        }
        return false;
    }

    //getter pt nume
    [[nodiscard]] std::string& getNume() const {return nume;}
    //getter pt specializare
    [[nodiscard]] const std::string& getSpecializare() const { return specializare;}

    friend std::ostream& operator<<(std::ostream& os, const Angajat& a);
};

class Programare {
    std::string numeClient;
    Angajat stilist;
    std::vector<Serviciu> servicii;
    std::string* observatii;
    IntervalOrar interval;
    int zi, luna, an;
public:
    Programare( const std::string& client,const Angajat& a, int z, int l , int an_pr, IntervalOrar inter, const std::string& obs = "");
    ~Programare();
    Programare(const Programare& other);
    Programare& operator=(const Programare& other);

    void setStilist(const Angajat& s) {stilist = s;}
    void setData (int d, int l, int a) {zi =d; luna= l; an = a ;}
    void setInterval(const IntervalOrar& inter) {interval = inter;}

    void setObservatii(const std::string& obs);

    [[nodiscard]] IntervalOrar getInterval() const {return interval; }
    [[nodiscard]] int getZi() const {return zi;}
    [[nodiscard]] int getLuna() const {return luna;}
    [[nodiscard]] int getAn() const {return an;}
    [[nodiscard]] const Angajat& getStilist() const { return stilist; }
    [[nodiscard]] const std::string& getNumeClient() const {return numeClient; }

    void adaugaServiciu(const Serviciu& s);
    [[nodiscard]] double calculeazaTotal() const;
    [[nodiscard]] bool estePremium() const;
    [[nodiscard]] double aplicaDiscount() const;
    [[nodiscard]] int calculeazaDurataTotala() const;


    friend std::ostream& operator<<(std::ostream& os, const Programare& p);
};

class Salon {
    std::string numeSalon;
    std:: vector<Programare> listaProgramari;
    std::vector <Angajat> angajatiSalon;
public:
    explicit Salon(std:: string nume = "Beauty Salon");

    //metoda pt a popula echipa de angajati
    void adaugaAngajat(const Angajat& a) {angajatiSalon.push_back(a); }

    //folosim losta interna
    void afiseazaDisponibilitateAngajat( const std::string& numeAngajat, int z, int l, int an, int durataNecesara) const;
    void adaugaProgramare(const Programare& p);

    [[nodiscard]] bool esteAngajatDisponibil(const Angajat& ang, int z, int l , int a, IntervalOrar inter) const;

    [[nodiscard]] double calculeazaIncasariTotale() const;
    void afiseazaRaportZilnic() const;

    [[nodiscard]] const std::vector<Angajat>& getAngajati() const {return angajatiSalon; }
    friend std::ostream& operator<<(std::ostream& os, const Salon& s);

};
#endif
