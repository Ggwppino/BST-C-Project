#include "getfileonweb.h"




GetFileOnWeb::GetFileOnWeb(QString url)
    : reply(nullptr)
    , defaultUrl1(url)
    , file(nullptr)
    , httpRequestAborted(false)
    , readys(false)

{

}


void GetFileOnWeb::downloadFile(){
    QString urlSpec = defaultUrl1;


    const QUrl newUrl = urlSpec;

    startRequest(newUrl);
}

void GetFileOnWeb::startRequest(const QUrl &requestedUrl){
    url = requestedUrl;
    httpRequestAborted = false;

    reply = qnam.get(QNetworkRequest(url));

    QObject::connect(reply, &QNetworkReply::finished, this, &GetFileOnWeb::httpFinished);
    QObject::connect(reply, &QIODevice::readyRead, this, &GetFileOnWeb::httpReadyRead);

}

void GetFileOnWeb::httpFinished(){


    if (reply->error()) {
        content = "Error";
        reply->deleteLater();
        reply = nullptr;
        return;
    }


    reply->deleteLater();
    reply = nullptr;

    this->deleteLater();

    emit getReadys(content);
    //processLink();
}

void GetFileOnWeb::httpReadyRead()
{

    content = reply->readAll();
}

QString GetFileOnWeb::get_filePathName(){
    return content;
}



