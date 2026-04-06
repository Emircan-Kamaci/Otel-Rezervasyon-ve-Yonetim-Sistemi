#include <iostream>      // For input and output stream
#include <fstream>       // For file stream
#include <string>        // For string class
#include <vector>        // For vector container
#include <iomanip>       // For stream manipulators like setw
#include <sstream>       // For string stream
#include <algorithm>     // For standard algorithms like none_of and remove_if

using namespace std;     // To avoid using std:: prefix

// Constants for room prices
const double SINGLE_ROOM_PRICE = 50.0;
const double DOUBLE_ROOM_PRICE = 75.0;
const double TRIPLE_ROOM_PRICE = 100.0;

// Base class Room
class Room {
protected:
    int roomNo;         // Room number
    double price;       // Room price
    string type;        // Room type (Single, Double, Triple)
public:
    // Constructor initializing room number, price, and type
    Room(int no, double price, string type) : roomNo(no), price(price), type(type) {}
    
    // Virtual destructor for proper cleanup of derived classes
    virtual ~Room() = default;

    // Getter for room number
    int getRoomNo() const { return roomNo; }
    
    // Virtual function to get room information
    virtual string getInfo() const {
        stringstream ss;
        ss << type << " Room No: " << roomNo;
        return ss.str();
    }
    
    // Virtual function to get price per day
    virtual double getPricePerDay() const { return price; }
};

// Derived class for Single Room
class SingleRoom : public Room {
public:
    // Constructor initializing room number and using base class constructor
    SingleRoom(int no) : Room(no, SINGLE_ROOM_PRICE, "Single") {}
};

// Derived class for Double Room
class DoubleRoom : public Room {
public:
    // Constructor initializing room number and using base class constructor
    DoubleRoom(int no) : Room(no, DOUBLE_ROOM_PRICE, "Double") {}
};

// Derived class for Triple Room
class TripleRoom : public Room {
public:
    // Constructor initializing room number and using base class constructor
    TripleRoom(int no) : Room(no, TRIPLE_ROOM_PRICE, "Triple") {}
};

// Class for Reservation
class Reservation {
private:
    string name, email, phone;  // Customer details
    int roomNo;                 // Reserved room number
    int stayDuration;           // Duration of stay in days
public:
    // Constructor initializing all member variables
    Reservation(string name, string email, string phone, int room, int stayDuration)
        : name(name), email(email), phone(phone), roomNo(room), stayDuration(stayDuration) {}

    // Getter for name
    string getName() const { return name; }
    // Getter for email
    string getEmail() const { return email; }
    // Getter for phone
    string getPhone() const { return phone; }
    // Getter for room number
    int getRoomNo() const { return roomNo; }
    // Getter for stay duration
    int getStayDuration() const { return stayDuration; }

    // Function to print reservation details
    void print() const {
        cout << "\nReservation Details:\n"
             << "\nFullname: " << name << "\n"
             << "Email: " << email << "\n"
             << "Phone: " << phone << "\n"
             << "Room No: " << roomNo << "\n"
             << "Stay Duration: " << stayDuration << " days" << endl;
    }
};

// Class for Hotel management
class Hotel {
private:
    vector<Room*> rooms;          // Vector to store rooms
    vector<Reservation> reservations; // Vector to store reservations

    // Function to check if a room is available
    bool isRoomAvailable(int roomNo) const {
        return none_of(reservations.begin(), reservations.end(), [&](const Reservation& r) {
            return r.getRoomNo() == roomNo;
        });
    }

    // Function to calculate total price for a stay
    double calculateTotalPrice(int roomNo, int stayDuration) const {
        for (const auto& room : rooms) {
            if (room->getRoomNo() == roomNo) {
                return room->getPricePerDay() * stayDuration;
            }
        }
        return 0;
    }

public:
    // Constructor to initialize rooms
    Hotel() {
        for (int i = 1; i <= 5; ++i) rooms.push_back(new SingleRoom(i));
        for (int i = 6; i <= 10; ++i) rooms.push_back(new DoubleRoom(i));
        for (int i = 11; i <= 15; ++i) rooms.push_back(new TripleRoom(i));
    }

    // Destructor to clean up allocated room objects
    ~Hotel() {
        for (auto room : rooms) {
            delete room;
        }
    }

    // Function to list empty rooms
    void listEmptyRooms() const {
        cout << "==========================================================\n"
             << "                       Rooms\n"
             << "==========================================================\n"
             << "     50 Euro             75 Euro              100 Euro\n";

        vector<string> singleRoomInfo, doubleRoomInfo, tripleRoomInfo;
        for (const auto& room : rooms) {
            if (isRoomAvailable(room->getRoomNo())) {
                if (room->getPricePerDay() == SINGLE_ROOM_PRICE)
                    singleRoomInfo.push_back(room->getInfo());
                else if (room->getPricePerDay() == DOUBLE_ROOM_PRICE)
                    doubleRoomInfo.push_back(room->getInfo());
                else
                    tripleRoomInfo.push_back(room->getInfo());
            }
        }

        size_t maxRows = max({singleRoomInfo.size(), doubleRoomInfo.size(), tripleRoomInfo.size()});
        for (size_t i = 0; i < maxRows; ++i) {
            cout << left << setw(20) << (i < singleRoomInfo.size() ? singleRoomInfo[i] : "")
                 << left << setw(20) << (i < doubleRoomInfo.size() ? doubleRoomInfo[i] : "")
                 << (i < tripleRoomInfo.size() ? tripleRoomInfo[i] : "") << endl;
        }
        cout << "----------------------------------------------------------" << endl;
    }

    // Function to make a reservation
    void makeReservation() {
        string name, email, phone;
        int roomNo, stayDuration;

        cout << "\nFullname: ";
        cin.ignore();
        getline(cin, name);

        cout << "Email: ";
        getline(cin, email);

        cout << "Phone: ";
        getline(cin, phone);

        cout << "Enter the room number you want to reserve (1-15): ";
        while (!(cin >> roomNo) || roomNo < 1 || roomNo > 15) {
            cout << "Invalid input! Please enter a room number between 1 and 15: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter the duration of stay (in days): ";
        cin >> stayDuration;

        if (!isRoomAvailable(roomNo)) {
            cout << "\nThe room is not available. Please try again." << endl;
            return;
        }

        double totalPrice = calculateTotalPrice(roomNo, stayDuration);
        cout << "\n\nThe total price for your stay is: " << totalPrice << " Euro." << endl;
        cout << "\nThe payment will be made upon check-in at the hotel." << endl;

        reservations.emplace_back(name, email, phone, roomNo, stayDuration);
        cout << "\nRoom " << roomNo << " reserved." << endl;
    }

    // Function to cancel a reservation
    void cancelReservation(int roomNo) {
        auto it = remove_if(reservations.begin(), reservations.end(), [roomNo](const Reservation& r) {
            return r.getRoomNo() == roomNo;
        });
        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            cout << "Reservation for Room " << roomNo << " canceled." << endl;
        } else {
            cout << "No reservation found for Room " << roomNo << "." << endl;
        }
    }

    // Function to save reservation data to a file
    void saveData(const string& fileName) const {
        ofstream file(fileName);
        if (file.is_open()) {
            for (const auto& reservation : reservations) {
                file << reservation.getName() << "\n"
                     << reservation.getEmail() << "\n"
                     << reservation.getPhone() << "\n"
                     << reservation.getRoomNo() << "\n"
                     << reservation.getStayDuration() << "\n";
            }
            cout << "\nData saved to file." << endl;
        } else {
            cout << "\nUnable to open file!" << endl;
        }
    }

    // Function to load reservation data from a file
    void loadData(const string& fileName) {
        ifstream file(fileName);
        if (file.is_open()) {
            string name, email, phone;
            int roomNo, stayDuration;
            while (getline(file, name) && getline(file, email) && getline(file, phone) && file >> roomNo >> stayDuration) {
                file.ignore();
                reservations.emplace_back(name, email, phone, roomNo, stayDuration);
            }
            cout << "\nData loaded from file." << endl;
        } else {
            cout << "\nUnable to open file!" << endl;
        }
    }

    // Function to list all reservations
    void listReservations() const {
        if (reservations.empty()) {
            cout << "No reservations found. Please press 6 to load data from a file." << endl;
        } else {
            for (const auto& reservation : reservations) {
                reservation.print();
                cout << endl;
            }
        }
    }
};

// Main function
int main() {
    // Welcome message
    cout << "===========================================\n"
         << "     Welcome to the Programming Hotel!\n"
         << "===========================================\n";

    Hotel hotel;  // Create a Hotel object

    int choice, roomNo;  // Variables to store user choices

    do {
        // Display menu options
        cout << "\n1. List Empty Rooms\n"
             << "2. Make Reservation\n"
             << "3. Cancel Reservation\n"
             << "4. List Reservations\n"
             << "5. Save Data to File\n"
             << "6. Load Data from File\n"
             << "7. Exit\n"
             << "\nEnter your choice: ";
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Switch case for user choices
        switch (choice) {
            case 1:
                hotel.listEmptyRooms();  // List available rooms
                break;
            case 2:
                hotel.makeReservation();  // Make a reservation
                break;
            case 3:
                cout << "\nEnter the room number of the reservation you want to cancel (1-15): ";
                while (!(cin >> roomNo) || roomNo < 1 || roomNo > 15) {
                    cout << "Invalid input! Please enter a room number between 1 and 15: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                hotel.cancelReservation(roomNo);  // Cancel the reservation
                break;
            case 4:
                hotel.listReservations();  // List all reservations
                break;
            case 5:
                hotel.saveData("hotel_data.txt");  // Save data to file
                break;
            case 6:
                hotel.loadData("hotel_data.txt");  // Load data from file
                break;
            case 7:
                cout << "\nThank you for choosing Programming Hotel. Have a great day!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);  // Loop until user chooses to exit

    return 0;  // Return 0 to indicate successful execution
}