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
    virtual Wheel getWheel() {
        return wheel;  
    }
   
};

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

class Spot{
    private:
    string spotNumber;
    Wheel wheel;
    bool status;
    Car *spotOccupiedCar;
    int reservedHour;
    public:
    Spot(string spotNumber,Wheel wheel):spotNumber(spotNumber),wheel(wheel),reservedHour(0),status(false){};
    Wheel getWheel(){
        return wheel;
    }
    void reserveSpot(Car& car,int hour){
        spotOccupiedCar = &car;
        status = true;
        reservedHour  = hour;
    }
      string getSpotNumber(){
        return spotNumber;
    }
    bool getStatus(){
        return status;
    }
    Car* getCar(){
        return spotOccupiedCar;
    }
    int getReservedHour(){
        return reservedHour;
    }
    void releaseSpot(){
        spotOccupiedCar = NULL;
        status = false;
        reservedHour = 0;
    }
    virtual double getPricePerHour() = 0;
};
class TwoWheelerSpot:public Spot{
    public:
    TwoWheelerSpot(string spotNumber):Spot(spotNumber,Wheel::TWO){}
    double getPricePerHour(){
        return TwoWheelerPricePerHour;
    }
};
class FourWheelerSpot:public Spot{
    public:
    FourWheelerSpot(string spotNumber):Spot(spotNumber,Wheel::FOUR){}
    double getPricePerHour(){
        return FourWheelerPricePerHour;
    }
};
class SixWheelerSpot:public Spot{
    public:
    SixWheelerSpot(string spotNumber):Spot(spotNumber,Wheel::SIX){}
    double getPricePerHour(){
        return SixWheelerPricePerHour;
    }
};
class EnumConverter{
       public:
      Wheel convert(string wheelInfo){
        if(wheelInfo == "TWO") return Wheel::TWO;
        else if(wheelInfo == "FOUR") return Wheel::FOUR;
        else if(wheelInfo == "SIX") return Wheel::SIX;
      }
};

class CarFactory{
    EnumConverter converter;
    public:
    Car* CreateCar(string registrationNumber, string brand, string color,string wheels){
        Wheel currentWheels = converter.convert(wheels);
        switch(currentWheels){
            case Wheel::TWO:
                return new TwoWheelerCar(registrationNumber,brand,color);
                break;
            case Wheel::FOUR:
                return new FourWheelerCar(registrationNumber,brand,color);
                break;
            case Wheel::SIX:
                return new SixWheelerCar(registrationNumber,brand,color);
                break;
        }
    }
};
class SpotFactory{
    EnumConverter converter;
    public:
    Spot* CreateSpot(string spotNumber,string wheels){
        Wheel currentWheels = converter.convert(wheels);
        switch(currentWheels){
            case Wheel::TWO:
                return new TwoWheelerSpot(spotNumber);
                break;
            case Wheel::FOUR:
                return new FourWheelerSpot(spotNumber);
                break;
            case Wheel::SIX:
                return new SixWheelerSpot(spotNumber);
                break;
        }
    }
};

class ParkingManager{
    private:
    vector<Car*> CarStorage;
    vector<Spot*> SpotStorage;
    CarFactory carCreator;
    SpotFactory spotCreator;
    public:
    ParkingManager(){
        cout<<"Welome to Car Parking Management System"<<endl;
    }
    void addSpot(string spotNumber,string wheels){
          Spot *currentSpot = spotCreator.CreateSpot(spotNumber,wheels);
          SpotStorage.push_back(currentSpot);
    }
    void reserveSpot(string spotNumber,string registrationNumber, string brand, string color,string wheels,int hour){
            Car* currentCar = carCreator.CreateCar(registrationNumber,brand,color,wheels);
            CarStorage.push_back(currentCar);
            for(Spot* spots:SpotStorage){
                if(spotNumber == spots->getSpotNumber()){
                    spots->reserveSpot(*currentCar,hour);
                }
            }
    }
    double releaseSpotAndBill(string registrationNumber){
        double price;
           for(Spot* spots:SpotStorage){
                if(spots->getStatus()){
                    Car* currentCar = spots->getCar();
                    if(currentCar->getRegistrationNumber() == registrationNumber){
                     price = spots->getPricePerHour()*spots->getReservedHour();
                    spots->releaseSpot();
                    }
                }
            }
        return price;
    }
    void printUnreservedSpotNumber(string wheels){
        EnumConverter converter;
        Wheel wheel = converter.convert(wheels);
         for(Spot* spots:SpotStorage){
                if(spots->getStatus()==false&&spots->getWheel()==wheel){
                    cout<<spots->getSpotNumber()<<endl;
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
int main() {
    // Create a ParkingManager instance
    ParkingManager parkingManager;

    // Add parking spots
    parkingManager.addSpot("A1", "TWO");
    parkingManager.addSpot("A2", "FOUR");
    parkingManager.addSpot("A3", "SIX");
    parkingManager.addSpot("B1", "FOUR");

    // Reserve spots
    parkingManager.reserveSpot("A1", "TN01AA1234", "Yamaha", "Blue", "TWO", 3);
    parkingManager.reserveSpot("A2", "KA05BB5678", "Toyota", "Black", "FOUR", 5);

    // Print unreserved spots for "FOUR" wheeler
    cout << "Unreserved spots for FOUR wheeler:" << endl;
    parkingManager.printUnreservedSpotNumber("FOUR");

    // Print reserved spots details
    cout << "\nReserved spots details:" << endl;
    parkingManager.printReservedSpotsDetails();

    // Release spot and generate bill
    cout << "\nReleasing spot and generating bill for TN01AA1234:" << endl;
    double bill = parkingManager.releaseSpotAndBill("TN01AA1234");
    cout << "Bill: $" << bill << endl;

    // Print reserved spots after releasing
    cout << "\nReserved spots details after releasing:" << endl;
    parkingManager.printReservedSpotsDetails();

    // Print unreserved spots for "TWO" wheeler after release
    cout << "\nUnreserved spots for TWO wheeler after release:" << endl;
    parkingManager.printUnreservedSpotNumber("TWO");

    return 0;
}
