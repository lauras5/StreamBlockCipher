#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Padding input data (if required) to 128 bits
void padData(char **data, size_t *data_len) {
    printf("here");
    size_t padded_len = ((*data_len + 15) / 16) * 16;
    char *padded_data = (char *)malloc(padded_len);
    if (!padded_data) {
        perror("Memory allocation error");
        exit(1);
    }
    memcpy(padded_data, *data, *data_len);

    for (size_t i = *data_len; i < padded_len; ++i) {
        padded_data[i] = 0x81;
    }

    free(*data);
    *data = padded_data;
    *data_len = padded_len;
}

// Block cipher encryption
void blockCipherEncrypt(char *data, size_t data_len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < data_len; ++i) {
        data[i] ^= key[i % key_len];
    }

    size_t start = 0;
    size_t end = data_len - 1;
    size_t keyIndex = 0;

    while (start < end) {
        if (key[keyIndex] % 2 == 1) {
            char temp = data[start];
            data[start] = data[end];
            data[end] = temp;
        }

        ++start;
        --end;
        ++keyIndex;

        if (keyIndex == key_len) {
            keyIndex = 0;
        }
    }
}

// Block cipher decryption
void blockCipherDecrypt(char *data, size_t data_len, const char *key) {
    size_t start = 0;
    size_t end = data_len - 1;
    size_t key_len = strlen(key);
    size_t keyIndex = key_len - 1;

    while (start < end) {
        if (key[keyIndex] % 2 == 1) {
            char temp = data[start];
            data[start] = data[end];
            data[end] = temp;
        }
        ++start;
        --end;
        --keyIndex;

        if (keyIndex == (size_t)-1) {
            keyIndex = key_len - 1;
        }
    }

    for (size_t i = 0; i < data_len; ++i) {
        data[i] ^= key[i % key_len];
    }
}

// Stream cypher encryption and decryption
void streamCipher(char *data, size_t data_len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < data_len; ++i) {
        data[i] ^= key[i % key_len];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <B|S> <input_file> <output_file> <key_file> <E|D>\n", argv[0]);
        return 1;
    }

    char mode = argv[1][0];
    const char *inputFileName = argv[2];
    const char *outputFileName = argv[3];
    const char *keyFileName = argv[4];
    char operation = argv[5][0];

    if (mode != 'B' && mode != 'S') {
        fprintf(stderr, "Invalid mode. Use 'B' for block cipher or 'S' for stream cipher.\n");
        return 1;
    }

    FILE *keyFile = fopen(keyFileName, "r");
    if (!keyFile) {
        perror("Key file not found");
        return 1;
    }

    char key[128]; // Assuming a maximum key length of 128 characters
    if (fgets(key, sizeof(key), keyFile) == NULL) {
        perror("Error reading key from the key file");
        fclose(keyFile);
        return 1;
    }

    fclose(keyFile);

    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Input file not found");
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    size_t inputFileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    char *inputData = (char *)malloc(inputFileSize);
    if (!inputData) {
        perror("Memory allocation error");
        fclose(inputFile);
        return 1;
    }

    size_t bytes_read = fread(inputData, 1, inputFileSize, inputFile);
    fclose(inputFile);

    if (bytes_read != inputFileSize) {
        perror("Error reading input file");
        free(inputData);
        return 1;
    }

    if (operation == 'E') {
        if (mode == 'B') {
            padData(&inputData, &inputFileSize);
            blockCipherEncrypt(inputData, inputFileSize, key);
        } else {
            streamCipher(inputData, inputFileSize, key);
        }
    } else if (operation == 'D') {
        if (mode == 'B') {
            blockCipherDecrypt(inputData, inputFileSize, key);
        } else {
            streamCipher(inputData, inputFileSize, key);
        }
    } else {
        fprintf(stderr, "Invalid operation. Use 'E' for encryption or 'D' for decryption.\n");
        free(inputData);
        return 1;
    }

    // Open and write the output file
    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        free(inputData);
        return 1;
    }

    size_t bytes_written = fwrite(inputData, 1, inputFileSize, outputFile);
    fclose(outputFile);

    free(inputData);

    if (bytes_written != inputFileSize) {
        perror("Error writing output file");
        return 1;
    }

    return 0;
}

