#include <bits/stdc++.h>
using namespace std;

// Scoped enum for Wheel types
const double TwoWheelerPricePerHour = 20.0;
const double FourWheelerPricePerHour = 40.0;
const double SixWheelerPricePerHour = 60.5;

enum class Wheel {
    TWO,
    FOUR,
    SIX
};

// Base class for Car
class Car {
private:
    string registrationNumber;
    string brand;
    string color;
    Wheel wheel;

public:
    Car(string registrationNumber, string brand, string color, Wheel wheel)
        : registrationNumber(registrationNumber), brand(brand), color(color), wheel(wheel) {}

    string getRegistrationNumber() {
        return registrationNumber;
    }
    string getBrand() {
        return brand;
    }
    string getColor() {
        return color;
    }
    Wheel getWheel() {
        return wheel;
    }
};

// Derived Car classes
class TwoWheelerCar : public Car {
public:
    TwoWheelerCar(string registrationNumber, string brand, string color)
        : Car(registrationNumber, brand, color, Wheel::TWO) {}
};

class FourWheelerCar : public Car {
public:
    FourWheelerCar(string registrationNumber, string brand, string color)
        : Car(registrationNumber, brand, color, Wheel::FOUR) {}
};

class SixWheelerCar : public Car {
public:
    SixWheelerCar(string registrationNumber, string brand, string color)
        : Car(registrationNumber, brand, color, Wheel::SIX) {}
};

// Spot base class
class Spot {
private:
    string spotNumber;
    Wheel wheel;
    bool status;
    Car *spotOccupiedCar;
    int reservedHour;

public:
    Spot(string spotNumber, Wheel wheel)
        : spotNumber(spotNumber), wheel(wheel), reservedHour(0), status(false), spotOccupiedCar(nullptr) {}

    Wheel getWheel() {
        return wheel;
    }
    string getSpotNumber() {
        return spotNumber;
    }
    bool getStatus() {
        return status;
    }
    Car* getCar() {
        return spotOccupiedCar;
    }
    int getReservedHour() {
        return reservedHour;
    }
    void reserveSpot(Car &car, int hour) {
        spotOccupiedCar = &car;
        status = true;
        reservedHour = hour;
    }
    void releaseSpot() {
        spotOccupiedCar = nullptr;
        status = false;
        reservedHour = 0;
    }
    virtual double getPricePerHour() = 0;
};

// Derived Spot classes
class TwoWheelerSpot : public Spot {
public:
    TwoWheelerSpot(string spotNumber) : Spot(spotNumber, Wheel::TWO) {}
    double getPricePerHour() override {
        return TwoWheelerPricePerHour;
    }
};

class FourWheelerSpot : public Spot {
public:
    FourWheelerSpot(string spotNumber) : Spot(spotNumber, Wheel::FOUR) {}
    double getPricePerHour() override {
        return FourWheelerPricePerHour;
    }
};

class SixWheelerSpot : public Spot {
public:
    SixWheelerSpot(string spotNumber) : Spot(spotNumber, Wheel::SIX) {}
    double getPricePerHour() override {
        return SixWheelerPricePerHour;
    }
};

// Utility for converting string to Wheel enum
class EnumConverter {
public:
    Wheel convert(string wheelInfo) {
        if (wheelInfo == "TWO") return Wheel::TWO;
        else if (wheelInfo == "FOUR") return Wheel::FOUR;
        else if (wheelInfo == "SIX") return Wheel::SIX;
        throw invalid_argument("Invalid wheel type");
    }
};

// Factories for creating cars and spots
class CarFactory {
    EnumConverter converter;

public:
    Car* CreateCar(string registrationNumber, string brand, string color, string wheels) {
        Wheel currentWheels = converter.convert(wheels);
        switch (currentWheels) {
            case Wheel::TWO:
                return new TwoWheelerCar(registrationNumber, brand, color);
            case Wheel::FOUR:
                return new FourWheelerCar(registrationNumber, brand, color);
            case Wheel::SIX:
                return new SixWheelerCar(registrationNumber, brand, color);
        }
        return nullptr;
    }
};

class SpotFactory {
    EnumConverter converter;

public:
    Spot* CreateSpot(string spotNumber, string wheels) {
        Wheel currentWheels = converter.convert(wheels);
        switch (currentWheels) {
            case Wheel::TWO:
                return new TwoWheelerSpot(spotNumber);
            case Wheel::FOUR:
                return new FourWheelerSpot(spotNumber);
            case Wheel::SIX:
                return new SixWheelerSpot(spotNumber);
        }
        return nullptr;
    }
};

// Parking Manager
class ParkingManager {
private:
    vector<unique_ptr<Car>> CarStorage;
    vector<unique_ptr<Spot>> SpotStorage;
    CarFactory carCreator;
    SpotFactory spotCreator;

public:
    ParkingManager() {
        cout << "Welcome to Car Parking Management System" << endl;
    }

    void addSpot(string spotNumber, string wheels) {
        unique_ptr<Spot> currentSpot(spotCreator.CreateSpot(spotNumber, wheels));
        SpotStorage.push_back(move(currentSpot));
    }

    void reserveSpot(string spotNumber, string registrationNumber, string brand, string color, string wheels, int hour) {
        unique_ptr<Car> currentCar(carCreator.CreateCar(registrationNumber, brand, color, wheels));
        for (auto &spot : SpotStorage) {
            if (spot->getSpotNumber() == spotNumber && !spot->getStatus()) {
                spot->reserveSpot(*currentCar, hour);
                CarStorage.push_back(move(currentCar));
                return;
            }
        }
        cout << "Spot not available for reservation" << endl;
    }

    double releaseSpotAndBill(string registrationNumber) {
        for (auto &spot : SpotStorage) {
            if (spot->getStatus() && spot->getCar()->getRegistrationNumber() == registrationNumber) {
                double price = spot->getPricePerHour() * spot->getReservedHour();
                spot->releaseSpot();
                return price;
            }
        }
        cout << "Car not found" << endl;
        return 0.0;
    }

    void printUnreservedSpotNumber(string wheels) {
        EnumConverter converter;
        Wheel wheel = converter.convert(wheels);
        for (auto &spot : SpotStorage) {
            if (!spot->getStatus() && spot->getWheel() == wheel) {
                cout << spot->getSpotNumber() << endl;
            }
        }
    }

    void printReservedSpotsDetails() {
        for (auto &spot : SpotStorage) {
            if (spot->getStatus()) {
                Car *car = spot->getCar();
                cout << "Spot Number: " << spot->getSpotNumber()
                     << ", Car Registration: " << car->getRegistrationNumber()
                     << ", Brand: " << car->getBrand()
                     << ", Color: " << car->getColor()
                     << ", Reserved Hours: " << spot->getReservedHour() << endl;
            }
        }
    }
};
