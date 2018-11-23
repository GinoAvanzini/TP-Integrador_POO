
#include "Eslabon.h"

#include "Conjunto.h"

Eslabon::Eslabon(int ID) : Conjunto (ID){ //eslabon brazo
    this->longitud = 300;
    this->setTipo("Eslabon brazo");
    this->setId(ID);
    this->setPeso(1500);
    this->setDescripcion("Eslabon de brazo de 300mm de largo.");

}

Eslabon::Eslabon(int longitud, int ID) : Conjunto (ID){ //eslabon columna
    this->longitud = longitud;
    this->setTipo("Eslabon columna");
    this->setId(ID);
    this->setPeso(4000);
    this->setDescripcion("Eslabon de columna de 400mm de alto");

}

Eslabon::~Eslabon() {
}
