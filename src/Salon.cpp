#include "Salon.h"
#include <utility>

// This also works if you do not want `../`, but some editors might not like it
// #include "Example.h"


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
    os<< "Stlist:" <<a.nume <<"["<<a.specializare<< "]";
    return os;
}

//Implementare Programare
Programare::Programare(std::string client, Angajat a, int id )
    : numeClient(std::move(client)), stilist(std::move(a)) {
    idProgramare= new int(id);
}

Programare::~Programare(){delete idProgramare; }
Programare::Programare(const Programare& other)
    :numeClient(other.numeClient), stilist(other.stilist), servicii(other.servicii) {
    idProgramare = new int(*other.idProgramare);
}
Programare& Programare::operator = (const Programare& other) {
    if (this != &other) {
        numeClient = other.numeClient;
        stilist = other.stilist;
        servicii = other.servicii;
        int* new_id = new int(*other.idProgramare);
        delete idProgramare;
        idProgramare = new_id;
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
    os <<"Programarea #" << *p.idProgramare <<" pentru " <<p.numeClient<<"\n"<<" "
    <<p.stilist <<"\n Total:"<< p.calculeazaTotal()<< "Ron";
    return os;
}
