#include "Salon.h"
#include <algorithm>
#include <iomanip>


//Implementare Salon
Salon::Salon( std::string nume) : numeSalon(std::move(nume)) {
    if (numeSalon.empty()) numeSalon = "Beauty Salon";
}

void Salon:: adaugaAngajat(std::shared_ptr<Angajat> a) {
    if (a != nullptr) {
        angajatiSalon.push_back(a);
    }
}

double Salon::calculeazaIncasariAngajat( const std::string& numeAngajat) const {
    double total = 0.0;

    for (const auto& p : listaProgramari) {
        if (p.getStilist().getNume() == numeAngajat) {
            total += (p.calculeazaTotal() - p.aplicaDiscount());
        }
    }
    return total;
}
double Salon::calculeazaIncasariTotale() const {
    double total = 0 ;
    for (const auto& p : listaProgramari) {
        total += (p.calculeazaTotal() - p.aplicaDiscount());
    }
    return total;
}
void Salon::afiseazaProgramariDupaZi(int z, int l, int a) const {
    std::cout << "\n--- PROGRAMARI PENTRU ZIUA " << z << "/" << l << "/" << a << "---\n";
    bool gasit = false;
    int contor = 1;
    for (const auto& p : listaProgramari) {
        if (p.getZi() == z && p.getLuna() == l && p.getAn() == a) {
            std::cout << "\n(" << contor++ << ") " << p <<"\n";
            gasit = true;
        }
    }
    if (!gasit) std::cout <<"Nu s-au gasit programari inregistrate in aceasta zi.\n";
}

void Salon::afiseazaProgramariDupaLuna(int l, int a) const {
    std::cout<< "\n--- CENTRALIZATOR PROGRAMARI LUNA " << l << "/" << a << "---\n";
    bool gasit = false;
    for (const auto& p : listaProgramari) {
        if (p.getLuna() == l && p.getAn() == a) {
            std::cout << "Ziua " << std::setw(2) <<p.getZi() << " |Client: " << std::left << std::setw(15) << p.getNumeClient()
            << " | Stilist: " <<std::setw(15) <<p.getStilist().getNume()
            << " | Incasare: " << p.calculeazaTotal() - p.aplicaDiscount() << "RON\n";
            gasit = true;
        }
    }
    if (!gasit) std::cout << "Nu exista programari in luna selectata.\n";
}

void Salon::vizualizeazaSalarii() const {
    std::cout << "Incasari totale brute salon: " << calculeazaIncasariTotale() << " RON\n";
    std::cout << "---------------------------------------------------------\n";
    for (const auto& ang : angajatiSalon) {
        ang->afiseazaFluturasSalariu(*this);
    }
}

bool Salon::upgradeAngajatLaSenior(const std::string& numeCautat) {
    for (size_t i = 0; i < angajatiSalon.size(); ++i) {
        if (angajatiSalon[i]->getNume() == numeCautat) {
            if (auto junior = std::dynamic_pointer_cast<StilistJunior>(angajatiSalon[i])) {
                auto senior = std::make_shared<StilistSenior>(numeCautat, junior->getSpecializare(), 0.15, 0);                 std::string nume = junior->getNume();

                for (const auto& comp : junior->getCompetente()) {
                    senior->adaugaCompetenta(comp);
                }

                angajatiSalon[i] = senior;
                for (auto& prg : listaProgramari) {
                    if (prg.getStilist().getNume() == numeCautat) {
                        prg.setStilist(senior);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void Salon::schimbaManagerul(const std::string& numeManagerNou, double salariuFixNou) {
    angajatiSalon.erase(
         std::remove_if(angajatiSalon.begin(), angajatiSalon.end(),
                        [](const std::shared_ptr<Angajat>& ang) { return ang->esteManager(); }),
         angajatiSalon.end()
     );

    auto manager = std::make_shared<ManagerSalon>(numeManagerNou, salariuFixNou);
    angajatiSalon.push_back(manager);
    std::cout << "Managerul salonului a fost schimbat cu succes! Noul manager: " << numeManagerNou << "\n";
}

bool Salon::esteAngajatDisponibil( const Angajat& ang, int z, int l, int a, IntervalOrar inter) const {
    for (const auto& p : listaProgramari) {
        if (p.getStilist().getNume() == ang.getNume() &&
            p.getZi() == z && p.getLuna() == l && p.getAn() == a) {

            if (p.getInterval().seSuprapuneCu(inter)) {
                return false;
            }
        }
    }
    return true;
}

void Salon::afiseazaDisponibilitateAngajat(const std::string& numeAngajat,int zi, int luna, int an,int durataNecesara) const {
    bool gasit = false;

    for (const auto& ang : angajatiSalon) {
        if ( ang->getNume() == numeAngajat) {
            gasit = true;
            std::cout<<"\nIntervale libere pentru " << ang->getNume() << " la data de " << zi << "/" << luna << "/" << an << ":\n";

            bool areIntervale = false;
            for (int h = 9; h<= 18; ++h) {//parcuregere program de lucru
                for (int m: {0, 30}) {
                    IntervalOrar test = {h, m, 0, 0};
                    int totalMinStart = h * 60 + m;
                    int totalMinFinal = totalMinStart + durataNecesara;

                    test.oraFinal = totalMinFinal / 60;
                    test.minutFinal = totalMinFinal % 60;

                    if (test.oraFinal > 19) break;
                    if (esteAngajatDisponibil ( *ang, zi, luna, an, test)) {
                        std::cout << "[ " << h << ":" << (m ==0 ? "00" : "30") << "-"
                                  << test.oraFinal << ":" << (test.minutFinal < 10 ? "0" : "") << test.minutFinal << " ]";
                        areIntervale = true;
                    }
                }
            }
            if (!areIntervale) std::cout <<"Fara intervale libere.";
            std::cout << "\n";
            break;
        }
    }

    if (!gasit) {
        std::cout<<"Eroare: Angajatul " << numeAngajat << " nu a fost gasit in sistem.\n";
    }
}

void Salon::adaugaProgramare(const Programare& p) {
    if (!esteAngajatDisponibil(p.getStilist(), p.getZi(), p.getLuna(), p.getAn(), p.getInterval())) {
        throw SuprapunereIntervalException(p.getStilist().getNume(), p.getInterval().oraStart, p.getInterval().minutStart);
    }
    listaProgramari.push_back(p);
}

void Salon:: afiseazaRaportZilnic() const {
    std::cout << std::setw(25) << "-- Raport zilnic -- "<< numeSalon << "\n";

    if (listaProgramari.empty()) {
        std::cout<<"Nu exista programari inregistrate astazi.\n";
    }else {
        int contor = 1;
        for (const auto& p : listaProgramari) {
            std::cout<<"\n(" << contor++ << ")" << p <<"\n";
            double desc = p.aplicaDiscount();
            if (desc >0) {
                double pretFinal = p.calculeazaTotal() - desc;
                std::cout<< "Discount aplicat: -" << desc << "RON\n";
                std::cout << "    >>> PRET FINAL DE PLATA: " << std::fixed << std::setprecision(2) << pretFinal << " RON\n";
            }
        }
        std::cout<< "\nREZUMAT FINANCIAR:\n";
        std::cout<< "- Incasari: " << std::fixed <<std::setprecision(2)<< calculeazaIncasariTotale() << "RON\n";
        std::cout<<"- Numar total clienti: " <<listaProgramari.size() << "\n";
    }
}
std::ostream& operator<<(std::ostream& os, const Salon& s) {
    os<< "Salon: "<<s.numeSalon <<"- Programari active : "<< s.listaProgramari.size()<< "-";
    return os;
}

