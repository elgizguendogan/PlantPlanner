#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(PlantManager *plantManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plantManager(plantManager)
{
    ui->setupUi(this);
    this->setFixedSize(320,240);
    setWindowTitle(tr("Plant Planner"));
    //Create the scene and the gui, get the data and create the graphics view
    //Creating a timer for the weather function
    m_scene = new QGraphicsScene(this);
    //m_scene->setSceneRect(-60,-54,120,108);
    ui->weatherView->setScene(m_scene);
    ui->weatherView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->weatherView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    startTimer(1000);
    setupWeather();
    setupWidgets();
    setupSignals();

    this->setStyleSheet("QMainWindow#MainWindow{background-image: url(:/images/backgroundImage.png);background-repeat: no-repeat;} ");
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(ui->stackedWidget->currentWidget() == ui->mainPage) {
        ui->time->setText(QDate::currentDate().toString("dd.MM.yyyy") + "\n" + QTime::currentTime().toString("hh:mm:ss"));
    }
}

void MainWindow::setupSignals()
{
    connect(ui->managePlantsButton,&QPushButton::clicked, this, &MainWindow::goToPlantManagement);
    connect(ui->backMainButton, &QPushButton::clicked, this, &MainWindow::backToMainPage);
    connect(ui->addPlantButton, &QPushButton::clicked, this, &MainWindow::addPlant);
    connect(ui->removePlantButton, &QPushButton::clicked, this, &MainWindow::removePlant);
    connect(ui->editPlantButton, &QPushButton::clicked, this, &MainWindow::editPlant);
    connect(ui->backMainButton, &QPushButton::clicked, this, &MainWindow::backToMainPage);
    connect(ui->savePlantButton, &QPushButton::clicked, this, &MainWindow::saveChanges);
    connect(ui->cancelPlantButton, &QPushButton::clicked, this, &MainWindow::cancelChanges);
}

void MainWindow::setupWidgets()
{
    
    //Setup main table
    initializePlantTable();

    //Setup days to water list
    //ui->dayList->setSelectionModel()
    QStringList days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for ( int i=0; i < 7;++i) {
        QListWidgetItem* newItem = new QListWidgetItem(days[i], ui->dayList);
        newItem->setFlags(newItem->flags () | Qt::ItemIsUserCheckable);
        newItem->setCheckState(Qt::Unchecked);
        ui->dayList->addItem(newItem);
    }
    //Setup combo box
    ui->cupsNumber->addItem(tr("-Select-"));
    for ( int i=1; i < 21; ++i) {
        ui->cupsNumber->addItem(QString::number(i) + " cups");
    }

    ui->editPlantButton->setDisabled(true);
    ui->removePlantButton->setDisabled(true);

}

void MainWindow::setupWeather()
{
    Weather *weatherManager = new Weather;
    weatherManager->getLocationData();
    QSignalSpy spy(weatherManager,SIGNAL(iconIdReady()));
    while (spy.count() == 0) {
        spy.wait(3000);
    }
    if ( spy.count() >= 1) {
        QGraphicsTextItem *tempText = new QGraphicsTextItem;

        tempText->setFont(QFont("Geneva", 20));
        tempText->setPlainText(QString::number(weatherManager->getTemperature())+" °C");
        tempText->setPos(-30,-30);
        m_scene->addItem(tempText);

        QGraphicsTextItem *locationText = new QGraphicsTextItem;
        locationText->setFont(QFont("Geneva", 10));
        locationText->setPlainText(weatherManager->getCityName()+","+weatherManager->getCountryName());
        locationText->setPos(-30,30);
        m_scene->addItem(locationText);

        QGraphicsPixmapItem *icon = new QGraphicsPixmapItem;
        QPixmap icon2;
        icon2.load(":/weatherIcons/icons/"+weatherManager->getIconName()+".png");
        icon->setPixmap(icon2);
        //icon->setPixmap(weatherManager->getIcon());
        icon->setPos(30,-60);
        m_scene->addItem(icon);

        //QGraphicsTextItem *description = new QGraphicsTextItem;
        //description->setPlainText(weatherManager->getDescription());
        /*description->setPlainText("thunderstorms");
        description->setTextWidth(40);
        description->setFont(QFont("Geneva", 10));
        description->setPos(35,-20);
        m_scene->addItem(description);*/

    }
}

void MainWindow::resetEdit()
{
    ui->editName->setText(tr(""));
    for ( int i=0; i < 7;++i) {
       ui->dayList->item(i)->setCheckState(Qt::Unchecked);
     }
    ui->cupsNumber->setCurrentIndex(0);
}

void MainWindow::initializePlantTable()
{
    ui->plantTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->plantTable->verticalHeader()->setVisible(false);
    ui->plantTable->setRowCount(0);
    ui->plantTable->setColumnCount(2);
    ui->plantTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList tableHeader;
    tableHeader<<"Plants"<<"Cups";
    ui->plantTable->setHorizontalHeaderLabels(tableHeader);
}

void MainWindow::updatePlantTable()
{
    QDate dateToday = QDate::currentDate();
    Plants::PlantsList plantsToWater = m_plantManager->whichPlantsToWater(dateToday.dayOfWeek());
    ui->plantTable->setRowCount(0);

    if ( !plantsToWater.empty()) {
        int rowSize = plantsToWater.count();

        ui->plantTable->setRowCount(rowSize);
        ui->plantTable->setColumnCount(2);

        for ( int i = 0; i < rowSize ; ++i ) {
            if ( !m_plantManager->getEntryName(plantsToWater[i]).isEmpty() && m_plantManager->cupNumber(plantsToWater[i]) != -1) {
                QTableWidgetItem *itemName = new QTableWidgetItem(m_plantManager->getEntryName(plantsToWater[i]),0);
                ui->plantTable->setItem(i,0, itemName);

                QTableWidgetItem *itemCups = new QTableWidgetItem(QString::number(m_plantManager->cupNumber(plantsToWater[i])),0);
                ui->plantTable->setItem(i,1, itemCups);
             }
        }
    }
}


void MainWindow::goToPlantManagement()
{
    ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
}

void MainWindow::addPlant()
{
    //Reset the plant editing screen
    resetEdit();
    ui->stackedWidget->setCurrentWidget(ui->plantEditPage);
}

void MainWindow::removePlant()
{
    auto item = ui->plantList->currentItem();
    auto entry = m_entryMap.value(item);

    if ( item && entry) {
        if ( m_plantManager->removeEntry(entry) ) {
            m_entryMap.remove(item);
            delete item;
        }
    }
    if ( m_entryMap.isEmpty()) {
        ui->editPlantButton->setDisabled(true);
        ui->removePlantButton->setDisabled(true);
    }
    updatePlantTable();
}

void MainWindow::editPlant()
{
     auto item = ui->plantList->currentItem();
     auto entry = m_entryMap.value(item);
     // Fill the form with the information of the selected plant
     if ( item && entry) {
         ui->editName->setText(entry->name());
         for ( int i=0; i < 7;++i) {
            if (entry->isWaterDaySet(i)) {
                ui->dayList->item(i)->setCheckState(Qt::Checked);
            }
            else {
                ui->dayList->item(i)->setCheckState(Qt::Unchecked);
            }
         }
         ui->cupsNumber->setCurrentIndex(entry->cups());
     }

     ui->stackedWidget->setCurrentWidget(ui->plantEditPage);
}

void MainWindow::backToMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::cancelChanges()
{
    resetEdit();
    ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
}

void MainWindow::saveChanges()
{
    bool isInfoComplete = true;
    QMessageBox msgBox;
    PlantEntry *newEntry = new PlantEntry(this);

    // Set the Name
    if ( !ui->editName->text().isEmpty() ) {
        newEntry->setName(ui->editName->text());
    }
    else {
        //Give Warning
        msgBox.setText(tr("Please give your plant a name"));
        msgBox.exec();
        isInfoComplete = false;
    }
    // Set the Watering Days
    QBitArray water_days(7);
    for ( int i=0; i < 7;++i) {
       if(ui->dayList->item(i)->checkState()) {
           water_days[i] = true;
       }
       else {
           water_days[i] = false;
       }
    }
    newEntry->setWaterDays(water_days);

    //Set the cups number
    if (ui->cupsNumber->currentIndex() != 0) {
        newEntry->setCups(ui->cupsNumber->currentIndex());
    }
    else {
        //Give Warning
        msgBox.setText(tr("Please select a cup number"));
        msgBox.exec();
        isInfoComplete = false;
    }


    if(isInfoComplete) {
        QList<QListWidgetItem *> items = ui->plantList->findItems(newEntry->name(),Qt::MatchExactly);
        if ( items.count() > 0 ){
           // There is already an element update it
            m_entryMap[items[0]] = newEntry;
            m_plantManager->updateEntry(newEntry);
        }
        else {
            m_plantManager->addEntry(newEntry);
            ui->plantList->addItem( newEntry->name() );
            auto item = ui->plantList->item( ui->plantList->count() - 1 );
            m_entryMap.insert(item, newEntry);
        }

        updatePlantTable();
        ui->editPlantButton->setDisabled(false);
        ui->removePlantButton->setDisabled(false);
        ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
        resetEdit();
    }
}


