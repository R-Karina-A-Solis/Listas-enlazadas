#include <iostream>
#include <fstream>
#include <string>
#include <ctime> // Para medir tiempo
#include <cstdlib> // Para atoi
#include <thread>
using namespace std;

// Definición de la estructura de nodo
struct Player {
    int id;
    string name;
    int score;
    Player* next;
};

// Función para crear un nuevo nodo
Player* createNode(int id, string name, int score) {
    Player* newNode = new Player();
    newNode->id = id;
    newNode->name = name;
    newNode->score = score;
    newNode->next = NULL;
    return newNode;
}

// Función para agregar un nodo al final de la lista
void appendNode(Player*& head, int id, string name, int score) {
    Player* newNode = createNode(id, name, score);
    if (head == NULL) {
        head = newNode;
        return;
    }
    Player* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Función para calcular la puntuación promedio
double calculateAverage(Player* head) {
    double sum = 0;
    int count = 0;
    Player* temp = head;
    while (temp != NULL) {
        sum += temp->score;
        count++;
        temp = temp->next;
    }
    return (count == 0) ? 0 : sum / count;
}

// Función para encontrar el jugador con la puntuación más alta
Player* findHighestScore(Player* head) {
    Player* highest = head;
    Player* temp = head;
    while (temp != NULL) {
        if (temp->score > highest->score) {
            highest = temp;
        }
        temp = temp->next;
    }
    return highest;
}

// Función para encontrar el jugador con la puntuación más baja
Player* findLowestScore(Player* head) {
    Player* lowest = head;
    Player* temp = head;
    while (temp != NULL) {
        if (temp->score < lowest->score) {
            lowest = temp;
        }
        temp = temp->next;
    }
    return lowest;
}

// Función para eliminar jugadores con puntuaciones por debajo del promedio
void removeBelowAverage(Player*& head, double average) {
    Player* temp = head;
    Player* prev = NULL;
    while (temp != NULL) {
        if (temp->score < average) {
            if (prev != NULL) {
                prev->next = temp->next;
            } else {
                head = temp->next;
            }
            Player* toDelete = temp;
            temp = temp->next;
            delete toDelete;

            // Introduce una pausa de 1000 microsegundos (1 milisegundo)
            this_thread::sleep_for(chrono::microseconds(1000)); // Pausa de 1000 microsegundos (1 milisegundo)
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
}

// Función principal
int main() {
    Player* head = NULL;
    ifstream inputFile("jugadores.txt");
    string line;

    // Leer datos desde el archivo
    while (getline(inputFile, line)) {
        int id, score;
        string name;
        size_t firstSpace = line.find(' ');
        size_t lastSpace = line.rfind(' ');

        id = atoi(line.substr(0, firstSpace).c_str());
        name = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
        score = atoi(line.substr(lastSpace + 1).c_str());

        appendNode(head, id, name, score);
    }
    inputFile.close();

    // Calcular la puntuación promedio
    double average = calculateAverage(head);
    cout << "Puntuacion media: " << average << endl;

    // Encontrar jugadores con puntuación más alta y más baja
    Player* highest = findHighestScore(head);
    Player* lowest = findLowestScore(head);

    cout << "Puntuacion mas alta: ID=" << highest->id
         << ", Nombre=" << highest->name
         << ", Puntuacion=" << highest->score << endl;

    cout << "Puntuacion mas baja: ID=" << lowest->id
         << ", Nombre=" << lowest->name
         << ", Puntuacion=" << lowest->score << endl;

    // Eliminar jugadores por debajo del promedio y medir el tiempo
    clock_t start = clock();
    removeBelowAverage(head, average);
    clock_t end = clock();

    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Tiempo para eliminar jugadores por debajo del promedio: " << elapsed << " segundos" << endl;

    return 0;
}
