#ifndef PLANTENTRY_H
#define PLANTENTRY_H

#include <QObject>

class PlantEntry : public QObject
{
    Q_OBJECT
public:
    explicit PlantEntry(QObject *parent = nullptr);

signals:

};

#endif // PLANTENTRY_H
