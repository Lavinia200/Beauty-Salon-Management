#ifndef SALON_H
#define SALON_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <exception>
#include <memory>

class Salon;

class SalonException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit SalonException(std::string m) : mesaj(std::move(m)) {}
    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class DataInvalidaException : public SalonException {
public:
    DataInvalidaException(int z, int l, int a)
        : SalonException("Eroare de Validare : Data introdusa (" +
                         std::to_string(z) + "/" + std::to_string(l) + "/" + std::to_string(a) +
                         ") este invalida sau incorecta calendaristic !" ) {}
};

class SuprapunereIntervalException : public SalonException {
public:
    SuprapunereIntervalException (const std::string& numeAngajat, int h, int m)
        : SalonException("Eroare de Rezervare:  Stilistul " + numeAngajat +
            " este deja ocupat la ora " +  std::to_string(h) + ":" + (m==0 ? "00" : std::to_string(m)) + "!") {}
};


struct IntervalOrar {
    int oraStart, minutStart;
    int oraFinal, minutFinal;

    bool seSuprapuneCu(const IntervalOrar& altul) const;
};

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

class Angajat {
protected:
    std::string nume;
    std::string specializare;
    std::vector <std::string> competente;

    static int numarTotalAngajati;
public:
    explicit Angajat(std::string nume_ang = "Anonim", std::string spec = "General");
    virtual ~Angajat();

    [[nodiscard]] virtual Angajat* clone() const = 0;

    [[nodiscard]] virtual double calculeazaSalariu(const Salon& salon) const  = 0;
    virtual void afiseazaFluturasSalariu(const Salon& salon) const = 0;
    virtual void editeazaProfilSpecifice() = 0;

    [[nodiscard]] virtual bool esteManager() const {return false;}

    [[nodiscard]] virtual std::string getGrad() const = 0;

    void adaugaCompetenta(const std::string& categorie) {competente.push_back(categorie);} // metoda pt adaugare serviciu prestat
    [[nodiscard]] const std::vector<std::string>& getCompetente() const { return competente; }

    bool poatePresta( const std::string& categorieCautata) const {
        for (const auto& cat : competente) {
            if (cat == categorieCautata) return true;
        }
        return false;
    }

    //getter pt nume
    [[nodiscard]]  const std::string& getNume() const {return nume;}
    //getter pt specializare
    [[nodiscard]] const std::string& getSpecializare() const { return specializare;}

    void setSpecializare(const std::string& nouaSpec) {specializare = nouaSpec; }

    static int getNumarTotalAngajati() {return numarTotalAngajati; }

    friend std::ostream& operator<<(std::ostream& os, const Angajat& a);
};

class StilistJunior : public Angajat {
    double tarifOrarBaza;
public:
    StilistJunior(const std::string& nume_ang,const std::string& spec, double tarif);
    [[nodiscard]] Angajat* clone() const override {return new StilistJunior(*this); }
    [[nodiscard]] double calculeazaSalariu(const Salon& salon) const override;
    void afiseazaFluturasSalariu(const Salon& salon) const override;
    void editeazaProfilSpecifice() override;

    [[nodiscard]] std::string getGrad() const override { return "Junior"; }
};

class StilistSenior : public Angajat {
    double procentComision;
    int numarUcenici;
public:
    StilistSenior(const std::string& nume_ang,const std::string& spec,double comision, int ucenici = 0);
    [[nodiscard]] Angajat* clone() const override { return new StilistSenior(*this); }
    [[nodiscard]] double calculeazaSalariu (const Salon& salon) const override;
    void afiseazaFluturasSalariu(const Salon& salon) const override;
    void editeazaProfilSpecifice() override;

    [[nodiscard]] std::string getGrad() const override { return "Senior"; }
    void adaugaUcenic() {numarUcenici ++; }
    [[nodiscard]] int getNumarUcenici() const { return numarUcenici; }
};

class ManagerSalon : public Angajat {
    double salariuFix;
public:
    ManagerSalon(const std::string& nume_ang, double fix);
    [[nodiscard]] Angajat* clone() const override { return new ManagerSalon(*this); }
    [[nodiscard]] double calculeazaSalariu(const Salon& salon) const override;
    void afiseazaFluturasSalariu(const Salon& salon) const override;
    void editeazaProfilSpecifice() override;
    [[nodiscard]] bool esteManager() const override {return true;}

    [[nodiscard]] std::string getGrad() const override {return "Manager"; }
    void setSalariuFix(double suma) {salariuFix = suma; }
};

class Programare {
    std::string numeClient;
    Angajat* stilist;
    std::vector<Serviciu> servicii;
    std::string* observatii;
    IntervalOrar interval;
    int zi, luna, an;
public:
    Programare( const std::string& client,const Angajat& a, int z, int l , int an_pr, IntervalOrar inter, const std::string& obs = "");
    ~Programare();
    Programare(const Programare& other);
    Programare& operator=(const Programare& other);

    void setStilist(Angajat* s) {stilist = s;}
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

class Salon {
    std::string numeSalon;
    std:: vector<Programare> listaProgramari;
    std::vector <Angajat*> angajatiSalon;

    std::vector<Serviciu> catalogServicii;
public:
    explicit Salon(std:: string nume = "Beauty Salon");

    ~Salon();
    Salon(const Salon& other);
    Salon& operator=(const Salon& other);
    friend void swap(Salon& first, Salon& second) noexcept;

    //metoda pt a popula echipa de angajati
    void adaugaAngajat(Angajat* a);

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

    [[nodiscard]] const std::vector<Angajat*>& getAngajati() const {return angajatiSalon; }
    friend std::ostream& operator<<(std::ostream& os, const Salon& s);

};
#endif
