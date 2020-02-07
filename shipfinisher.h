#ifndef SHIPFINISHER_H
#define SHIPFINISHER_H

#include <QPoint>
#include "abstractfield.h"

class Cell;

namespace
{
    typedef Cell CellMatrix[kNumberSquaresInRow][kNumberSquaresInRow];
}

class ShipFinisher
{
public:
    ShipFinisher(const CellMatrix &new_CellMatrix);
    bool FinishShip(const QPoint &wounded_deck);
    QPoint FinishShip();    
protected:
    enum class SO { NOT_DEFINED, HORIZONTAL, VERTICAL }; // ship(what it should to finish) orientation
    // Shooting direction. When it is found which side to shoot to kill the ship
    enum class SD { NOT_DEFINED, UP, RIGHT, DOWN, LEFT };

    SO ships_orientation;
    SD shooting_direction_;
    QPoint wounded_deck_;
    const CellMatrix& users_cell_matrix_;

    bool TryToDefineShipsOrientation();
    QPoint UnharmedDeckCanBeLeft();
    QPoint UnharmedDeckCanBeRight();
    QPoint UnharmedDeckCanBeUp();
    QPoint UnharmedDeckCanBeDown();
private:
    QPoint FinishShipOnEqualChances();
    QPoint FinishShipOnNotEqualChances();
    QPoint FinishShipOnDefinedShipsOrientation();

};

#endif // SHIPFINISHER_H
