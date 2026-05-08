#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

protected:
    ModelListener* modelListener;

public:
    // Sensor state — updated each tick from shared volatile vars
    int pm25;
    int pm10;
    int co2;
    int aqi;

    bool isNetworkUp;
    char currentIP[16];
    int tickCounter;
};

#endif // MODEL_HPP
