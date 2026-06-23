#include "servicii.h"
#include <utility>

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
