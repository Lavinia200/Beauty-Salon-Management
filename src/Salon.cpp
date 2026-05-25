#include "Salon.h"
#include <utility>
#include <iomanip>


bool IntervalOrar::seSuprapuneCu(const IntervalOrar& altul) const {
    //convertim totul in minute
    int start1 = oraStart * 60 + minutStart;
    int final1 = oraFinal * 60 + minutFinal;

    int start2 = altul.oraStart * 60 + altul.minutStart;
    int final2 = altul.oraFinal * 60 + altul.minutFinal;

    // interavlele se suprapun daca s1<f2 si s2<f1
    return (start1 < final2 && start2 < final1);
}

//Implementare Serviciu
Serviciu::Serviciu(std::string nume_serv, double pret_serv, int durata_serv, std::string t_serv)
    : nume(std::move(nume_serv)), pret(pret_serv), durataMinute(durata_serv), tip(std::move(t_serv)) {
    if (pret_serv < 0 ) {
        std::cerr <<"Pret negativ introdus pentru " << nume << " Setat la 0\n.";
        pret = 0;
    }

    if (durataMinute <= 0) {
        std::cerr <<"Durata invalida pentru " << nume << ", S-a setat la 30 min.\n";
        durataMinute = 30;
    }
}
double Serviciu::getPret() const{return pret;}

std::ostream& operator<<(std::ostream& os, const Serviciu& s) {
    os<< s.getNume() <<"("<<s.getPret()<<"Ron)";
    return os;
}

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

std::ostream& operator<<(std::ostream& os, const Angajat& a) {
    os<< "Stilist:" <<a.nume <<"["<<a.specializare<< " -" << a.getGrad() << "]";
    return os;
}

StilistJunior::StilistJunior(const std::string& nume_ang,const std::string& spec, double tarif)
    : Angajat(nume_ang, spec), tarifOrarBaza(tarif) {}

double StilistJunior::calculeazaSalariu(double) const {
    return tarifOrarBaza * 160.0;
}

StilistSenior::StilistSenior(const std::string& nume_ang,const std::string& spec, double comision, int ucenici)
    : Angajat(nume_ang, spec), procentComision(comision), numarUcenici(ucenici) {}

double StilistSenior::calculeazaSalariu(double incasariSalon) const {
    return 3000.0 + (incasariSalon * procentComision) + (numarUcenici * 200.0);
}

ManagerSalon:: ManagerSalon(const std::string& nume_ang, double fix)
    : Angajat(nume_ang, "Manager Salon"), salariuFix(fix) {}

double ManagerSalon::calculeazaSalariu(double incasariSalon) const {
    return salariuFix + (incasariSalon * 0.01); //+ bonus
}

//Implementare Programare
Programare::Programare( const std::string& client,const Angajat& a, int z, int l, int an_pr, IntervalOrar inter, const std::string& obs )
    : numeClient(client),
      stilist(const_cast<Angajat*>(&a)),
      observatii(obs.empty() ? nullptr : new std::string(obs)),
      interval(inter),
      zi(z),
      luna(l),
      an(an_pr) {}

Programare::~Programare() {
    delete observatii;
}

//copy constructor
Programare::Programare(const Programare& other)
  : numeClient(other.numeClient),
    stilist(other.stilist),
    servicii(other.servicii),
    interval(other.interval),
    zi(other.zi), luna(other.luna), an(other.an) {
    if (other.observatii != nullptr) {
        observatii = new std::string(*other.observatii);
    }else {
        observatii = nullptr;
    }
}

//operator de atribuire
Programare& Programare::operator = (const Programare& other) {
    if (this != &other) {
        numeClient = other.numeClient;
        stilist = other.stilist;
        servicii = other.servicii;
        interval = other.interval;
        zi = other.zi; luna = other.luna; an = other.an;

        delete observatii;
        if (other.observatii != nullptr) {
            observatii = new std::string(*other.observatii);
        }else {
            observatii = nullptr;
        }
    }
    return *this ;
}

void Programare::setObservatii (const std::string& obs) {
    delete observatii;
    if (!obs.empty()) {
        observatii = new std::string(obs);
    }else {
        observatii = nullptr;
    }
}

void Programare::adaugaServiciu (const Serviciu& s){servicii.push_back(s);}

double Programare::calculeazaTotal() const {
    double total = 0;
    for (const auto& s: servicii) total += s.getPret();
    return total;
}

int Programare::calculeazaDurataTotala() const {
    int sumaMinute = 0;
    for ( const auto& s : servicii) {
        sumaMinute += s.getDurata();
    }
    return sumaMinute;
}

bool Programare::estePremium() const{return calculeazaTotal() > 500.0; }

double Programare::aplicaDiscount() const {
    double total = calculeazaTotal();
    double discount = 0;
    if (servicii.size() >= 5) {
        discount = total * 0.20;
    } else if (servicii.size() >= 3) {
        discount = total * 0.10;
    }else if (total > 1000.0) {
        discount = total * 0.05;
    }
    return discount;
}

std::ostream& operator<<(std::ostream& os, const Programare& p) {
    os <<"Programare client " << p.numeClient ;

    if (p.estePremium()) {
        os << " [CLIENT PREMIUM]";
    }
    os << "\n " << *(p.stilist) << "\n Servicii: ";

    if (p.servicii.empty()) {
        os<<"Niciun serviciu selectat.";
    }else {
        for (const auto& s : p.servicii) {
            os << "\n - " <<s;
        }
    }
    if (p.observatii != nullptr) {
        os << "\n Observatii:" << *p.observatii;
    }
    os << "\n Total de plata: " << p.calculeazaTotal() << " RON";
    os <<"\n Timp total estimat: " << p.calculeazaDurataTotala() << "minute";
    if (p.calculeazaDurataTotala() > 120) {
        os << "\n [ Vizita de lunga durata]";
    }
    return os;
}

//Implementare Salon
Salon::Salon( std::string nume) : numeSalon(std::move(nume)) {}

//destructor
Salon::~Salon() {
    for (auto* ang : angajatiSalon) {
        delete ang;
    }
    angajatiSalon.clear();
}

Salon::Salon(const Salon& other) : numeSalon(other.numeSalon), listaProgramari(other.listaProgramari) {
    for ( const auto* ang : other.angajatiSalon) {
        if (ang != nullptr) {
            this->angajatiSalon.push_back(ang->clone());
        }
    }
}

void swap(Salon& first, Salon& second) noexcept {
    using std::swap;
    swap(first.numeSalon, second.numeSalon);
    swap(first.listaProgramari, second.listaProgramari);
    swap(first.angajatiSalon, second.angajatiSalon);
    swap(first.catalogServicii, second.catalogServicii);
}

Salon& Salon::operator=(const Salon& other) {
    if (this != &other) {
        Salon copie(other);
        swap(*this, copie);
    }
    return *this;
}

void Salon:: adaugaAngajat(Angajat* a) {
    if (a != nullptr) {
        angajatiSalon.push_back(a);
    }
}

double Salon::calculeazaIncasariAngajat( const std::string& numeAngajat) const {
    double total = 0;

    for (const auto& p : listaProgramari) {
        if (p.getStilist().getNume() == numeAngajat) {
            total += (p.calculeazaTotal() - p.aplicaDiscount());
        }
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
    double incasariGlobale = calculeazaIncasariTotale();
    std::cout << "Incasari totale salon: " << incasariGlobale << "RON\n";
    for (const auto* ang : angajatiSalon) {
        double argumentFinanciar = incasariGlobale;

        if (ang->getGrad() == "Senior") {
            argumentFinanciar = calculeazaIncasariAngajat(ang->getNume());
        }

        std::cout << "- " << std::left << std::setw(12) << ang->getNume()
        << " [" <<std::setw(15) << ang->getSpecializare() << " - "
        << std::setw(7) << ang->getGrad() << "] -> salariu brut: "
        << std::fixed <<std::setprecision(2) << ang->calculeazaSalariu(argumentFinanciar) << " RON\n";

        if (ang->getGrad() == "Senior") {
            auto* seniorPtr = dynamic_cast<const StilistSenior*>(ang);
            std::cout << " (bazat pe incasari proprii de " << argumentFinanciar << " RON)";
            if (seniorPtr) {
                std::cout << " | ucenici coordonati: " << seniorPtr->getNumarUcenici();
            }
            std::cout << ")";
        }
        std::cout<< "\n";
    }
}

bool Salon::upgradeAngajatLaSenior(const std::string& numeCautat) {
    for (size_t i = 0; i < angajatiSalon.size(); ++i) {
        if (angajatiSalon[i]->getNume() == numeCautat) {
            const auto* junior = dynamic_cast<StilistJunior*>(angajatiSalon[i]);
            if (junior != nullptr) {
                std::string nume = junior->getNume();
                std::string spec = junior->getSpecializare();
                auto competenteSalvate = junior->getCompetente();

                delete angajatiSalon[i];

                auto* senior = new StilistSenior(nume, spec, 0.12, 0);
                for (const auto& comp : competenteSalvate) {
                    senior->adaugaCompetenta(comp);
                }

                angajatiSalon[i] = senior;
                for (auto& p : listaProgramari) {
                    if (p.getStilist().getNume() == nume) {
                        p.setStilist(senior);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void Salon:: schimbaManagerul(const std::string& numeManagerNou, double salariuFixNou) {
    for (size_t i= 0; i < angajatiSalon.size(); ++i) {
        if (dynamic_cast<ManagerSalon*>(angajatiSalon[i]) != nullptr) {
            delete angajatiSalon[i];
            angajatiSalon.erase(angajatiSalon.begin() + i);
            break;
        }
    }
    auto* nouManager = new ManagerSalon(numeManagerNou, salariuFixNou);
    adaugaAngajat(nouManager);
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

    for (const auto* ang : angajatiSalon) {
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
    listaProgramari.push_back(p);
}
double Salon::calculeazaIncasariTotale() const {
    double total = 0 ;
    for (const auto& p : listaProgramari) {
        total += (p.calculeazaTotal() - p.aplicaDiscount());
    }
    return total;
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

