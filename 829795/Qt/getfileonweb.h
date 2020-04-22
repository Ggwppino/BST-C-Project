#ifndef GETFILEONWEB_H
#define GETFILEONWEB_H


#include <QtWidgets>
#include <QtNetwork>
#include <QUrl>


QT_BEGIN_NAMESPACE
class QFile;
class QNetworkReply;

QT_END_NAMESPACE

class GetFileOnWeb : public QObject{
    Q_OBJECT

public:
    explicit GetFileOnWeb(QString url);



    void startRequest(const QUrl &requestedUrl);

    QString get_filePathName();
    void downloadFile();

signals:
    void getReadys(QString content);
    //void giveLink(QString link);

private slots:
    //void cancelDownload();
    void httpReadyRead();
    void httpFinished();
    //void processLink();


private:

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QString defaultUrl1;
    QString content;
    QFile *file;
    bool httpRequestAborted;
    bool readys;
};

#endif // GETFILEONWEB_H
