#ifndef MASK_H
#define MASK_H

#include "exception.h"
#include <QString>
#include <QVector>

class Mask { //Класс для обработки масок
public:
    Mask(QString name, QString mask);

private:
    QString const name;      //Имя для замены
    QString fullMask;        //Полная маска файла
    QString totalName;       //Итоговое имя
    QVector<uint> stepValue; //Массив, хранящий значение шагов для шаблона [C]
    QVector<uint> beginValue; //Массив, хранящий значения чисел для подстановки
                              //для шаблона [C]
    void readMask(QString& mask); //функция прочтения отдельных масок
    void maskN(QString& mask);
    void maskE(QString& mask);
    void maskYMD(QString& mask);
    void maskHMS(QString& mask);
    void maskC(QString& mask);
    void checkBracketBalance();

public:
    void readName(); //Основныя функция прочтения имени
    QString getTotalName() const; //Возвращает итоговое имя
    uint Count_C() const; //Возвращает число шаблонов [C] в общем шаблоне
    QString area() const;
    uint getValue_C(uint index);
};

#endif // MASK_H
