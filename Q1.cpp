#include <iostream>
#include <string>
#include <vector>
#include <memory> // For smart pointers

// QUESTION 1
using namespace std;

class Sensor {
public:
    virtual void gatherData() const = 0;
    virtual void processData() const = 0;
    virtual ~Sensor() {} // Adding a virtual destructor for polymorphic behavior
};

class Airspeed : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Airspeed Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Airspeed Sensor" << endl;
    }
};

class Attitude : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Attitude Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Attitude Sensor" << endl;
    }
};

class Altimeter : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Altimeter Sensor" << endl;
    }
    void processData() const override {
        cout << "Processing data from Altimeter Sensor" << endl;
    }
};

class SensorFactory {
public:
    static unique_ptr<Sensor> createSensor(const string& sensorType) {
        if (sensorType == "Airspeed") {
            return make_unique<Airspeed>();
        }
        else if (sensorType == "Attitude") {
            return make_unique<Attitude>();
        }
        else if (sensorType == "Altimeter") {
            return make_unique<Altimeter>();
        }
        else {
            return nullptr;
        }
    }
};

class AerospaceControlSystem {
private:
    vector<unique_ptr<Sensor>> sensors;

public:
    void addSensor(unique_ptr<Sensor> sensor) {
        sensors.push_back(std::move(sensor));
    }

    void monitorAndAdjust() {
        for (const auto& sensor : sensors) {
            sensor->gatherData();
            sensor->processData();
            cout << "Adjusting controls based on sensor data." << endl;
        }
    }
};

int main() {
    AerospaceControlSystem ctrlSys;

    ctrlSys.addSensor(SensorFactory::createSensor("Airspeed"));
    ctrlSys.addSensor(SensorFactory::createSensor("Attitude"));
    ctrlSys.addSensor(SensorFactory::createSensor("Altimeter"));

    ctrlSys.monitorAndAdjust();

    return 0;
}
