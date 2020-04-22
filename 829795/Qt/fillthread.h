#ifndef FILLTHREAD_H
#define FILLTHREAD_H

#include <QThread>
#include <QChar>
#include <QRegularExpression>

class FillThread : public QThread
{
    Q_OBJECT
public:
    explicit FillThread(QObject *parent = nullptr);
    void run();
    void setContent(QString content);

signals:
    void giveLink(QString link, QChar iniziale);
    void statusOk();

private slots:

    void processLink();

private:
    QString content;

};

#endif // FILLTHREAD_H
