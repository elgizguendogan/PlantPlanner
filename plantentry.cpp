#include "plantentry.h"

PlantEntry::PlantEntry(QObject *parent) : QObject(parent)
{

}

QString PlantEntry::name() const
{
    return m_name;
}

void PlantEntry::setName(const QString &name)
{
    if ( m_name != name) {
        m_name = name;
        emit nameChanged();
    }

}

QBitArray PlantEntry::waterDays() const
{
    return m_waterDays;
}

void PlantEntry::setWaterDays(const QBitArray &waterDays)
{
    if ( m_waterDays != waterDays ) {
         m_waterDays = waterDays;
         emit waterDaysChanged();
    }
}

bool PlantEntry::isWaterDaySet(const int index)
{
    Q_ASSERT_X(index < 8 && index >= 0,"index_out_of_bounds","Index is out of bond for the weekdays list");
    if ( index < 8 && index >= 0) {
        return m_waterDays[index];
    }
}

int PlantEntry::cups() const
{
    return m_cups;
}

void PlantEntry::setCups(int cups)
{
    m_cups = cups;
}

/*void PlantEntry::setOneWaterDay(const int index)
{
    if ( index < 8 && index >= 0) {
        m_waterDays[index] = true;
        emit oneWaterDaySet();
    }
}

void PlantEntry::unsetOneWaterDat(const int index)
{
    if ( index < 8 && index >= 0) {
        m_waterDays[index] = false;
        emit oneWaterDayUnset();
    }
}*/



