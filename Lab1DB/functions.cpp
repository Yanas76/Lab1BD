#include <iostream>
#include "functions.h"
using namespace std;

void getClientInfo(string& name, string& address, string& number, string& dateOfB){
    cout << "Enter name" << endl;
    cin >> name;
    cout << "Enter emailAddress" << endl;
    cin >> address;
    cout << "Enter phone number" << endl;
    cin >> number;
    cout << "Enter date of birth" << endl;
    cin >> dateOfB;
}

void getReservationInfo(string& dateOfArr, int& days, string& description){
    cout << "Enter date of arrival" << endl;
    cin >> dateOfArr;
    cout << "Enter amount of days" << endl;
    cin >> days;
    cout << "Enter description" << endl;
    cin >> description;
}

void getOptions(){

    cout << "What do you want to do next?" << endl;
    cout << "1 - Add client" << endl;
    cout << "2 - Remove client" << endl;
    cout << "3 - Edit client" << endl;
    cout << "4 - Add reservation" << endl;
    cout << "5 - Remove reservation" << endl;
    cout << "6 - Edit reservation" << endl;
    cout << "7 - Get list of all clients" << endl;
    cout << "8 - Get list of all reservations" << endl;
    cout << "9 - Get list of all reservations for specific client" << endl;
    cout << "10 - Exit" << endl;
}

