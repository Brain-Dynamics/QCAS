# QCAS [![Build Status](https://travis-ci.org/Brain-Dynamics/QCAS.svg?branch=master)](https://travis-ci.org/Brain-Dynamics/QCAS) [![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/Brain-Dynamics/qcas/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
A Qt library for getting tickets from a Central Authentication Service (CAS Server)

This library allows authentication against a CAS Server and retrieve a ticket for a service. For that, you only have to do a call to a single method with the URL of the CAS server with the correct path, the user's credentials and the login URL of the service that you want to access.

## Requirements

To use this package you only need the following dependencies.

* Qt (4.8.6)
* CMAKE (2.8)

And for the coverage you are going to need.

* lcov >= 1.11


You also need to have enable the REST Api of the CAS server to access the service. In the middle of this [link](http://jasig.github.io/cas/4.0.x/protocol/REST-Protocol.html) you can find cas server configuration to enable it.

## Compiling and installing the project

For compile the project you only need to setup the cmake configuration and make it.

Some possible cmake options:

  - `-DCMAKE_BUILD_TYPE=DEBUG`: Enables some debug output (other than making easier to debug the code)
  - `-DBUILD_DOCUMENTATION=ON`: Builds the doxygen documentation     
  - `-DBUILD_TESTS=ON`: Builds the unit tests and coverage (only in Debug Build), you need the following options to build the tests
	  - `-DTEST_DATA_CAS_URL=http://www.example.com/cas/v1/tickets`: A url of a CAS server to launch the tests (non ssl)
	  - `-DTEST_DATA_SERVICE_URL=http://www.example.com/service`: A url of a service to launch the tests (non ssl)
	  - `-DTEST_DATA_USERNAME=username`: The username of a test user
	  - `-DTEST_DATA_PASSWORD=password`: The password of the test user
  - `-DCMAKE_INSTALL_PREFIX=/custom/path/`: Install QCAS in a custom directory

For compile and install you can follow the nexts steps.

~~~shell
$ mkdir QCAS-build
$ cd QCAS-build
$ cmake ../QCAS -DCMAKE_INSTALL_PREFIX=_preferred_path_ ..
$ make
$ make install
~~~
If you want to compile the test and coverage targets, you have to setup the project with the Debug build type and add the test data variables in cmake. After that, you can execute the tests with the following command.

~~~shell
$ make
$ make test
~~~
And for the coverage with this other (you can see the results in the coverage folder in the QCAS-build directory).

~~~shell
$ make
$ make coverage
~~~
The coverage target ONLY works if the tests were passed

## How to use the QCAS project.

To use the library you need the following parameters.

* **casUrl:** This is the url of the CAS server endpoint ticket generator. You need to include the path to the REST API service.
* **username:** The username of the user that is going to authenticate.
* **password:** The password of the user.
* **serviceUrl:** The login url of the service that the user wants to access throught the CAS Server.
* **ignoreSslErrors (OPTIONAL):** This is a flag to ignore or not the SSL errors (like certificate error), its default value is set to false. If you want to ignore the errors due to SSL protocol you need to add this flag with the value of false.

The method returns a QString with the value of the ticket for the request service, or a CASException is there was a problem in the process.

~~~cpp
#include "QCAS/QCAS.h"
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Gets the arguments
    QStringList args = a.arguments();
    if(args.length() < 5) {
        qDebug() << "Not Enought parameters. Usage: ./example https://www.example.com/cas/v1/tickets user password https://www.example.com/service ";
        return 1;
    }

    // Parsing the arguments
    QString casUrl = args.at(1);
    QString username = args.at(2);
    QString password = args.at(3);
    QString serviceUrl = args.at(4);

    try{
        // Getting the ticket
        QString ticket = QCAS::CAS(casUrl,username,password,serviceUrl,true);
        qDebug() << "Ticket for the service " + serviceUrl + ": " + ticket;
    } catch(CASException &exception) {
        // If there's an error, show it!
        qWarning() << exception.what();
    }

    return 0;
}
~~~

## TODO List

- [ ] There's a problem causing the algorithm to block with ssl protocols in windows and linux


### Development by
![Brain Dynamics](http://www.brain-dynamics.es/images/bdync.png)
