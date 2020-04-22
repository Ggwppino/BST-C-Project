#include "fillthread.h"


FillThread::FillThread(QObject *parent) : QThread(parent), content("Errore")
{

}

void FillThread::run(){
    processLink();
}

void FillThread::setContent(QString content){
    this->content = content;
    emit statusOk();
}



void FillThread::processLink(){
    QStringList listString = content.split('\n');
    QString line;
    QString myString;
    QString country;
    if(content!="Errore"){
        for(QString line: listString){

            line.replace("&amp ", "&amp;");
            QStringList tmp = line.split(' ', QString::SkipEmptyParts);

            if(tmp.size()>=2){

                tmp[0].remove('\"');
                tmp[0].remove(' ');
                QRegularExpression rex("(?<=https://)(?<country>.*)(?=.wikipedia.org)");

                QRegularExpressionMatch match = rex.match(tmp[0]);


                if(match.hasMatch()){
                    country = match.captured("country");
                    tmp[0].remove(QString("https://%1.wikipedia.org").arg(country));
                }


                QString link= tmp[0] == nullptr ? "/404": tmp[0];
                QString name= tmp[1].replace('_', ' ').trimmed();
                QChar firstLetter = name[0].toUpper();

                if(match.hasMatch())
                    myString = QString("<a href=\"https://%1.wikipedia.org%2\">%3</a>")
                            .arg(country)
                            .arg(link)
                            .arg(name);

                else
                    myString = QString("<a href=\"https://en.wikipedia.org%1\">%2</a>")
                            .arg(link)
                            .arg(name);

                emit giveLink(myString, firstLetter);
            }
            this->msleep(10);
        }
    } else emit giveLink(myString, '\0');
}
