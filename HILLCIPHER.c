#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 3
#define MAX_TEXT_LENGTH 100
#define MAX_TRIGRAMS ((MAX_TEXT_LENGTH / SIZE) + 1)

// Function prototypes
int gcd(int a, int b);
int modInverse(int a, int m);
void readMatrix(int matrix[SIZE][SIZE]);
void printMatrix(int matrix[SIZE][SIZE]);
int determinant(int matrix[SIZE][SIZE]);
void inverseMatrix(int matrix[SIZE][SIZE], int inverse[SIZE][SIZE]);
void encryptTrigram(char trigram[], int keyMatrix[SIZE][SIZE], char encryptedTrigram[]);
void decryptTrigram(char encryptedTrigram[], int keyMatrix[SIZE][SIZE], char decryptedTrigram[]);
void processText(char text[], int keyMatrix[SIZE][SIZE]);

int main() {
    int keyMatrix[SIZE][SIZE];
    int keyMatrixInverse[SIZE][SIZE];

    // Read the key matrix from the user
    printf("Enter the 3x3 key matrix:\n");
    readMatrix(keyMatrix);

    // Calculate the determinant of the key matrix
    int det = determinant(keyMatrix);

    // Check if the determinant is invertible modulo 26
    if (gcd(det, 26) != 1) {
        printf("The key matrix is not invertible modulo 26.\n");
        return 1;
    }

    // Calculate the inverse of the key matrix
    inverseMatrix(keyMatrix, keyMatrixInverse);

    // Read the text from the user
    char text[MAX_TEXT_LENGTH];
    printf("Enter a sentence (uppercase letters and spaces only):\n");
    scanf(" %[^\n]", text); // Read the entire line including spaces

    // Process the text to encrypt and decrypt
    processText(text, keyMatrix);

    return 0;
}

// Function to compute GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // No modular inverse
}

// Function to calculate determinant of a 3x3 matrix
int determinant(int matrix[SIZE][SIZE]) {
    int det = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
              matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
              matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return (det % 26 + 26) % 26; // Ensure the determinant is positive modulo 26
}

// Function to compute the inverse of a 3x3 matrix
void inverseMatrix(int matrix[SIZE][SIZE], int inverse[SIZE][SIZE]) {
    int det = determinant(matrix);
    int detInv = modInverse(det, 26);

    if (detInv == -1) {
        printf("Modular inverse of determinant does not exist.\n");
        exit(1);
    }

    int adj[SIZE][SIZE] = {
        {
            matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1],
            matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2],
            matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]
        },
        {
            matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2],
            matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0],
            matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]
        },
        {
            matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0],
            matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1],
            matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]
        }
    };

    // Apply modulo 26 and multiply by the modular inverse of the determinant
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            adj[i][j] = (adj[i][j] % 26 + 26) % 26; // Ensure positive values
            inverse[i][j] = (adj[i][j] * detInv) % 26;
        }
    }
}

// Function to encrypt a trigram
void encryptTrigram(char trigram[], int keyMatrix[SIZE][SIZE], char encryptedTrigram[]) {
    int input[SIZE] = {0};
    int output[SIZE] = {0};
    
    for (int i = 0; i < SIZE; i++) {
        input[i] = trigram[i] - 'A';
    }

    for (int i = 0; i < SIZE; i++) {
        output[i] = 0;
        for (int j = 0; j < SIZE; j++) {
            output[i] += keyMatrix[i][j] * input[j];
        }
        output[i] %= 26; // Wrap around if value exceeds 25
    }

    for (int i = 0; i < SIZE; i++) {
        encryptedTrigram[i] = output[i] + 'A';
    }
}

// Function to decrypt a trigram
void decryptTrigram(char encryptedTrigram[], int keyMatrix[SIZE][SIZE], char decryptedTrigram[]) {
    int input[SIZE] = {0};
    int output[SIZE] = {0};
    
    for (int i = 0; i < SIZE; i++) {
        input[i] = encryptedTrigram[i] - 'A';
    }

    for (int i = 0; i < SIZE; i++) {
        output[i] = 0;
        for (int j = 0; j < SIZE; j++) {
            output[i] += keyMatrix[i][j] * input[j];
        }
        output[i] %= 26; // Wrap around if value exceeds 25
    }

    for (int i = 0; i < SIZE; i++) {
        decryptedTrigram[i] = output[i] + 'A';
    }
}

// Function to process the text into trigrams
void processText(char text[], int keyMatrix[SIZE][SIZE]) {
    int len = strlen(text);
    char trigram[SIZE + 1];
    char encryptedTrigram[SIZE + 1];
    char decryptedTrigram[SIZE + 1];
    
    char encryptedText[MAX_TEXT_LENGTH] = {0};
    char decryptedText[MAX_TEXT_LENGTH] = {0};
    
    int encryptedTextIndex = 0;
    int decryptedTextIndex = 0;

    // Process the text into trigrams
    for (int i = 0; i < len; i += SIZE) {
        // Fill trigram with characters or pad with 'X'
        for (int j = 0; j < SIZE; j++) {
            if (i + j < len && text[i + j] != ' ') {
                trigram[j] = text[i + j];
            } else {
                trigram[j] = 'X';
            }
        }
        trigram[SIZE] = '\0'; // Null-terminate the string

        // Encrypt the trigram
        encryptTrigram(trigram, keyMatrix, encryptedTrigram);
        encryptedTrigram[SIZE] = '\0'; // Null-terminate the string
        
        // Append encrypted trigram to the encrypted text
        for (int j = 0; j < SIZE; j++) {
            encryptedText[encryptedTextIndex++] = encryptedTrigram[j];
        }

        // Decrypt the trigram
        int keyMatrixInverse[SIZE][SIZE];
        inverseMatrix(keyMatrix, keyMatrixInverse);
        decryptTrigram(encryptedTrigram, keyMatrixInverse, decryptedTrigram);
        decryptedTrigram[SIZE] = '\0'; // Null-terminate the string

        // Append decrypted trigram to the decrypted text
        for (int j = 0; j < SIZE; j++) {
            decryptedText[decryptedTextIndex++] = decryptedTrigram[j];
        }
    }

    // Print the results
    printf("Encrypted text: ");
    for (int i = 0; i < encryptedTextIndex; i++) {
        printf("%c", encryptedText[i]);
    }
    printf("\n");

    printf("Decrypted text: ");
    for (int i = 0; i < decryptedTextIndex; i++) {
        printf("%c", decryptedText[i]);
    }
    printf("\n");
}

// Function to read a 3x3 matrix from the user
void readMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("Enter element (%d, %d): ", i + 1, j + 1);
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function to print a 3x3 matrix
void printMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

