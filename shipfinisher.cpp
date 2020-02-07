#include "shipfinisher.h"
#include "cell.h"
#include <QDebug>
extern const int kNumberSquaresInRow;


ShipFinisher::ShipFinisher(const CellMatrix& new_CellMatrix) :
    users_cell_matrix_(new_CellMatrix)
{
    ships_orientation = SO::NOT_DEFINED;
}

bool ShipFinisher::FinishShip(const QPoint &wounded_deck)
{
    wounded_deck_ = wounded_deck;
    ships_orientation = SO::NOT_DEFINED;
    return true;
}

QPoint ShipFinisher::FinishShip()
{
    TryToDefineShipsOrientation();
    if ( ships_orientation != SO::NOT_DEFINED )
    {
        return FinishShipOnDefinedShipsOrientation();
    }
    // chances that orientation of ship is horizontal or vertical is equal
    if ( (UnharmedDeckCanBeLeft() != QPoint(-1,-1)) + (UnharmedDeckCanBeRight() != QPoint(-1,-1))
         ==
         (UnharmedDeckCanBeUp() != QPoint(-1,-1)) + (UnharmedDeckCanBeDown() != QPoint(-1,-1)) )
    {
        return FinishShipOnEqualChances();
    }

    return FinishShipOnNotEqualChances();
}

QPoint ShipFinisher::UnharmedDeckCanBeLeft()
{
    if ( ships_orientation == SO::VERTICAL )
    {
        return QPoint(-1,-1);
    }
    int i = 0;
    while ( (wounded_deck_.x() - i >= 0) && (users_cell_matrix_[wounded_deck_.x() - i][wounded_deck_.y()].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
           i++;
    }
    if ( wounded_deck_.x() - i < 0 )
    {
        return QPoint(-1, -1);
    }

    if ( users_cell_matrix_[wounded_deck_.x() - i][wounded_deck_.y()].GetImage() != Cell::SE::EMPTY )
    {
        return QPoint(-1, -1);
    }

    return users_cell_matrix_[wounded_deck_.x() - i][wounded_deck_.y()].GetCoord();
}

QPoint ShipFinisher::UnharmedDeckCanBeRight()
{
    if ( ships_orientation == SO::VERTICAL )
    {
        return QPoint(-1,-1);
    }
    int i = 0;
    while ( (wounded_deck_.x() + i != kNumberSquaresInRow) && (users_cell_matrix_[wounded_deck_.x() + i][wounded_deck_.y()].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
           i++;
    }
    if ( wounded_deck_.x() + i == kNumberSquaresInRow )
    {
        return QPoint(-1, -1);
    }

    if ( users_cell_matrix_[wounded_deck_.x() + i][wounded_deck_.y()].GetImage() != Cell::SE::EMPTY )
    {
        return QPoint(-1, -1);
    }

    return users_cell_matrix_[wounded_deck_.x() + i][wounded_deck_.y()].GetCoord();
}

QPoint ShipFinisher::UnharmedDeckCanBeUp()
{
    if ( ships_orientation == SO::HORIZONTAL )
    {
        return QPoint(-1,-1);
    }
    int i = 0;
    while ( (wounded_deck_.y() - i >= 0) && (users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() - i].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
           i++;
    }
    if ( wounded_deck_.y() - i < 0 )
    {
        return QPoint(-1, -1);
    }

    if ( users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() - i].GetImage() != Cell::SE::EMPTY )
    {
        return QPoint(-1, -1);
    }

    return users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() - i].GetCoord();
}

QPoint ShipFinisher::UnharmedDeckCanBeDown()
{
    if ( ships_orientation == SO::HORIZONTAL )
    {
        return QPoint(-1,-1);
    }
    int i = 0;
    while ( (wounded_deck_.y() + i != kNumberSquaresInRow) && (users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() + i].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
           i++;
    }
    if ( wounded_deck_.y() + i == kNumberSquaresInRow )
    {
        return QPoint(-1, -1);
    }

    if ( users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() + i].GetImage() != Cell::SE::EMPTY )
    {
        return QPoint(-1, -1);
    }

    return users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() + i].GetCoord();
}
// Finish ship when it is equal chances, that ship has horizontal
// or vertical orientation
QPoint ShipFinisher::FinishShipOnEqualChances()
{
    int i = 0;
    QPoint m[4]; // probable coordinates of shoot
    if ( UnharmedDeckCanBeUp() != QPoint(-1,-1) )
    {
        m[i] = UnharmedDeckCanBeUp();
        i++;
    }
    if ( UnharmedDeckCanBeRight() != QPoint(-1,-1) )
    {
        m[i] = UnharmedDeckCanBeRight();
        i++;
    }
    if ( UnharmedDeckCanBeDown() != QPoint(-1,-1) )
    {
        m[i] = UnharmedDeckCanBeDown();
        i++;
    }
    if ( UnharmedDeckCanBeLeft() != QPoint(-1,-1) )
    {
        m[i] = UnharmedDeckCanBeLeft();
        i++;
    }
    return m[ rand()%i ];
}

// Returns recommended coordinates of shoot when not defined ships orientation
// and
QPoint ShipFinisher::FinishShipOnNotEqualChances()
{
    if ( UnharmedDeckCanBeLeft() == QPoint(-1,-1) || UnharmedDeckCanBeRight() == QPoint(-1,-1) )
    {
        if ( rand()%2 )
        {
            return UnharmedDeckCanBeUp();
        }
        else
        {
            return UnharmedDeckCanBeDown();
        }
    }
    else
    {
        if ( rand()%2 )
        {
            return UnharmedDeckCanBeLeft();
        }
        else
        {
            return UnharmedDeckCanBeRight();
        }
    }
}

QPoint ShipFinisher::FinishShipOnDefinedShipsOrientation()
{
    if ( ships_orientation == SO::HORIZONTAL )
    {
        if ( UnharmedDeckCanBeRight() == QPoint(-1,-1) )
        {
            return UnharmedDeckCanBeLeft();
        }
        if ( UnharmedDeckCanBeLeft() == QPoint(-1,-1) )
        {
            return UnharmedDeckCanBeRight();
        }
        if ( rand()%1 )
        {
            return UnharmedDeckCanBeLeft();
        }
        else
        {
            return UnharmedDeckCanBeRight();
        }

    }
    if ( ships_orientation == SO::VERTICAL )
    {
        if ( UnharmedDeckCanBeUp() == QPoint(-1,-1) )
        {
            return UnharmedDeckCanBeDown();
        }
        if ( UnharmedDeckCanBeDown() == QPoint(-1,-1) )
        {
            return UnharmedDeckCanBeUp();
        }
        if ( rand()%1 )
        {
            return UnharmedDeckCanBeUp();
        }
        else
        {
            return UnharmedDeckCanBeDown();
        }
    }
    return QPoint(-1,-1);
}

bool ShipFinisher::TryToDefineShipsOrientation()
{
    // define orientation using WOUNDED_DECK image
    if ( (wounded_deck_.x() - 1 >= 0 && users_cell_matrix_[wounded_deck_.x() - 1][wounded_deck_.y()].GetImage() == Cell::SE::WOUNDED_DECK)
         ||
         (wounded_deck_.x() + 1 < kNumberSquaresInRow && users_cell_matrix_[wounded_deck_.x() + 1][wounded_deck_.y()].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
        ships_orientation = SO::HORIZONTAL;
        return true;
    }
    if ( (wounded_deck_.y() - 1 >= 0 && users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() - 1].GetImage() == Cell::SE::WOUNDED_DECK)
         ||
         (wounded_deck_.y() + 1 < kNumberSquaresInRow && users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() + 1].GetImage() == Cell::SE::WOUNDED_DECK) )
    {
        ships_orientation = SO::VERTICAL;
        return true;
    }

    // define orientation using POINT image
    if ( (wounded_deck_.x() - 1 < 0 || users_cell_matrix_[wounded_deck_.x() - 1][wounded_deck_.y()].GetImage() == Cell::SE::POINT)
         &&
         (wounded_deck_.x() + 1 == kNumberSquaresInRow || users_cell_matrix_[wounded_deck_.x() + 1][wounded_deck_.y()].GetImage() == Cell::SE::POINT) )
    {
        ships_orientation = SO::VERTICAL;
        return true;
    }
    if ( (wounded_deck_.y() - 1 < 0 || users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() - 1].GetImage() == Cell::SE::POINT)
         &&
         (wounded_deck_.y() + 1 == kNumberSquaresInRow || users_cell_matrix_[wounded_deck_.x()][wounded_deck_.y() + 1].GetImage() == Cell::SE::POINT) )
    {
        ships_orientation = SO::HORIZONTAL;
        return true;
    }
    return false;
}

