#ifndef ARTISTIWINDOW_H
#define ARTISTIWINDOW_H

#include "fillthread.h"
#include "getfileonweb.h"

#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class ArtistiWindow; }
QT_END_NAMESPACE

class ArtistiWindow : public QMainWindow
{
    Q_OBJECT
    struct CounterIniziale {
        QChar iniziale;
        int counter;
        CounterIniziale(QChar iniziale, int counter): iniziale(iniziale), counter(counter){}
        CounterIniziale(QChar iniziale): iniziale(iniziale), counter(0){}

        bool operator<(const CounterIniziale &rhs) const {
           return iniziale<rhs.iniziale ? true : false;
        }

        bool operator==(const CounterIniziale &rhs) const {
            return iniziale == rhs.iniziale;
        }

        bool operator!=(const CounterIniziale &rhs) const {
            return !(rhs == *this);
        }

        ~CounterIniziale(){
            iniziale = '\0';
            counter=0;
        }
    };

public:
    ArtistiWindow(QWidget *parent = nullptr);
    ~ArtistiWindow();
    void starts();

signals:
    void allRead();


private slots:

    void print_artisti_universal(QString content, QChar firstLetter);
    void print_artisti_emi(QString content, QChar firstLetter);
    void check_all_file_read();
    void create_chart_artisti();
    void startThreadUniversal(QString content);
    void startThreadEmi(QString content);

private:
    Ui::ArtistiWindow *ui;
    QString universalUrl;
    QString emiUrl;
    FillThread *universalThread;
    FillThread *emiThread;
    QList<CounterIniziale> universalCount;
    QList<CounterIniziale> emiCount;
    bool readys;
};
#endif // ARTISTIWINDOW_H
