
#ifndef INTERFATA_UTILIZATOR_H
#define INTERFATA_UTILIZATOR_H

#include "Salon.h"
class InterfataUtilizator {
private:
    static void curataBuffer();
    void executaModClient(Salon& salon);
    void executaModManager(Salon& salon);
public:
    void ruleaza(Salon& salon);
};
#endif