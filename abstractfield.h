#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H

#include <stdio.h>
#include "cell.h"
#include "ship.h"

class MainWindow;

const int kSquareLength = 25;
const int kLineWidth = 4;
const int kNumberSquaresInRow = 10;
const int kMaxNumberShipOnAbstractField = 10;

namespace
{
    typedef Cell CellMatrix[kNumberSquaresInRow][kNumberSquaresInRow];
}

class AbstractField : public QWidget
{ 
public:
    AbstractField(QWidget *parent, const int &LT_x, const int &LT_y, MainWindow *new_main_window);
    void SetShip(Ship& newShip, const int &x_coord, const int &y_coord);
    QPoint ShipToCellCoordinate(const Ship& ship) const;
    bool CanPutShip(const QPoint &LT_angle, const int& number_decks, const Ship::DO& orientation) const;
    virtual void PutShipOnCells(Ship &ship, const int& x_coord, const int& y_coord);
    virtual void TakeOffShipFromCells(Ship &ship, const int& x_coord, const int& y_coord);
    const Cell &GetCell(const int& X, const int& Y);
    virtual bool ShootIntoCell(const int& X, const int& Y);
    void AddOwnShip(Ship* new_ship);
    int GetNumberShips() const;
    const vector<Ship*>& GetOwnShips() const;
    void InstallIgnoreFilterOnOwnShips();
    void SetItselfParentToOwnShips();
    const CellMatrix& GetCellMatrix() const;
    bool IsKilledShip(const Cell& wounded_deck);

protected:
    MainWindow* main_window_;
    int number_ships_; // number ships, that situated on AbstractField at the moment
    Cell cell_matrix_[kNumberSquaresInRow][kNumberSquaresInRow];
    vector<Ship*> own_ships_;
    void SetShipOnCellsAsSE(const Ship &ship, const int& x_coord, const int& y_coord, const Cell::SE new_state);
    void PutDotsAroundKilledShip(const Cell &cell);
    bool eventFilter(QObject *target, QEvent *event);

private:
   void DrawField(QWidget* parent, const int& LT_x, const int& LT_y);
   void paintEvent(QPaintEvent*);
   void PutDotsOnEmptyCellsAroundCell(const Cell& cell);

};

#endif // ABSTRACTFIELD_H
