#include "angajat.h"
#include "Salon.h"
#include <utility>
#include <iomanip>
#include <algorithm>
#include <limits>

//Implementare Angajat
int Angajat::numarTotalAngajati = 0;

Angajat::Angajat(std::string nume_ang, std::string spec)
    :nume(std::move(nume_ang)), specializare(std::move(spec)) {
    if (nume.empty()) nume = "Anonim";
    if (specializare.empty()) specializare = "General";
    numarTotalAngajati++;
}
Angajat::~Angajat() {
    numarTotalAngajati--;
}

bool Angajat::poatePresta(const std::string& categorieCautata) const {
    for (const auto& cat : competente) {
        if (cat == categorieCautata) return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Angajat& a) {
    os<< "Stilist:" <<a.nume <<"["<<a.specializare<< " -" << a.getGrad() << "]";
    return os;
}

StilistJunior::StilistJunior(const std::string& nume_ang,const std::string& spec, double tarif)
    : Angajat(nume_ang, spec), tarifOrarBaza(tarif) {}

double StilistJunior::calculeazaSalariu(const Salon& salon) const {
    double baza = tarifOrarBaza * 160.0;
    if (salon.calculeazaIncasariTotale() > 3000.0) {
        baza += 250.0;
    }
    return baza;
}
void StilistJunior::afiseazaFluturasSalariu(const Salon& salon) const {
    double total = calculeazaSalariu(salon);
    std::cout << "- " << std::left << std::setw(12) << nume
        << " [" << std::setw(7) << getGrad() << "] -> Salariu brut: "
        << std::fixed << std::setprecision(2) << total << " RON";
    if (salon.calculeazaIncasariTotale() > 3000.0) {
        std:: cout << " (Include +250.00 RON bonus performanta colectiva)";
    }
    std::cout << "\n";
}
void StilistJunior::editeazaProfilSpecifice() {
    std::cout << "-> Angajatii de rang Junior nu contin atribute administrative avansate editabile.\n";
}

StilistSenior::StilistSenior(const std::string& nume_ang,const std::string& spec, double comision, int ucenici)
    : Angajat(nume_ang, spec), procentComision(comision), numarUcenici(ucenici) {}

double StilistSenior::calculeazaSalariu(const Salon& salon) const {
    double incasariProprii = salon.calculeazaIncasariAngajat(nume);
    double total = 3000.0 + (incasariProprii * procentComision) + (numarUcenici * 200.0);
    if (incasariProprii > 1000.0) {
        total += 350.0;
    }
    return total;
}
void StilistSenior::afiseazaFluturasSalariu(const Salon& salon) const {
    double incasariProprii = salon.calculeazaIncasariAngajat(nume);
    double total = calculeazaSalariu(salon);
    std::cout << "- " << std::left << std::setw(12) << nume
        << " [" << std::setw(15) << specializare << " - "
        << std::setw(7) << getGrad() << "] -> Salariu brut: "
        << std::fixed << std::setprecision(2) << total << " RON\n"
        << " [Detalii:  Baza garantata 3000 RON | Comision: " << procentComision * 100 << "% din " << incasariProprii << " RON"
        << " | Ucenici in subordine: " << getNumarUcenici() << " (*200 RON)]";
    if ( incasariProprii > 1000.0 ) {
        std::cout << "\n >>> Extra-Bonus atins: +350.00 RON adaugati pentru depasire target vanzari.";
    }
    std:: cout << "\n";
}

void StilistSenior::editeazaProfilSpecifice() {
    std::cout << "Proprietati specifice Senior: \n1. Aloca un ucenic nou in subordine\nSelectie: ";
    int op; std::cin>> op;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (op == 1) {
        adaugaUcenic();
        std::cout << "Modificare salvata. Total ucenici curent: " << numarUcenici << "\n";
    }
}

ManagerSalon:: ManagerSalon(const std::string& nume_ang, double fix)
    : Angajat(nume_ang, "Manager Salon"), salariuFix(fix) {}

double ManagerSalon::calculeazaSalariu(const Salon& salon) const {
    double incasariSalon = salon.calculeazaIncasariTotale();
    double total = salariuFix + (incasariSalon * 0.01);
    if (incasariSalon > 5000.0) {
        total += 500.0;
    }
    return total;
}

void ManagerSalon::afiseazaFluturasSalariu(const Salon& salon) const {
    double incasariSalon = salon.calculeazaIncasariTotale();
    double total = salariuFix + (incasariSalon * 0.01);
    std::cout << "- " << std::left << std::setw(12) << nume
        << " [" << std::setw(15) << specializare << " - "
        << std::setw(7) << getGrad() << "] -> Salariu brut: "
        << std::fixed << std::setprecision(2) << total << " RON\n"
        << " [Detalii: Contract administrativ " << salariuFix << " RON + 1% bonus din incasari globale (" << incasariSalon << " RON]";
    if (incasariSalon > 5000.0) {
        std::cout << "\n >>> Extra-bonus atins: +500.00 RON adaugati pentru eficienta operationala a salonului.";
    }
    std::cout << "\n";
}
void ManagerSalon::editeazaProfilSpecifice() {
    std::cout << "Proprietati specifice Manager:\nIntroduceti noul salariu de baza fix administrativ: ";
    double salNou; std::cin >> salNou;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (salNou > 0) {
        setSalariuFix(salNou);
        std::cout << "Modificare salvata. Noul salariu de baza fix este: " << salariuFix << " RON\n";
    }
}
