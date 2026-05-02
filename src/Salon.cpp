#include "Salon.h"
#include <utility>




//Implementare Serviciu
Serviciu::Serviciu(std::string nume_serv, double pret_serv)
    : nume(std::move(nume_serv)), pret(pret_serv){}
double Serviciu::getPret() const{return pret;}
std::ostream& operator<<(std::ostream& os, const Serviciu& s) {
    os<< s.getNume() <<"("<<s.getPret()<<"Ron)";
    return os;
}

//Implementare Angajat
Angajat::Angajat(std::string nume_ang, std::string spec)
    :nume(std::move(nume_ang)), specializare(std::move(spec)) {}
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
    os <<"Programare client" << p.numeClient <<"\n "
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

//Implementare Salon

Salon::Salon( std::string nume) : numeSalon(std::move(nume)) {}
void Salon::adaugaProgramare(const Programare& p) {
    listaProgramari.push_back(p);
}
double Salon::calculeazaIncasariTotale() const {
    double total = 0 ;
    for (const auto& p : listaProgramari) {
        total += p.calculeazaTotal();
    }
    return total;
}
void Salon:: afiseazaRaportZilnic() const {
    std::cout<< "-- Raport zilnic -- "<< numeSalon << "\n";
    for (const auto& p : listaProgramari) {
        std::cout<<p<<std::endl;
    }
    std::cout<< "Total incasari "<< calculeazaIncasariTotale() <<"RON\n";
}
std::ostream& operator<<(std::ostream& os, const Salon& s) {
    os<< "Salon: "<<s.numeSalon <<"- Programari active : "<< s.listaProgramari.size()<< "-";
    return os;
}