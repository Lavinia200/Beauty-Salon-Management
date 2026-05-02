#include "Salon.h"
#include <utility>




//Implementare Serviciu
Serviciu::Serviciu(std::string nume_serv, double pret_serv)
    : nume(std::move(nume_serv)), pret(pret_serv) {
    if (pret_serv < 0 ) {
        std::cerr <<"Pret negativ introdus pentru " << nume << " Setat la 0\n.";
        pret = 0;
    } else {
        pret = pret_serv;
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
Programare::Programare( const std::string& client,const Angajat& a, const std::string& obs )
    : numeClient(client), stilist(a) {
    if (!obs.empty()) {
        observatii = new std:: string(obs);
    }else {
        observatii = nullptr;
    }
}

Programare::~Programare() {
    delete observatii;
}

Programare::Programare(const Programare& other)
    :numeClient(other.numeClient), stilist(other.stilist), servicii(other.servicii) {
    if (other.observatii != nullptr) {
        observatii = new std::string(*other.observatii);
    }else {
        observatii = nullptr;
    }
}
Programare& Programare::operator = (const Programare& other) {
    if (this != &other) {
        numeClient = other.numeClient;
        stilist = other.stilist;
        servicii = other.servicii;

        delete observatii;
        if (other.observatii != nullptr) {
            observatii = new std::string(*other.observatii);
        }else {
            observatii = nullptr;
        }
    }
    return *this ;
}

void Programare::adaugaServiciu (const Serviciu& s){servicii.push_back(s);}
double Programare::calculeazaTotal() const {
    double total = 0;
    for (const auto& s: servicii) total += s.getPret();
    return total;
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
    return os;
}

double Programare::aplicaDiscount() const {
    double total = calculeazaTotal();
    double discount = 0;

    if (servicii.size() >= 3) {
        discount = total * 0.10;
    } else if (total > 1000.0) {
        discount = total * 0.05;
    }
    return discount;
}
//Implementare Salon

Salon::Salon( std::string nume) : numeSalon(std::move(nume)) {}
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

