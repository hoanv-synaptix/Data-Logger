#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/screen1_screen/Screen1View.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{
}

void Screen1Presenter::activate()
{
}

void Screen1Presenter::deactivate()
{
}

// Triggered by Model::tick() — runs in TouchGFX task context (safe to call View)
void Screen1Presenter::onSensorDataUpdated(int pm25, int pm10, int co2, int aqi)
{
    view.updateSensorData(pm25, pm10, co2, aqi);
}

void Screen1Presenter::onNetworkStatusChanged(bool isUp, const char* ip)
{
    view.updateNetworkData(isUp, ip);
}