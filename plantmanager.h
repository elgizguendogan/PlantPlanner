#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include <QObject>

class PlantManager : public QObject
{
    Q_OBJECT
public:
    explicit PlantManager(QObject *parent = nullptr);

signals:

};

#endif // PLANTMANAGER_H
