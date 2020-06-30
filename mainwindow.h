#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDate>
#include <QCalendar>
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QSignalSpy>
#include <QPixmap>
#include <QFile>
#include <QtXml>
#include <QXmlStreamWriter>

#include "plantmanager.h"
#include "weather.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PlantManager *plantManager, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);
private:
    Ui::MainWindow *ui;
    PlantManager *m_plantManager;
    QGraphicsScene *m_scene;
    QFile m_dataFile;
    QDomDocument m_document;


    void setupSignals();
    void setupWidgets();
    void setupWeather();
    void resetEdit();
    void initializePlantTable();
    void updatePlantTable();
    void readFromXml();
    void writeToXml();

    QHash<QListWidgetItem*, PlantEntry*> m_entryMap;

private slots:
    void goToPlantManagement();
    void addPlant();
    void removePlant();
    void editPlant();
    void backToMainPage();
    void cancelChanges();
    void saveChanges();

};
#endif // MAINWINDOW_H
