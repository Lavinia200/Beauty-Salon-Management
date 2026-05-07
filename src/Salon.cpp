#include "Salon.h"
#include <utility>



bool IntervalOrar::seSuprapuneCu(const IntervalOrar& altul) const {
    //convertim totul in minute
    int start1 = oraStart * 60 + altul.minutStart;
    int final1 = oraFinal * 60 + altul.minutFinal;

    int start2 = altul.oraStart * 60 + altul.minutStart;
    int final2 = altul.oraFinal * 60 + altul.minutFinal;

    // interavlele se suprapun daca s1<f2 si s2<f1
    return (start1 < final2 && start2 < final1);
}

//Implementare Serviciu
Serviciu::Serviciu(std::string nume_serv, double pret_serv, int durata_serv, TipServiciu t_serv)
    : nume(std::move(nume_serv)), pret(pret_serv), durataMinute(durata_serv), tip(t_serv) {
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
Angajat::Angajat(std::string nume_ang, std::string spec)
    :nume(std::move(nume_ang)), specializare(std::move(spec)) {
    if (nume.empty()) nume = "Anonim";
    if (specializare.empty()) specializare = "General";
}
std::ostream& operator<<(std::ostream& os, const Angajat& a) {
    os<< "Stilist:" <<a.nume <<"["<<a.specializare<< "]";
    return os;
}

//Implementare Programare
Programare::Programare( const std::string& client,const Angajat& a, int z, int l, int an_pr, IntervalOrar inter, const std::string& obs )
    : numeClient(client),
      stilist(a),
      zi(z),
      luna(l),
      an(an_pr),
      interval(inter) {
    if (!obs.empty()) {
        observatii = new std:: string(obs);
    }else {
        observatii = nullptr;
    }
}

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
std::ostream& operator<<(std::ostream& os, const Programare& p) {
    os <<"Programare client " << p.numeClient <<"\n "
    <<p.stilist <<"\n Servicii:";

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
    os << "\n Total de plata: " << p.calculeazaTotal() << "RON";
    os <<"\n Timp total estimaty: " << p.calculeazaDurataTotala() << "minute";
    return os;
}

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

//Implementare Salon

Salon::Salon( std::string nume) : numeSalon(std::move(nume)) {}

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
        if ( ang.getNume() == numeAngajat) {
            gasit = true;
            std::cout<<"\nIntervale libere pentru " << ang.getNume() << " la data de " << zi << "/" << luna << "/" << an << ":\n";

            bool areIntervale = false;
            for (int h = 9; h<= 18; ++h) {//parcuregere program de lucru
                for (int m: {0, 30}) {
                    IntervalOrar test = {h, m, 0, 0};
                    int totalMinStart = h * 60 + m;
                    int totalMinFinal = totalMinStart + durataNecesara;

                    test.oraFinal = totalMinFinal / 60;
                    test.minutFinal = totalMinFinal % 60;

                    if (test.oraFinal > 19) break;
                    if (esteAngajatDisponibil ( ang, zi, luna, an, test)) {
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

