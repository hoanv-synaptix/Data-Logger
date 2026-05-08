#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <string.h>

// LwIP Includes removed for testing

// Shared volatile variables — written by sensor FreeRTOS task.
// Declare in sensor task: extern volatile int g_pm25, g_pm10, g_co2, g_aqi;
volatile int g_pm25 = 35;
volatile int g_pm10 = 58;
volatile int g_co2  = 510;
volatile int g_aqi  = 72;

Model::Model() : modelListener(0), pm25(-1), pm10(-1), co2(-1), aqi(-1), isNetworkUp(false), tickCounter(0)
{
    // Initialize to -1 so the first real tick always triggers a UI update
    // only AFTER setupScreen() has been called and presenter is bound.
    currentIP[0] = '\0';
}

void Model::tick()
{
    if(!modelListener) return;

    int newPM25 = (int)g_pm25;
    int newPM10 = (int)g_pm10;
    int newCO2  = (int)g_co2;
    int newAQI  = (int)g_aqi;

    if(newPM25 != pm25 || newPM10 != pm10 || newCO2 != co2 || newAQI != aqi)
    {
        pm25 = newPM25;
        pm10 = newPM10;
        co2  = newCO2;
        aqi  = newAQI;

        modelListener->onSensorDataUpdated(pm25, pm10, co2, aqi);
    }

    tickCounter++;
    if(tickCounter >= 60) { // Check every ~1 second (assuming 60Hz tick)
        tickCounter = 0;
        
        // FAKE NETWORK STATUS FOR TESTING UI
        bool currentStatus = false;
        
        // Notify listener
        if(modelListener != 0) {
            modelListener->onNetworkStatusChanged(currentStatus, currentStatus ? currentIP : "Offline");
        }
    }
}
