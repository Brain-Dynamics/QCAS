/*
 * File:
 * Author: daniel.agredano
 * Email: daniel.agredano at brain-dynamics.es
 *
 * Created on 23/03/2015
 *
 *
 */

#include "QCAS/QCAS.h"
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QStringList args = a.arguments();

    if(args.length() < 5) {
        qDebug() << "Not Enought parameters. Usage: ./example https://www.example.com/cas/v1/tickets user password https://www.example.com/service ";
        return 1;
    }

    QString casUrl = args.at(1);
    QString username = args.at(2);
    QString password = args.at(3);
    QString serviceUrl = args.at(4);

    try{
        qDebug() << QCAS::CAS(casUrl,username,password,serviceUrl);
    } catch(CASException &exception) {
        qWarning() << exception.what();
    }

    return 0;
}