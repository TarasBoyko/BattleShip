#ifndef CELL_H
#define CELL_H

#include <QLabel>

class AbstractField;

class Cell : public QLabel
{
public:
    enum class SE { EMPTY, DECK, WOUNDED_DECK, POINT };
    void SetCoordinate(const int& x, const int& y);
    void SetImage(const SE& newImage);
    SE GetImage() const;
    void PutOnAbstractField(AbstractField *parent, const int &x, const int &y);
    void SetState(const SE& newState);
    SE GetState() const;
    int X() const;
    int Y() const;
    QPoint GetCoord() const;

private:
    SE current_state_;
    SE current_image_;
    int x_;
    int y_;
};

#endif // CELL_H
