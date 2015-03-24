/*
 * File:
 * Author: daniel.agredano
 * Email: daniel.agredano at brain-dynamics.es
 *
 * Created on 23/03/2015
 *
 *
 */

#ifndef QCAS_H
#define QCAS_H

#include <QString>
#include <stdlib.h>

/**
 * @brief The QCAS class
 */
class QCAS {
public:
    /**
     * @brief QCAS
     * @param casUrl
     * @param username
     * @param password
     * @param serviceUrl
     * @return
     */
    static QString CAS(const QString& casUrl, const QString& username, const QString& password, const QString& serviceUrl);
};


/**
 * @brief The CASException class
 */
class CASException: public std::runtime_error {
public:
    /**
     * @brief CASException
     * @param message
     */
    CASException(const QString& message): std::runtime_error(message.toStdString()) {}
};

#endif // QCAS_H
