#include <QMimeData>
#include <QDebug>
#include <QApplication>
#include "ship.h"
#include "abstractfield.h"
#include "usersfield.h"

Ship::Ship(QWidget* parent, const int& number_decks, const DO& orientation,
           const QPoint& first_deck_coordinate, AbstractField* new_self_field, AbstractField* new_alien_field) :
    QLabel(parent),
    number_decks_(number_decks),
    self_field_(new_self_field),
    alien_field_(new_alien_field)
{
    int i;
    int shipsLength = 0;
    for ( i = 0; i < number_decks - 1; i++ )
    {
        shipsLength += kSquareLength + kLineWidth;
    }
    shipsLength += kSquareLength;

    orientation_ = orientation;
    if ( orientation == DO::HORIZONTAL )
    {
        setGeometry(first_deck_coordinate.x(), first_deck_coordinate.y(), shipsLength, kSquareLength);
    }
    else
    {
        setGeometry(first_deck_coordinate.x() + kLineWidth, first_deck_coordinate.y(), kSquareLength, shipsLength);
    }
    setStyleSheet("background-color: rgb(0, 170, 0);");
    is_situated_on_abstract_field_ = self_field_->CanPutShip(first_deck_coordinate, number_decks, orientation_);
    show();
}

bool Ship::IsSituatedOnAbstractField() const
{
    return is_situated_on_abstract_field_;
}

void Ship::SetSituatedOnAbstractField(const bool &is_situated)
{
    is_situated_on_abstract_field_ = is_situated;
}

int Ship::GetNumberDecks() const
{
    return number_decks_;
}

Ship::DO Ship::GetOrientation() const
{
    return orientation_;
}

void Ship::ChangeOrientation()
{
    orientation_ = GetOppositeOrientation();
    setGeometry(x(), y(), height(), width());
}

Ship::DO Ship::GetOppositeOrientation() const
{
    if ( orientation_ == Ship::DO::HORIZONTAL )
    {
        return Ship::DO::VERTICAL;
    }
    else
    {
        return Ship::DO::HORIZONTAL;
    }
}

void Ship::mousePressEvent(QMouseEvent *e)
{
    QPoint coord = self_field_->ShipToCellCoordinate(*this);
    delta_ = e->pos();
    old_pos_ = pos();
    if ( is_situated_on_abstract_field_ )
    {
        self_field_->TakeOffShipFromCells(*this, coord.x(), coord.y() );
    }
}

void Ship::mouseMoveEvent(QMouseEvent *e)
{
    if ( e->buttons() != Qt::LeftButton )
    {
        return;
    }
    move( pos() + e->pos() - delta_ );
}

void Ship::mouseReleaseEvent(QMouseEvent *e)
{
    // coordinates of left upper deck of ship
    QPoint coord = self_field_->ShipToCellCoordinate(*this);
    // if user released ship by right mouse button
    if ( e->button() == Qt::RightButton )
    {
        // change orientation, if it is possible
        if ( self_field_->CanPutShip(coord, number_decks_, GetOppositeOrientation()) )
        {
            ChangeOrientation();
            self_field_->SetShip(*this, coord.x(), coord.y() );
        }
        else
        {
            QRect RectAfterReorientation = QRect( x(), y(), height(), width() );
            // if ship will intersects some AbstractField will goes beyond form
            if ( RectAfterReorientation.intersects(self_field_->geometry())
                 ||
                 RectAfterReorientation.intersects(alien_field_->geometry())
                 ||
                 !parentWidget()->rect().contains(RectAfterReorientation) )
            {
                move(old_pos_);
                if ( is_situated_on_abstract_field_)
                {
                    self_field_->SetShip( *this, self_field_->ShipToCellCoordinate(*this).x(), self_field_->ShipToCellCoordinate(*this).y() );
                }
                QApplication::beep();
            }
            else
            {
                ChangeOrientation();
            }
        }
    }
    // if user released ship by left mouse button
    else
    {
        // set ship, if it is possible

        if ( self_field_->CanPutShip( coord , number_decks_, orientation_ ) )
        {
            self_field_->SetShip(*this, coord.x(), coord.y() );
            SetSituatedOnAbstractField(true);
        }
        else
        {
            // if ship intersects some AbstractField or goes beyond form
            if ( geometry().intersects(self_field_->geometry())
                 ||
                 geometry().intersects(alien_field_->geometry())
                 ||
                 !parentWidget()->rect().contains(geometry()) )
            {
                move(old_pos_);
                if ( is_situated_on_abstract_field_)
                {
                    self_field_->SetShip( *this, self_field_->ShipToCellCoordinate(*this).x(), self_field_->ShipToCellCoordinate(*this).y() );
                }

                QApplication::beep();
            }
            else
            {
                SetSituatedOnAbstractField(false);
            }
        }
    }
}






