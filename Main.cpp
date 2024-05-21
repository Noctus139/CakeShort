#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine

using namespace std;

// Plate struct to hold the plate data
struct Plate {
    vector<int> slots;
    Plate() : slots(6, 0) {}
};

// Function to fill empty slots from adjacent plates
void fillEmptySlots(vector<vector<Plate>>& matrix, size_t i, size_t j) {
    cout << "Filling empty slots for plate at (" << i << ", " << j << ")\n";
    bool filled;
    do {
        filled = false;
        // Checking the right plate
        if (j + 1 < 4) {
            for (size_t k = 0; k < 6; ++k) {
                if (matrix[i][j].slots[k] == 0 && matrix[i][j + 1].slots[k] != 0) {
                    matrix[i][j].slots[k] = matrix[i][j + 1].slots[k];
                    matrix[i][j + 1].slots[k] = 0;
                    filled = true;
                }
            }
        }

        // Checking the left plate
        if (j > 0) {
            for (size_t k = 0; k < 6; ++k) {
                if (matrix[i][j].slots[k] == 0 && matrix[i][j - 1].slots[k] != 0) {
                    matrix[i][j].slots[k] = matrix[i][j - 1].slots[k];
                    matrix[i][j - 1].slots[k] = 0;
                    filled = true;
                }
            }
        }

        // Checking the bottom plate
        if (i + 1 < 5) {
            for (size_t k = 0; k < 6; ++k) {
                if (matrix[i][j].slots[k] == 0 && matrix[i + 1][j].slots[k] != 0) {
                    matrix[i][j].slots[k] = matrix[i + 1][j].slots[k];
                    matrix[i + 1][j].slots[k] = 0;
                    filled = true;
                }
            }
        }

        // Checking the top plate
        if (i > 0) {
            for (size_t k = 0; k < 6; ++k) {
                if (matrix[i][j].slots[k] == 0 && matrix[i - 1][j].slots[k] != 0) {
                    matrix[i][j].slots[k] = matrix[i - 1][j].slots[k];
                    matrix[i - 1][j].slots[k] = 0;
                    filled = true;
                }
            }
        }
    } while (filled);
}

// Function to check and switch numbers between adjacent plates automatically
void checkAndSwitch(vector<vector<Plate>>& matrix, size_t i, size_t j) {
    cout << "Running the switch system for plate at (" << i << ", " << j << ")\n";

    auto switchIfSame = [&](size_t i, size_t j, size_t i1, size_t j1) {
        bool switched;
        do {
            switched = false;
            for (size_t k = 0; k < 6; ++k) {
                if (matrix[i][j].slots[k] != 0) {
                    for (size_t l = 0; l < 6; ++l) {
                        if (matrix[i][j].slots[k] == matrix[i1][j1].slots[l] && matrix[i1][j1].slots[l] != 0) {
                            cout << "Filling slot in (" << i1 << ", " << j1 << ") with value from (" << i << ", " << j << ")\n";
                            for (size_t m = 0; m < 6; ++m) {
                                if (matrix[i1][j1].slots[m] == 0) {
                                    matrix[i1][j1].slots[m] = matrix[i][j].slots[k];
                                    matrix[i][j].slots[k] = 0;
                                    switched = true;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        } while (switched);
    };

    // Checking the right plate
    if (j + 1 < 4) {
        switchIfSame(i, j, i, j + 1);
    }

    // Checking the left plate
    if (j > 0) {
        switchIfSame(i, j, i, j - 1);
    }

    // Checking the bottom plate
    if (i + 1 < 5) {
        switchIfSame(i, j, i + 1, j);
    }

    // Checking the top plate
    if (i > 0) {
        switchIfSame(i, j, i - 1, j);
    }
}

// Function to generate a new plate with random values
Plate generatePlate() {
    Plate plate;
    size_t emptySlots = 0;
    for (size_t i = 0; i < plate.slots.size(); ++i) {
        plate.slots[i] = rand() % 7;
        if (plate.slots[i] == 0) emptySlots++;
    }
    // Ensure at least two zeros
    while (emptySlots < 2) {
        size_t pos = rand() % 6;
        if (plate.slots[pos] != 0) {
            plate.slots[pos] = 0;
            emptySlots++;
        }
    }
    return plate;
}

// Function to display the matrix
void displayMatrix(const vector<vector<Plate>>& matrix) {
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "[";
            for (size_t k = 0; k < 6; ++k) {
                cout << matrix[i][j].slots[k] << " ";
            }
            cout << "] ";
        }
        cout << "\n";
    }
}

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to check if all slots in a plate are of the same type
bool isFullPlate(const Plate& plate) {
    int type = plate.slots[0];
    if (type == 0) return false;
    for (int slot : plate.slots) {
        if (slot != type) return false;
    }
    return true;
}

// Function to check for a loss condition
bool isFullMatrix(const vector<vector<Plate>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& plate : row) {
            for (int slot : plate.slots) {
                if (slot == 0) return false;
            }
        }
    }
    return true;
}

// Function to reset the game
void resetGame(vector<vector<Plate>>& matrix) {
    for (auto& row : matrix) {
        for (auto& plate : row) {
            plate = Plate();
        }
    }
}

int main() {
    vector<vector<Plate>> matrix(5, vector<Plate>(4));
    int score = 0;

    // Displaying the table coordinates
    cout << "Table Coordinates:\n";
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "(" << i << ", " << j << ") ";
        }
        cout << endl;
    }

    // Initialize random number generator
    srand(static_cast<unsigned>(time(0)));
    std::random_device rd;
    std::mt19937 g(rd());

    while (true) {
        // Generate a new plate
        Plate plate = generatePlate();

        // Displaying the contents of the new plate
        cout << "\nNew Plate contents:\n";
        for (size_t i = 0; i < plate.slots.size(); ++i) {
            cout << plate.slots[i] << " ";
        }
        cout << "\n";

        // Requesting the user to enter table indices
        size_t i, j;
        cout << "Enter the table indices (i and j) you want to fill (or -1 to exit): ";
        cin >> i >> j;

        // Check for exit condition
        if (i == -1 || j == -1) {
            break;
        }

        // Validating the indices
        if (i >= 5 || j >= 4) {
            cout << "Invalid indices, please enter indices within range: 0 <= i < 5 and 0 <= j < 4\n";
            continue;
        }

        // Displaying the matrix contents before modification
        cout << "\nMatrix contents before modification:\n";
        displayMatrix(matrix);

        // Inserting the contents of the plate into matrix[i][j]
        matrix[i][j] = plate;

        // Filling empty slots from adjacent plates
        fillEmptySlots(matrix, i, j);

        // Check and switch numbers with adjacent plates automatically
        checkAndSwitch(matrix, i, j);

        // Check for full plate
        if (isFullPlate(matrix[i][j])) {
            cout << "Plate at (" << i << ", " << j << ") is full of one type! Clearing plate and adding points.\n";
            matrix[i][j] = Plate();
            score++;
            cout << "Current score: " << score << "\n";
        }

        // Check for loss condition
        if (isFullMatrix(matrix)) {
            cout << "All coordinates are full! You lose.\n";
            char choice;
            cout << "Do you want to restart the game? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                resetGame(matrix);
                score = 0;
                clearScreen();
                continue;
            } else {
                break;
            }
        }

        // Clear the screen before showing the modified state
        clearScreen();

        // Displaying the matrix contents after modification
        cout << "\nMatrix contents after modification:\n";
        displayMatrix(matrix);
    }

    return 0;
}
