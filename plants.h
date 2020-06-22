#ifndef PLANTS_H
#define PLANTS_H

#include <QObject>

class Plants : public QObject
{
    Q_OBJECT
public:
    explicit Plants(QObject *parent = nullptr);

signals:

};

#endif // PLANTS_H
