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
 * @brief The QCAS class manages all the operations for retrive a
 * tickect access to a service.
 */
class QCAS {
public:
    /**
     * @brief This method manages all the operations for retrive a
     * tickect access to the request service. This return a string
     * with the value of the ticket to add to the service's url and
     * grant access to the service's resources.
     *
     * @param casUrl:           Is the cas server URL.
     * @param username:         Username of the user that wants to
     * get access to the service.
     * @param password:         Password of the user
     * @param serviceUrl:       Service's access that the user wants
     * to get access.
     * @param ignoneSslErrors:  Ignore the errors due to the ssl protocol.
     *
     * @throw CASException:     The method throws this exception if there
     * was and error.
     *
     * @return The access ticket for the request service as QString.
     */
    static QString CAS(const QString& casUrl,
                       const QString& username,
                       const QString& password,
                       const QString& serviceUrl,
                       const bool ignoneSslErrors = false);
};



/**
 * @brief The CASException class is thrown when an error happens
 * during the ticket getting process
 */
class CASException: public std::runtime_error {
public:
    /**
     * @brief CASException: Main constructor of the exception
     * @param message: is the exception message.
     */
    CASException(const QString& message):
        std::runtime_error(message.toStdString()) {}
};

#endif // QCAS_H
