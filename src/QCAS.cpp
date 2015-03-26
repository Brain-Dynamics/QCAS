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
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QEventLoop>
#include <QHostInfo>
#include <QRegExp>
#include <QString>
#include <QUrl>


/**
 * @brief Checks if there's any errors in the reply. If it's the case, throws an
 * exception
 * @param response is the QNetworkReply to check
 */
static void checkError(QNetworkReply *response) {
    if(response->error() != QNetworkReply::NoError ) {
        QString error = response->errorString();
        delete response;
        throw CASException(error);
    }
}



// ----------------------------------------------------------------------------------- //
QString QCAS::CAS(const QString &casUrl,
                  const QString &username,
                  const QString &password,
                  const QString &serviceUrl,
                  const bool ignoneSslErrors) {

    // Create variables
    QUrl postData;
    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkReply *response = nullptr;

    // Create the login ticket request
    QNetworkRequest requestCas;
    requestCas.setUrl(QUrl(casUrl));
    requestCas.setHeader(QNetworkRequest::ContentTypeHeader,
                         "application/x-www-form-urlencoded");
    // Prepare the post data
    postData.clear();
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);
    // Send the request
    response = nam.post(requestCas,postData.encodedQuery());
    if(ignoneSslErrors) response->ignoreSslErrors();

    // Waiting for the reply...
    loop.connect(response,SIGNAL(finished()),SLOT(quit()));
    loop.exec();

    // Checks for errors
    checkError(response);

    // Get the message and the token
    QString responseMessage = response->readAll();
    QString casAccessURL = QString(responseMessage).section("action=\"",1).section("\"",0,0);
    delete response;

    // If is empty, launch an exception
    if(casAccessURL.isEmpty())
        throw CASException("Error getting the ticket: " + responseMessage);

    // Create the login service ticket request
    QNetworkRequest requestService;
    requestService.setUrl(QUrl(QUrl(casAccessURL)));
    requestService.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/x-www-form-urlencoded");
    // Prepare the post data
    postData.clear();
    postData.addQueryItem("service", serviceUrl);
    // Send the request
    response = nam.post(requestService,postData.encodedQuery());
    if(ignoneSslErrors) response->ignoreSslErrors();

    // Waiting for the reply...
    loop.connect(response,SIGNAL(finished()),SLOT(quit()));
    loop.exec();

    // Checks for errors
    checkError(response);

    // Get the ticket
    QString serviceAnswer = response->readAll();
    delete response;

    return serviceAnswer;
}
