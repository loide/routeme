#include "routesearchmanager.h"
#include "serviceprovider.h"
#include "routecoordinateitem.h"
#include "routeplace.h"

RouteSearchManager::RouteSearchManager(QObject *parent)
    : QObject(parent)
    , m_searchManager(0)
    , m_coordinate(0)
    , m_place(0)
{
    init();
}

RouteSearchManager::~RouteSearchManager()
{
}

void RouteSearchManager::init()
{
    m_searchManager = ServiceProvider::instance()->searchManager();
}

RouteCoordinateItem* RouteSearchManager::coordinate()
{
    return m_coordinate;
}

void RouteSearchManager::setCoordinate(RouteCoordinateItem* coordinate)
{
    m_coordinate = coordinate;

    emit coordinateChanged();
}

void RouteSearchManager::searchPlace()
{
    if (!m_searchManager)
        return;

    if (!m_coordinate)
        return;

    QGeoCoordinate coord;
    coord.setLatitude(m_coordinate->latitude());
    coord.setLongitude(m_coordinate->longitude());

    QGeoSearchReply *reply = m_searchManager->reverseGeocode(coord);
    connect(reply, SIGNAL(finished()), this, SLOT(onReverseGeocodeFinished()));
}

void RouteSearchManager::onReverseGeocodeFinished()
{
    QGeoSearchReply *reply = qobject_cast<QGeoSearchReply *>(sender());

    QList<QGeoPlace> places;
    if (reply)
        places = reply->places();

    if (!places.isEmpty()) {
        m_place = new RoutePlace;
        m_place->setCoordinate(m_coordinate);
        m_place->setGeoPlace(places.at(0));
    }

    emit placeAvailable();
    reply->deleteLater();
}

RoutePlace* RouteSearchManager::place()
{
    return m_place;
}