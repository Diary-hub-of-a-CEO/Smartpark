#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>

using namespace std;

// ======================================================
// BASE CLASS: Vehicle
// Demonstrates Encapsulation + Inheritance + Polymorphism
// ======================================================

class Vehicle {
private:
    string registrationNumber;
    string ownerName;

protected:
    int parkingSlot;

public:
    // Constructor
    Vehicle(string reg, string owner)
        : registrationNumber(reg), ownerName(owner), parkingSlot(-1) {}

    // Virtual destructor
    virtual ~Vehicle() {}

    // Getter functions - Encapsulation
    string getRegistrationNumber() const {
        return registrationNumber;
    }

    string getOwnerName() const {
        return ownerName;
    }

    int getParkingSlot() const {
        return parkingSlot;
    }

    // Setter
    void setParkingSlot(int slot) {
        parkingSlot = slot;
    }

    // Pure virtual functions - Polymorphism
    virtual double calculateFee(int hours) const = 0;
    virtual string getVehicleType() const = 0;

    // Virtual display function
    virtual void display() const {
        cout << "\nVehicle Type : " << getVehicleType();
        cout << "\nRegistration : " << registrationNumber;
        cout << "\nOwner        : " << ownerName;
        cout << "\nParking Slot : " << parkingSlot;
    }
};


// ======================================================
// DERIVED CLASS: Bike
// ======================================================

class Bike : public Vehicle {
public:

    Bike(string reg, string owner)
        : Vehicle(reg, owner) {}

    double calculateFee(int hours) const override {
        return hours * 20;
    }

    string getVehicleType() const override {
        return "Bike";
    }
};


// ======================================================
// DERIVED CLASS: Car
// ======================================================

class Car : public Vehicle {
public:

    Car(string reg, string owner)
        : Vehicle(reg, owner) {}

    double calculateFee(int hours) const override {
        return hours * 50;
    }

    string getVehicleType() const override {
        return "Car";
    }
};


// ======================================================
// DERIVED CLASS: Bus
// ======================================================

class Bus : public Vehicle {
public:

    Bus(string reg, string owner)
        : Vehicle(reg, owner) {}

    double calculateFee(int hours) const override {
        return hours * 100;
    }

    string getVehicleType() const override {
        return "Bus";
    }
};


// ======================================================
// PARKING LOT CLASS
// ======================================================

class ParkingLot {

private:

    vector<Vehicle*> vehicles;

    const int TOTAL_SLOTS = 10;

    // Find an empty slot
    int findAvailableSlot() const {

        for (int slot = 1; slot <= TOTAL_SLOTS; slot++) {

            bool occupied = false;

            for (Vehicle* vehicle : vehicles) {

                if (vehicle->getParkingSlot() == slot) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied) {
                return slot;
            }
        }

        return -1;
    }


public:

    // Destructor
    ~ParkingLot() {

        for (Vehicle* vehicle : vehicles) {
            delete vehicle;
        }
    }


    // ==================================================
    // PARK VEHICLE
    // ==================================================

    void parkVehicle() {

        if (vehicles.size() >= TOTAL_SLOTS) {
            cout << "\nSorry! Parking lot is full.\n";
            return;
        }

        string registration;
        string owner;
        int type;

        cout << "\nEnter Registration Number: ";
        cin >> registration;

        // Check duplicate registration
        for (Vehicle* vehicle : vehicles) {

            if (vehicle->getRegistrationNumber() == registration) {
                cout << "\nVehicle is already parked!\n";
                return;
            }
        }

        cout << "Enter Owner Name: ";
        cin >> owner;

        cout << "\nSelect Vehicle Type";
        cout << "\n1. Bike";
        cout << "\n2. Car";
        cout << "\n3. Bus";

        cout << "\nEnter choice: ";
        cin >> type;


        Vehicle* vehicle = nullptr;


        try {

            if (type == 1) {

                vehicle = new Bike(registration, owner);

            }
            else if (type == 2) {

                vehicle = new Car(registration, owner);

            }
            else if (type == 3) {

                vehicle = new Bus(registration, owner);

            }
            else {

                throw invalid_argument("Invalid vehicle type!");
            }


            int slot = findAvailableSlot();

            vehicle->setParkingSlot(slot);

            vehicles.push_back(vehicle);


            cout << "\n--------------------------------";
            cout << "\nVehicle parked successfully!";
            cout << "\nAssigned Slot : " << slot;
            cout << "\nVehicle Type  : "
                 << vehicle->getVehicleType();
            cout << "\n--------------------------------\n";


            saveRecord(vehicle);

        }

        catch (exception& e) {

            cout << "\nError: " << e.what() << endl;

            delete vehicle;
        }
    }


    // ==================================================
    // REMOVE VEHICLE
    // ==================================================

    void removeVehicle() {

        string registration;

        cout << "\nEnter Registration Number: ";
        cin >> registration;

        for (int i = 0; i < vehicles.size(); i++) {

            if (vehicles[i]->getRegistrationNumber() == registration) {

                int hours;

                cout << "Enter Parking Hours: ";
                cin >> hours;


                if (hours <= 0) {
                    cout << "\nInvalid parking hours!\n";
                    return;
                }


                // Runtime polymorphism happens here
                double fee =
                    vehicles[i]->calculateFee(hours);


                cout << "\n========== PARKING BILL ==========";

                cout << "\nVehicle Type : "
                     << vehicles[i]->getVehicleType();

                cout << "\nRegistration : "
                     << vehicles[i]->getRegistrationNumber();

                cout << "\nParking Slot : "
                     << vehicles[i]->getParkingSlot();

                cout << "\nHours        : "
                     << hours;

                cout << fixed << setprecision(2);

                cout << "\nTotal Fee    : Rs. "
                     << fee;

                cout << "\n==================================\n";


                delete vehicles[i];

                vehicles.erase(vehicles.begin() + i);

                return;
            }
        }

        cout << "\nVehicle not found!\n";
    }


    // ==================================================
    // SEARCH VEHICLE
    // ==================================================

    void searchVehicle() const {

        string registration;

        cout << "\nEnter Registration Number: ";
        cin >> registration;


        for (Vehicle* vehicle : vehicles) {

            if (vehicle->getRegistrationNumber() == registration) {

                cout << "\n========== VEHICLE FOUND ==========";

                vehicle->display();

                cout << "\n====================================\n";

                return;
            }
        }

        cout << "\nVehicle not found!\n";
    }


    // ==================================================
    // PARKING SUMMARY
    // ==================================================

    void displaySummary() const {

        cout << "\n========== PARKING SUMMARY ==========\n";

        cout << "Total Slots     : " << TOTAL_SLOTS;

        cout << "\nOccupied Slots  : "
             << vehicles.size();

        cout << "\nAvailable Slots : "
             << TOTAL_SLOTS - vehicles.size();


        if (!vehicles.empty()) {

            cout << "\n\nCurrently Parked Vehicles:\n";

            cout << "-------------------------------------";

            for (Vehicle* vehicle : vehicles) {

                cout << "\nSlot " << vehicle->getParkingSlot()
                     << " -> "
                     << vehicle->getRegistrationNumber()
                     << " ("
                     << vehicle->getVehicleType()
                     << ")";
            }
        }

        cout << "\n=====================================\n";
    }


    // ==================================================
    // FILE HANDLING
    // ==================================================

    void saveRecord(Vehicle* vehicle) {

        ofstream file("parking_records.txt", ios::app);

        if (file.is_open()) {

            file << vehicle->getRegistrationNumber()
                 << " | "
                 << vehicle->getOwnerName()
                 << " | "
                 << vehicle->getVehicleType()
                 << " | Slot "
                 << vehicle->getParkingSlot()
                 << endl;

            file.close();
        }
    }
};


// ======================================================
// MAIN FUNCTION
// ======================================================

int main() {

    ParkingLot parkingLot;

    int choice;


    cout << "\n========================================";
    cout << "\n        SMARTPARK MANAGEMENT SYSTEM";
    cout << "\n========================================";


    do {

        cout << "\n\n1. Park Vehicle";
        cout << "\n2. Remove Vehicle";
        cout << "\n3. Search Vehicle";
        cout << "\n4. Parking Summary";
        cout << "\n5. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;


        switch (choice) {

            case 1:
                parkingLot.parkVehicle();
                break;

            case 2:
                parkingLot.removeVehicle();
                break;

            case 3:
                parkingLot.searchVehicle();
                break;

            case 4:
                parkingLot.displaySummary();
                break;

            case 5:
                cout << "\nThank you for using SmartPark!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 5);


    return 0;
}