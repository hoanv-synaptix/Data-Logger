#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}

    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    // Called by Model::tick() whenever sensor values have changed.
    // Presenter overrides this to forward data to the View.
    virtual void onSensorDataUpdated(int pm25, int pm10, int co2, int aqi) {}
    virtual void onNetworkStatusChanged(bool isUp, const char* ip) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
