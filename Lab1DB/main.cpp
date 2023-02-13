#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "functions.h"
using namespace std;

struct Client{
public:
    char name[40];
    char emailAddress[40];
    char phoneNumber[40];
    char dateOfBirth[40];
    int reservationsCount;
};

struct Identificator{
public:
    int cliId;
    int32_t clientAddress;
};

struct Reservation{
public:
    int reservationId;
    int clientId;
    char dateOfArrival[40];
    char description[40];
    int days;
};

Identificator getClientIndex(int position){

    Identificator index;
    FILE* IndexDB = fopen("clientIndex.bin", "rb");
    fseek(IndexDB, position * sizeof(Identificator), SEEK_SET);
    fread(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);
    return index;
}

Client getClient(int position) {

    Client client;
    Identificator index = getClientIndex(position);
    FILE *clientDB = fopen("clients.bin", "rb");
    fseek(clientDB, index.clientAddress, SEEK_SET);
    fread(&client, sizeof(Client), 1, clientDB);
    fclose(clientDB);
    return client;
}

Reservation getReservation(int position){

    Reservation reservation;
    FILE* reservationsDB = fopen("reservations.bin", "rb");
    fseek(reservationsDB, position * sizeof(Reservation), SEEK_SET);
    fread(&reservation, sizeof(Reservation), 1, reservationsDB);
    fclose(reservationsDB);
    return reservation;
}


void writeClient(Client& client, int position){

    Identificator index = getClientIndex(position);
    FILE* clientsDB = fopen("clients.bin", "rb+");
    fseek(clientsDB, index.clientAddress, SEEK_SET);
    fwrite(&client, sizeof(Client), 1, clientsDB);
    fclose(clientsDB);
}

void writeReservation(Reservation& reservation, int position){

    FILE* reservationsDB = fopen("reservations.bin", "rb+");
    fseek(reservationsDB, position * sizeof(Reservation), SEEK_SET);
    fwrite(&reservation, sizeof(Reservation), 1, reservationsDB);
    fclose(reservationsDB);
}

void writeIndex(Identificator index, int position){

    FILE* clientIndex = fopen("clientIndex.bin", "rb+");
    fseek(clientIndex, position * sizeof(Identificator), SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, clientIndex);
    fclose(clientIndex);
}

int getAmountOfClients(){

    FILE* cliDb = fopen("clients.bin", "ab+");
    fseek(cliDb, 0, SEEK_END);
    int count = ftell(cliDb)/sizeof(Client);
    fclose(cliDb);
    return count;
}

int getAmountOfReservations(){

    FILE* reservationsDB = fopen("reservations.bin", "ab+");
    fseek(reservationsDB, 0, SEEK_END);
    int count = ftell(reservationsDB) / sizeof(Reservation);
    fclose(reservationsDB);
    return count;
}

void addClient(Client& client) {

    Identificator index;
    FILE* IndexDB = fopen("clientIndex.bin", "ab+");
    fseek(IndexDB, 0, SEEK_END);
    int count = ftell(IndexDB)/sizeof(Identificator) + 1;

    index.cliId = count;
    index.clientAddress = (count - 1) * sizeof(Client);

    fseek(IndexDB, 0, SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);

    FILE* cliDb = fopen("clients.bin", "ab+");
    fwrite(&client, sizeof(Client), 1, cliDb);
    fclose(cliDb);
}

void addReservation(Reservation& reservation, int clientId){

    Client client = getClient(clientId - 1);
    client.reservationsCount++;
    writeClient(client, clientId - 1);

    FILE* reservationsDB = fopen("reservations.bin", "ab+");
    fseek(reservationsDB, 0, SEEK_END);
    reservation.reservationId = ftell(reservationsDB)/sizeof(Reservation) + 1;
    fwrite(&reservation, sizeof(Reservation), 1, reservationsDB);
    fclose(reservationsDB);
}

int idClientValidator(int id){

    int all = getAmountOfClients();
    if(id > all || id < 1){
        cout << "Wrong id!" << endl;
        cout << endl;
        return -1;
    }

    Identificator index = getClientIndex(id - 1);
    if(index.cliId == -1){
        cout << "This client is already deleted!" << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

int idReservationValidator(int id){

    int all = getAmountOfReservations();
    if(id > all || id < 1){
        cout << "Wrong id!" << endl;
        cout << endl;
        return -1;
    }

    Reservation reservation = getReservation(id - 1);
    if(reservation.reservationId == -1){
        cout << "This reservation is already deleted!" << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

void deleteClient(int id){

    int result = idClientValidator(id);
    if(result == -1) return;

    Identificator index = getClientIndex(id - 1);
    Client client = getClient(id - 1);
    Reservation reservation;
    int amount = getAmountOfReservations();

    if(client.reservationsCount > 0){
        for(int i = 0; i < amount; i++){
            reservation = getReservation(i);
            if(index.cliId == reservation.clientId){
                reservation.reservationId = -1;
                writeReservation(reservation, i);
            }
        }
    }

    index.cliId = -1;
    writeIndex(index, id - 1);
    cout << "Removed client with all reservations" <<endl;
    cout << endl;
}

void deleteReservation(int clientId, int reservationId){

    Client client = getClient(clientId - 1);
    client.reservationsCount--;
    writeClient(client, clientId - 1);

    Reservation reservation = getReservation(reservationId - 1);
    reservation.reservationId = -1;
    writeReservation(reservation, reservationId - 1);
}


int main() {

    Client client;
    Reservation reservation;
    Identificator index;
    int choice  = 0;
    int id, clientId, days, amount;
    string name, address, number, dateOfB, dateOfArr, description;

    getOptions();
    while(true){

        while(!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Wrong input, try again" << endl;
        }

        switch (choice) {
            case 1:

                getClientInfo(name, address, number, dateOfB);
                strcpy(client.name, name.c_str());
                strcpy(client.emailAddress, name.c_str());
                strcpy(client.phoneNumber, name.c_str());
                strcpy(client.dateOfBirth, name.c_str());
                client.reservationsCount = 0;
                addClient(client);
                break;

            case 2:
                cout << "Enter id of a client that you want to remove" << endl;
                cin >> id;
                deleteClient(id);
                break;

            case 3:
                cout << "Enter id of a client that you want to edit" << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {
                    getClientInfo(name, address, number, dateOfB);
                    client = getClient(id - 1);
                    strcpy(client.name, name.c_str());
                    strcpy(client.emailAddress, address.c_str());
                    strcpy(client.phoneNumber, number.c_str());
                    strcpy(client.dateOfBirth, dateOfB.c_str());
                    writeClient(client, id - 1);
                }
                break;

            case 4:
                cout << "Enter id of a client who wants to make a reservation" << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {

                    getReservationInfo(dateOfArr, days, description);
                    reservation.days = days;
                    reservation.clientId = id;
                    strcpy(reservation.dateOfArrival, dateOfArr.c_str());
                    strcpy(reservation.description, description.c_str());
                    addReservation(reservation, id);
                }
                break;

            case 5:
                cout << "Enter client id" << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.reservationsCount > 0) {

                        cout << "Enter reservation id" << endl;
                        cin >> id;
                        if (idReservationValidator(id) == 0) deleteReservation(clientId, id);

                    } else  cout << "This client has no reservations" << endl;
                }
                break;

            case 6:
                cout << "Enter client id" << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.reservationsCount > 0) {

                        cout << "Enter reservation id" << endl;
                        cin >> id;
                        if (idReservationValidator(id) == 0) {

                            reservation = getReservation(id - 1);
                            getReservationInfo(dateOfArr, days, description);
                            reservation.days = days;
                            strcpy(reservation.dateOfArrival, dateOfArr.c_str());
                            strcpy(reservation.description, description.c_str());
                            writeReservation(reservation, id - 1);
                            cout << "Reservation updated" << endl;
                        }
                    } else cout << "This client has no reservations" << endl;
                    cout << endl;
                }
                break;

            case 7:
                amount = getAmountOfClients();
                cout << "The list of all clients:" << endl;
                for (int i = 0; i < amount; i++) {
                    client = getClient(i);
                    index = getClientIndex(i);
                    if (index.cliId != -1) {
                        cout << "client id: " << index.cliId << endl;
                        cout << "client name: " << client.name << endl;
                        cout << "amount of reservations: " << client.reservationsCount << endl;
                        cout << endl;
                    }
                }

                break;

            case 8:
                amount = getAmountOfReservations();
                cout << "All reservations are:" << endl;
                for (int i = 0; i < amount; i++) {
                    reservation = getReservation(i);
                    if (reservation.reservationId != -1) {
                        cout << "reservation id: " << reservation.reservationId << endl;
                        cout << "description: " << reservation.description << endl;
                        cout << endl;
                    }
                }
                break;

            case 9:
                cout << "Enter id of a client" << endl;
                cin >> id;
                amount = getAmountOfReservations();
                if (idClientValidator(id) == 0) {
                    client = getClient(id - 1);
                    if (client.reservationsCount > 0) {
                        cout << "List of reservations for this client:" << endl;
                        for (int i = 0; i < amount; i++) {
                            reservation = getReservation(i);
                            if (id == reservation.clientId){
                                cout << "reservation id: " << reservation.reservationId << endl;
                                cout << "description: " << reservation.description << endl;
                            }
                        }
                    } else cout << "This client has no reservations" << endl;
                    cout << endl;
                }
                break;

            case 10:
                return 0;

            default:
                cout << "Wrong input!" << endl;
                break;
        }
        cout << "What do you want to do next?" << endl;
    }
}

