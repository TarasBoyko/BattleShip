#ifndef SHIP_H
#define SHIP_H

#include <QLabel>
#include <QVector>
#include <QMouseEvent>

using std::vector;

class AbstractField;

class Ship : public QLabel
{
public:
    enum class DO { HORIZONTAL, VERTICAL };
    Ship(QWidget* parent, const int& number_decks, const DO &orientation, const QPoint &first_deck_coordinate, AbstractField *new_self_field, AbstractField *new_alien_field);
    bool IsSituatedOnAbstractField() const;

    int GetNumberDecks() const;
    DO GetOrientation() const;
private:
    DO orientation_;
    const int number_decks_;
    QPoint delta_; // mouse press coordinates relative to upper left corner of ship
    QPoint old_pos_; // position after mouse press
    AbstractField* const self_field_;
    AbstractField* const alien_field_;
    bool is_situated_on_abstract_field_;

    void SetSituatedOnAbstractField(const bool& is_situated);
    void ChangeOrientation();
    DO GetOppositeOrientation() const;
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
};

#endif // SHIP_H
