#include "controlador.h"

#include <Qt3DExtras/QPlaneMesh>

#include <stdlib.h>

Controlador::Controlador(Qt3DCore::QEntity * rootEntity)
{

    //Render de Base Robot y las articulaciones
    {
        QUrl path = QStringLiteral("qrc:/res/base_robot.obj");
        this->BRobot = new BaseRobot(0, false, "192.168.1.10", rootEntity, path);
    }

    Qt3DCore::QEntity * floor = new Qt3DCore::QEntity(rootEntity);
    Qt3DExtras::QPlaneMesh * floorMesh = new Qt3DExtras::QPlaneMesh();
    Qt3DCore::QTransform * floorTransform = new Qt3DCore::QTransform();
    floorTransform->setScale(100);
    floorTransform->setTranslation(QVector3D(0, -10, 0));
    Qt3DExtras::QPhongMaterial * floorMat = new Qt3DExtras::QPhongMaterial();
    floorMat->setDiffuse((QColor(QRgb(0x9db1f0))));
    floor->addComponent(floorMat);
    floor->addComponent(floorMesh);
    floor->addComponent(floorTransform);



    this->BRobot->inicio(rootEntity);


    // Instancio contenedor con la animacion
    this->secuencia = new QSequentialAnimationGroup();


}

void Controlador::agregarInstruccion(std::string instruc){
    this->instrucciones.push(instruc);
    if (instruc == "C00"){
        this->interprete();
    }
}

void Controlador::interprete(){

    int ID;
    bool sentido = true;
    int velocidad;
    int avance;

    bool flag = true;

    std::string aux;
    while(!this->instrucciones.empty()){
        aux = this->instrucciones.front();
        this->instrucciones.pop();
        if(aux.front() == 'G'){
            aux.erase(0, 1);
            if (aux.front() == '1'){ //actuador lineal
                aux.erase(0, 1);
                ID = 10;
            } else if (aux.front() == '2'){ //Primera articulacion
                aux.erase(0, 1);
                ID = 11;
            } else if (aux.front() == '3'){  // Segunda articulacion
                aux.erase(0, 1);
                ID = 12;
            }

            //Implementación de homing de altura G50
//                else if (aux.front() == '5'){
//                aux.erase(0, 2);
//                ID = 10;
//                sentido = false;
//                avance = abs(this->alturaAbsoluta);
//                flag = false;
//                velocidad = 20;

//            }

            if (aux.front() == '1' && flag){
                sentido = true;
            } else if (aux.front() == '2' && flag) {
                sentido = false;
            }
            if (flag) { // Si es G50 no entra acá
                aux = this->instrucciones.front();
                this->instrucciones.pop();
                velocidad = std::stoi(aux, nullptr, 10);
                aux = this->instrucciones.front();
                this->instrucciones.pop();
                avance = std::stoi(aux, nullptr, 10);
            }

            this->agregarAnimacion(ID, sentido, velocidad, avance);

        } else if (aux.front() == 'C') {
            this->startAnimacion();
        }
    }


}

void Controlador::agregarAnimacion(int ID, bool sentido, int velocidad, int avance){

    int signo;

    if (sentido == true){
        signo = 1;
    } else {
        signo = -1;
    }

    switch(ID){

        case 10:  //Actuador Lineal

            this->animaciones.push_front(new QPropertyAnimation(this->BRobot->ActLineal->getTransform()));
            this->paralelo.push_front(new QParallelAnimationGroup());

            this->animaciones.front()->setTargetObject(this->BRobot->ActLineal->controlpieza);
            this->animaciones.front()->setPropertyName("altura");
            this->animaciones.front()->setStartValue(QVariant::fromValue(this->alturaAbsoluta));
            this->animaciones.front()->setEndValue(QVariant::fromValue(this->alturaAbsoluta + signo*avance));
            this->animaciones.front()->setDuration(1000*avance/velocidad);

            this->paralelo.front()->addAnimation(this->animaciones.front());


            this->animaciones.push_front(new QPropertyAnimation(this->BRobot->articulacion1->getTransform()));

            this->animaciones.front()->setTargetObject(this->BRobot->articulacion1->controlpieza);
            this->animaciones.front()->setPropertyName("altura");
            this->animaciones.front()->setStartValue(QVariant::fromValue(this->alturaAbsoluta));
            this->animaciones.front()->setEndValue(QVariant::fromValue(this->alturaAbsoluta + signo*avance));
            this->animaciones.front()->setDuration(1000*avance/velocidad);


            this->paralelo.front()->addAnimation(this->animaciones.front());


            this->animaciones.push_front(new QPropertyAnimation(this->BRobot->articulacion2->getTransform()));

            this->animaciones.front()->setTargetObject(this->BRobot->articulacion2->controlpieza);
            this->animaciones.front()->setPropertyName("altura");
            this->animaciones.front()->setStartValue(QVariant::fromValue(this->alturaAbsoluta));
            this->animaciones.front()->setEndValue(QVariant::fromValue(this->alturaAbsoluta + signo*avance));
        this->animaciones.front()->setDuration(1000*avance/velocidad);

        this->paralelo.front()->addAnimation(this->animaciones.front());


        this->alturaAbsoluta += signo*avance;

        this->secuencia->addAnimation(this->paralelo.front());

        break;


    case 11: // Primera articulación. Mueve ambas articulaciones

        this->animaciones.push_front(new QPropertyAnimation(this->BRobot->articulacion1->getTransform()));
        this->paralelo.push_front(new QParallelAnimationGroup());

        this->animaciones.front()->setTargetObject(this->BRobot->articulacion1->controlpieza);

        this->animaciones.front()->setPropertyName("angle");
        this->animaciones.front()->setStartValue(QVariant::fromValue(this->ang1Absoluto));
        this->animaciones.front()->setEndValue(QVariant::fromValue(this->ang1Absoluto + signo*avance));
        this->animaciones.front()->setDuration(1000*avance/velocidad);

        this->paralelo.front()->addAnimation(this->animaciones.front());



        this->animaciones.push_front(new QPropertyAnimation(this->BRobot->articulacion2->getTransform()));

        this->animaciones.front()->setTargetObject((this->BRobot->articulacion2->controlpieza));

        this->animaciones.front()->setPropertyName("angle");
        this->animaciones.front()->setStartValue(QVariant::fromValue(this->ang1Absoluto));
        this->animaciones.front()->setEndValue(QVariant::fromValue(this->ang1Absoluto + signo*avance));
        this->animaciones.front()->setDuration(1000*avance/velocidad);

        this->paralelo.front()->addAnimation(this->animaciones.front());


        this->secuencia->addAnimation(this->paralelo.front());

        this->ang1Absoluto += signo*avance;

        break;







    }
}


void Controlador::startAnimacion(){
    this->secuencia->start();
}

bool Controlador::getEstadoBR(){
    return this->BRobot->getEstado();
}
void Controlador::setEstadoBR(bool estado){
    this->BRobot->setEstado(estado);
}
