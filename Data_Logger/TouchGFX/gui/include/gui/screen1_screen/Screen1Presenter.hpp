#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <mvp/Presenter.hpp>
#include <gui/model/ModelListener.hpp>

using namespace touchgfx;

class Screen1View;

class Screen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);

    virtual void activate();
    virtual void deactivate();

    // Called every frame from Model::tick() when data changes
    virtual void onSensorDataUpdated(int pm25, int pm10, int co2, int aqi);
    virtual void onNetworkStatusChanged(bool isUp, const char* ip);

private:
    Screen1Presenter();
    Screen1View& view;
};

#endif