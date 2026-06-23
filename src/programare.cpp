#include "programare.h"
#include <utility>

bool IntervalOrar::seSuprapuneCu(const IntervalOrar& altul) const {
    //convertim totul in minute
    int start1 = oraStart * 60 + minutStart;
    int final1 = oraFinal * 60 + minutFinal;

    int start2 = altul.oraStart * 60 + altul.minutStart;
    int final2 = altul.oraFinal * 60 + altul.minutFinal;

    // interavlele se suprapun daca s1<f2 si s2<f1
    return (start1 < final2 && start2 < final1);
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
