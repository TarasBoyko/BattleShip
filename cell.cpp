#include "cell.h"
#include "abstractfield.h"
#include <QDebug>

// Set Cell to cross x row and y column in AbstractField
void Cell::SetCoordinate(const int &x, const int &y)
{
    int new_x_coordinate = x*(kSquareLength + kLineWidth) + kLineWidth;
    int new_y_coordinate = y*(kSquareLength + kLineWidth) + kLineWidth;
    x_ = x;
    y_ = y;
    setGeometry(new_x_coordinate, new_y_coordinate, kSquareLength, kSquareLength);
}

void Cell::SetImage(const Cell::SE &newImage)
{
    if ( newImage == Cell::SE::DECK )
    {
        setPixmap( QPixmap("deck.bmp") );
        current_image_ = SE::DECK;
    }
    if ( newImage == Cell::SE::EMPTY )
    {
        setPixmap( QPixmap("empty.bmp") );
        current_image_ = SE::EMPTY;
    }
    if ( newImage == Cell::SE::POINT )
    {
        setPixmap( QPixmap("point.bmp") );
        current_image_ = SE::POINT;
    }
    if ( newImage == Cell::SE::WOUNDED_DECK)
    {
        setPixmap( QPixmap("wounded_deck.bmp") );
        current_image_ = SE::WOUNDED_DECK;
    }
}

Cell::SE Cell::GetImage() const
{
    return current_image_;
}

void Cell::PutOnAbstractField(AbstractField *parent, const int& x, const int& y)
{
    setParent(parent);
    SetCoordinate(x, y);
    SetImage(SE::EMPTY);
    current_state_ = SE::EMPTY;
}

void Cell::SetState(const Cell::SE &newState)
{
    current_state_ = newState;
}

Cell::SE Cell::GetState() const
{
    return current_state_;
}

int Cell::X() const
{
    return x_;
}

int Cell::Y() const
{
    return y_;
}

QPoint Cell::GetCoord() const
{
    return QPoint(x_, y_);
}

