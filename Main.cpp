#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine

using namespace std;

// Function to fill empty slots from adjacent plates
void fillEmptySlots(vector<vector<vector<int>>>& matrix, size_t i, size_t j) {
    cout << "Filling empty slots for plate at (" << i << ", " << j << ")\n";
    // Checking the right plate
    if (j + 1 < 4) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j][k] == 0 && matrix[i][j + 1][k] != 0) {
                matrix[i][j][k] = matrix[i][j + 1][k];
                matrix[i][j + 1][k] = 0;
            }
        }
    }

    // Checking the left plate
    if (j > 0) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j][k] == 0 && matrix[i][j - 1][k] != 0) {
                matrix[i][j][k] = matrix[i][j - 1][k];
                matrix[i][j - 1][k] = 0;
            }
        }
    }

    // Checking the bottom plate
    if (i + 1 < 5) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j][k] == 0 && matrix[i + 1][j][k] != 0) {
                matrix[i][j][k] = matrix[i + 1][j][k];
                matrix[i + 1][j][k] = 0;
            }
        }
    }

    // Checking the top plate
    if (i > 0) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j][k] == 0 && matrix[i - 1][j][k] != 0) {
                matrix[i][j][k] = matrix[i - 1][j][k];
                matrix[i - 1][j][k] = 0;
            }
        }
    }
}

// Function to check and switch numbers between adjacent plates automatically
void checkAndSwitch(vector<vector<vector<int>>>& matrix, size_t i, size_t j) {
    cout << "Running the switch system for plate at (" << i << ", " << j << ")\n";

    auto switchIfSame = [&](size_t i, size_t j, size_t i1, size_t j1) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j][k] != 0) {
                for (size_t l = 0; l < 6; ++l) {
                    if (matrix[i][j][k] == matrix[i1][j1][l] && matrix[i1][j1][l] != 0) {
                        cout << "Filling slot in (" << i1 << ", " << j1 << ") with value from (" << i << ", " << j << ")\n";
                        for (size_t m = 0; m < 6; ++m) {
                            if (matrix[i1][j1][m] == 0) {
                                matrix[i1][j1][m] = matrix[i][j][k];
                                matrix[i][j][k] = 0;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
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

int main() {
    vector<vector<vector<int>>> matrix(5, vector<vector<int>>(4, vector<int>(6)));

    // Accessing and printing elements
    cout << "Initial Matrix:\n";
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "[";
            for (size_t k = 0; k < 6; ++k) {
                cout << matrix[i][j][k] << " ";
            }
            cout << "] ";
        }
        cout << endl;
    }

    // Initialize random number generator
    srand(static_cast<unsigned>(time(0)));
    std::random_device rd;
    std::mt19937 g(rd());

    size_t numPlates;
    cout << "Enter the number of plates you want to generate and fill: ";
    cin >> numPlates;

    for (size_t n = 0; n < numPlates; ++n) {
        // Creating vector (plate) with size 6 and filling it with random numbers between 0 and 6 (0 represents an empty slot)
        vector<int> plate(6);
        size_t emptySlots = 0;
        for (size_t i = 0; i < plate.size(); ++i) {
            plate[i] = rand() % 7;
            if (plate[i] == 0) emptySlots++;
        }
        // Ensure at least two zeros
        while (emptySlots < 2) {
            size_t pos = rand() % 6;
            if (plate[pos] != 0) {
                plate[pos] = 0;
                emptySlots++;
            }
        }

        // Displaying the contents of the plate vector
        cout << "\nPlate " << n + 1 << " contents:\n";
        for (size_t i = 0; i < plate.size(); ++i) {
            cout << plate[i] << " ";
        }
        cout << "\n";

        // Requesting the user to enter table indices
        size_t i, j;
        cout << "Enter the table indices (i and j) you want to fill: ";
        cin >> i >> j;

        // Validating the indices
        if (i >= 5 || j >= 4) {
            cout << "Invalid indices, please enter indices within range: 0 <= i < 5 and 0 <= j < 4\n";
            --n; // Repeat this iteration
            continue;
        }

        // Inserting the contents of the plate into matrix[i][j]
        matrix[i][j] = plate;

        // Filling empty slots from adjacent plates
        fillEmptySlots(matrix, i, j);

        // Check and switch numbers with adjacent plates automatically
        checkAndSwitch(matrix, i, j);
    }

    // Displaying the matrix contents after modification
    cout << "\nMatrix contents after modification:\n";
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "[";
            for (size_t k = 0; k < 6; ++k) {
                cout << matrix[i][j][k] << " ";
            }
            cout << "] ";
        }
        cout << "\n";
    }

    return 0;
}
