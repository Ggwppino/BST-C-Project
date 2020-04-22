#include "artistiwindow.h"
#include "fillthread.h"
#include "ui_artistiwindow.h"

#include <QtCharts>
#include <QtDebug>

using namespace  QtCharts;

const char defaultUrl1[]= "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt";
const char defaultUrl2[]= "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt";

ArtistiWindow::ArtistiWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ArtistiWindow)
    , universalUrl("Error")
    , emiUrl("Error")
    , universalThread(nullptr)
    , emiThread(nullptr)
    , readys(false)
{

    ui->setupUi(this);
}

void ArtistiWindow::starts(){

    GetFileOnWeb *universal= new GetFileOnWeb(defaultUrl1);
    GetFileOnWeb *emi= new GetFileOnWeb(defaultUrl2);
    universal->downloadFile();
    emi->downloadFile();
    universalThread = new FillThread(this);
    emiThread = new FillThread(this);

    connect(universal, SIGNAL(getReadys(QString)), this, SLOT(startThreadUniversal(QString)));
    connect(emi, SIGNAL(getReadys(QString)), this, SLOT(startThreadEmi(QString)));
    connect(universalThread, SIGNAL(giveLink(QString, QChar)), this, SLOT(print_artisti_universal(QString, QChar)));
    connect(emiThread, SIGNAL(giveLink(QString, QChar)), this, SLOT(print_artisti_emi(QString, QChar)));
    connect(universalThread, SIGNAL(finished()), this, SLOT(check_all_file_read()));
    connect(emiThread, SIGNAL(finished()), this, SLOT(check_all_file_read()));
    connect(this, &ArtistiWindow::allRead, this, &ArtistiWindow::create_chart_artisti);
}


void ArtistiWindow::startThreadUniversal(QString content){
    universalThread->setContent(content);
    universalThread->start();
}

void ArtistiWindow::startThreadEmi(QString content){
    emiThread->setContent(content);
    emiThread->start();
}


void ArtistiWindow::check_all_file_read(){
    if(readys){
        emit allRead();
    }else readys = true;
}

void ArtistiWindow::create_chart_artisti(){

    std::sort(universalCount.begin(),universalCount.end());
    QBarSeries *universalSeries = new QBarSeries();
    QBarSet *setUniversal;
    int max=0;

    for(CounterIniziale bar: universalCount){
        setUniversal = new QBarSet(QString(bar.iniziale).append(" : ").append(QString::number(bar.counter)));
        *setUniversal << bar.counter;
        if(max< bar.counter) max = bar.counter;
        universalSeries->append(setUniversal);
    }
    QChart *chartUniversal = new QChart();
    chartUniversal->setTitle("Numero di artisti per lettera");
    chartUniversal->addSeries(universalSeries);
    chartUniversal->setAnimationOptions(QChart::AllAnimations);
    QBarCategoryAxis * universalAxisX = new QBarCategoryAxis();
    universalAxisX->append("Capolettera");
    chartUniversal->addAxis(universalAxisX, Qt::AlignBottom);
    universalSeries->attachAxis(universalAxisX);
    QValueAxis * universalAxisY = new QValueAxis();
    universalAxisY->setRange(0, max);
    chartUniversal->addAxis(universalAxisY, Qt::AlignLeft);
    universalSeries->attachAxis(universalAxisY);
    chartUniversal->legend()->setVisible(true);
    chartUniversal->legend()->setAlignment(Qt::AlignLeft);
    QChartView * chartUniversalView = new QChartView(chartUniversal);
    chartUniversalView->setRenderHint(QPainter::Antialiasing);
    ui->universalLoading->deleteLater();
    ui->universalGraph->addWidget(chartUniversalView);




    std::sort(emiCount.begin(),emiCount.end());
    QBarSeries *emiSeries = new QBarSeries();
    QBarSet *setEmi;
    max =0;
    for(CounterIniziale bar2: emiCount){
        setEmi = new QBarSet(QString(bar2.iniziale).append(" : ").append(QString::number(bar2.counter)));
        *setEmi << bar2.counter;
        if(max<bar2.counter) max=bar2.counter;
        emiSeries->append(setEmi);
    }
    QChart *chartEmi = new QChart();
    chartEmi->setTitle("Numero di artisti per lettera");
    chartEmi->addSeries(emiSeries);
    chartEmi->setAnimationOptions(QChart::AllAnimations);
    QBarCategoryAxis *emiAxisX = new QBarCategoryAxis();
    emiAxisX->append("Capolettera");
    chartEmi->addAxis(emiAxisX, Qt::AlignBottom);
    emiSeries->attachAxis(emiAxisX);
    QValueAxis *emiAxisY = new QValueAxis();
    emiAxisY->setRange(0,max);
    chartEmi->addAxis(emiAxisY, Qt::AlignLeft);
    emiSeries->attachAxis(emiAxisY);
    chartEmi->legend()->setVisible(true);
    chartEmi->legend()->setAlignment(Qt::AlignLeft);
    QChartView * chartEmiView = new QChartView(chartEmi);
    ui->emiLoading->deleteLater();
    ui->emiGraph->addWidget(chartEmiView);




    QBarSet *setUniversalConfronto = new QBarSet("Universal");
    QBarSet *setEmiConfronto = new QBarSet("Emi");
    *setUniversalConfronto << ui->listWidget1->count();
    *setEmiConfronto << ui->listWidget2->count();
    max = ui->listWidget1->count()>ui->listWidget2->count()? ui->listWidget1->count() : ui->listWidget2->count();
    QBarSeries *series = new QBarSeries();
    series->setName("Numero artisti");
    series->append(setUniversalConfronto);
    series->append(setEmiConfronto);
    series->setLabelsVisible(true);
    QChart *chart = new QChart();
    chart->setTitle("Numero di artisti per ciascuna etichetta");
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::AllAnimations);
    QBarCategoryAxis *confrontoAxisX = new QBarCategoryAxis();
    confrontoAxisX->append("Etichette");
    chart->addAxis(confrontoAxisX, Qt::AlignBottom);
    series->attachAxis(confrontoAxisX);
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView * chartView = new QChartView(chart);
    ui->confrontoLoading->deleteLater();
    ui->confrontoGraph->addWidget(chartView);
}

void ArtistiWindow::print_artisti_universal(QString in, QChar firstLetter){

    QListWidgetItem *item;
    QLabel * myLabel;
    bool opened = in!="Error";

    if (opened)
    {
        int index = universalCount.indexOf(CounterIniziale(firstLetter));
        if(index !=-1){
            universalCount[index].counter++;
        }else{
            universalCount.append(CounterIniziale(firstLetter, 1));
        }
        item = new QListWidgetItem("");
        ui->listWidget1->addItem(item);
        myLabel = new QLabel(in);
        myLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        myLabel->setOpenExternalLinks(true);
        ui->listWidget1->setItemWidget(item, myLabel);

    } else {
        item = new QListWidgetItem("Errore");
        ui->listWidget1->addItem(item);
    }
}

void ArtistiWindow::print_artisti_emi(QString in, QChar firstLetter){

    QListWidgetItem *item;
    QLabel * myLabel;
    bool opened = in!="Error";

    if (opened)
    {
        int index = emiCount.indexOf(CounterIniziale(firstLetter));
        if(index !=-1){
            emiCount[index].counter++;
        }else{
            emiCount.append(CounterIniziale(firstLetter, 1));
        }
        item = new QListWidgetItem("");
        ui->listWidget2->addItem(item);
        myLabel = new QLabel(in);
        myLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        myLabel->setOpenExternalLinks(true);
        ui->listWidget2->setItemWidget(item, myLabel);

    } else {
        item = new QListWidgetItem("Errore");
        ui->listWidget2->addItem(item);
    }
}

ArtistiWindow::~ArtistiWindow()
{
    universalUrl = nullptr;
    emiUrl = nullptr;
    emiThread->quit();
    emiThread->wait();
    universalThread->quit();
    universalThread->wait();
    delete ui;
}

