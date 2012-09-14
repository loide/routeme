#include "routemecontroller.h"
#include "routemeserver.h"

#include <QtCore/QThread>
#include <qcontactmanager.h>
#include <qcontactphonenumber.h>
#include <qmessageservice.h>

using namespace QtMobility;

RouteMeController::RouteMeController(QObject *parent)
    : QObject(parent)
    , m_server(new RouteMeServer(this))
{
}

RouteMeController::~RouteMeController()
{
}

void RouteMeController::init()
{
    m_server->listen();
}

void RouteMeController::sendMyCurrentLocation(const QString &number, const QString &message)
{
    QMessageService *service = new QMessageService(this);

    QMessage location;
    location.setType(QMessage::Sms);
    location.setPriority(QMessage::NormalPriority);
    location.setSubject(QString("RouteMe"));
    location.setBody(message);
    location.setTo(QMessageAddress(QMessageAddress::Phone, number));

    service->send(location);
}

bool RouteMeController::isContactsAvailable()
{
    QContactManager *contactManager = new QContactManager(this);

    QList<QContactLocalId> contactLocalId = contactManager->contactIds();
    QList<QContactLocalId>::iterator it;

    for (it = contactLocalId.begin(); it != contactLocalId.end(); ++it) {
        QContact contact = contactManager->contact(*it);
        QContactPhoneNumber contactNumber = contact.detail<QContactPhoneNumber>();

        if (contactNumber.number() != "") {
            return true;
        }
    }

    return false;
}
