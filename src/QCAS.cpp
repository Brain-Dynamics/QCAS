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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

static void checkError(QNetworkReply *response) {
    if(response->error() != QNetworkReply::NoError ) {
        QString error = response->errorString();
        delete response;
        throw CASException(error);
    }
}


QString QCAS::CAS(const QString &casUrl, const QString &username, const QString &password, const QString &serviceUrl) {

    // Create variables
    QUrl postData;
    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkReply *response = nullptr;

    // Create the login ticket request
    QNetworkRequest requestCas;
    requestCas.setUrl(QUrl(casUrl));
    requestCas.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    postData.clear();
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);
    response = nam.post(requestCas,postData.encodedQuery());
    response->ignoreSslErrors();


    loop.connect(response,SIGNAL(finished()),SLOT(quit()));
    loop.exec();

    checkError(response);

    QString responseMessage = response->readAll();
    QString casAccessURL = QString(responseMessage).section("action=\"",1).section("\"",0,0);
    delete response;

    if(casAccessURL.isEmpty())
        throw CASException("Error getting the ticket: " + responseMessage);

    QNetworkRequest requestService;
    requestService.setUrl(QUrl(QUrl(casAccessURL)));
    requestService.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    postData.clear();
    postData.addQueryItem("service", serviceUrl);
    response = nam.post(requestService,postData.encodedQuery());
    response->ignoreSslErrors();

    loop.connect(response,SIGNAL(finished()),SLOT(quit()));
    loop.exec();

    checkError(response);

    QString serviceAnswer = response->readAll();
    delete response;

    return serviceAnswer;
}



//QCAS::CASException::CASException(const QString& message): m_message(message) {
//}

//QString QCAS::CASException::message() const {
//    return m_message;
//}
