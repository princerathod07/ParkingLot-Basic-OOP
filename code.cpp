#include <bits/stdc++.h>
using namespace std;

class vehicle {
protected:
    string plateN;

public:
    vehicle(string plate = "UNKNOWN") {
        plateN = plate;
    }

    virtual void dispInfo() {
        cout << "Vehicle with plate: " << plateN << endl;
    }

    virtual string getType() { return "Vehicle"; }

    string getPlate() { return plateN; }

    virtual ~vehicle() {}
};

class car : public vehicle {
public:
    car(string plate) : vehicle(plate) {}

    void dispInfo() override {
        cout << "Car number: " << plateN << endl;
    }

    string getType() override { return "Car"; }
};

class bike : public vehicle {
public:
    bike(string plate) : vehicle(plate) {}

    void dispInfo() override {
        cout << "Bike number: " << plateN << endl;
    }

    string getType() override { return "Bike"; }
};

class parkingLot {
    int capacity;
    vector<unique_ptr<vehicle>> slots;

    const string filename = "data.txt";

public:
    parkingLot(int total) {
        capacity = total;
        ofstream("data.txt").close();  
        loadFromFile();
    }

    bool isDuplicate(const string &plate) {
        for (auto &v : slots) {
            if (v->getPlate() == plate) {
                return true;
            }
        }
        return false;
    }

    void parkVehicle(unique_ptr<vehicle> v) {
        if ((int)slots.size() >= capacity) {
            cout << "Parking is Full! Cannot park.\n";
            return;
        }

        if (isDuplicate(v->getPlate())) {
            cout << "Error: Vehicle with plate number " << v->getPlate()
                 << " is already parked!\n";
            return;
        }

        cout << v->getType() << " parked successfully.\n";
        slots.push_back(move(v));
        saveToFile();
    }

    void removeVehicle() {
        if (!slots.empty()) {
            cout << "Removing: ";
            slots.back()->dispInfo();
            slots.pop_back();
            saveToFile();
        } else {
            cout << "No vehicles to remove.\n";
        }
    }

    void dispStatus() {
        cout << "\nParking Lot Status:\n";
        cout << "Total slots: " << capacity
             << "   Occupied: " << slots.size()
             << "   Free: " << (capacity - slots.size()) << endl;

        for (size_t i = 0; i < slots.size(); i++) {
            cout << i + 1 << ". ";
            slots[i]->dispInfo();
        }
        cout << "......................................\n";
    }

private:
    void saveToFile() {
        ofstream fout(filename);
        if (!fout) {
            cerr << "Error opening file for writing!\n";
            return;
        }
        for (auto &v : slots) {
            fout << v->getType() << " " << v->getPlate() << endl;
        }
        fout.close();
    }

    void loadFromFile() {
        ifstream fin(filename);
        if (!fin) {
            return;
        }
        string type, plate;
        while (fin >> type >> plate) {
            if (type == "Car") {
                slots.push_back(make_unique<car>(plate));
            } else if (type == "Bike") {
                slots.push_back(make_unique<bike>(plate));
            }
        }
        fin.close();
    }
};

int main() {
    int tslot;
    cout << "Enter total parking capacity: ";
    cin >> tslot;

    parkingLot lot(tslot);

    while (1) {
        int choice;
        cout << "\n--- Parking Lot Menu ---\n";
        cout << "1. Park Car\n";
        cout << "2. Park Bike\n";
        cout << "3. Remove Last Vehicle\n";
        cout << "4. Display Status\n";
        cout << "0. Exit Program\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting program......!!\n";
            break;
        }

        string plate;
        switch (choice) {
            case 1:
                cout << "Enter Car plate number: ";
                cin >> plate;
                lot.parkVehicle(make_unique<car>(plate));
                break;
            case 2:
                cout << "Enter Bike plate number: ";
                cin >> plate;
                lot.parkVehicle(make_unique<bike>(plate));
                break;
            case 3:
                lot.removeVehicle();
                break;
            case 4:
                lot.dispStatus();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}
