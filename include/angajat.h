#ifndef ANGAJAT_H
#define ANGAJAT_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

class Salon;

class Angajat {
protected:
    std::string nume;
    std::string specializare;
    std::vector <std::string> competente;

    static int numarTotalAngajati;
public:
    explicit Angajat(std::string nume_ang = "Anonim", std::string spec = "General");
    virtual ~Angajat();
    virtual std::shared_ptr<Angajat> clone() const = 0;

    [[nodiscard]] virtual double calculeazaSalariu(const Salon& salon) const  = 0;
    virtual void afiseazaFluturasSalariu(const Salon& salon) const = 0;
    virtual void editeazaProfilSpecifice() = 0;

    [[nodiscard]] virtual bool esteManager() const {return false;}

    [[nodiscard]] virtual std::string getGrad() const = 0;

    void adaugaCompetenta(const std::string& categorie) {competente.push_back(categorie);} // metoda pt adaugare serviciu prestat
    [[nodiscard]] const std::vector<std::string>& getCompetente() const { return competente; }
    [[nodiscard]] bool poatePresta(const std::string& categorieCautata) const;

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
    std::shared_ptr<Angajat> clone() const override {return std::make_shared<StilistJunior>(*this);}
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
    std::shared_ptr<Angajat> clone() const override {return std::make_shared<StilistSenior>(*this);}
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
    std::shared_ptr<Angajat> clone() const override {return std::make_shared<ManagerSalon>(*this);}
    [[nodiscard]] double calculeazaSalariu(const Salon& salon) const override;
    void afiseazaFluturasSalariu(const Salon& salon) const override;
    void editeazaProfilSpecifice() override;
    [[nodiscard]] bool esteManager() const override {return true;}

    [[nodiscard]] std::string getGrad() const override {return "Manager"; }
    void setSalariuFix(double suma) {salariuFix = suma; }
};
#endif