#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Borra los contenidos de la pantalla. Ejecuta `clear` o `cls` dependiendo del sistema operativo.\n\n
 * Recuperado de https://stackoverflow.com/a/36253316/14916353
 */
void clearScreen(){
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

/**
 * Escribe el mensaje "Input any character to continue: " y espera la respuesta por teclado.
 * Útil para dar tiempo al usuario a ver un mensaje en la pantalla antes de borrarla y continuar el juego
 */
void pressToContinue() {
    char dummy[600];
    printf("Input any character to continue: ");
    scanf("%s", dummy);
}

void showValidPasswordRules() {
    clearScreen();
    printf(
            "Password rules:\n\n"
            "    - The password can contain six 'pins': *, +, &, %%, #, @ \n"
            "    - It needs to have a length of 4.\n"
            "    - You can repeat any pin as many times as you wish\n"
            "    - You may want to make it hard for the other player ;)\n"
            "    - Valid examples are: \n"
            "        ****\n"
            "        #*++\n"
            "        ##*%%\n");
    pressToContinue();
}

short validSecretCode(const char* secretCode) {
    char pins[] = {'*', '+', '&', '%', '#', '@'};

    size_t string_len = strlen(secretCode);

    if (strcmp(secretCode, "h") == 0 || strcmp(secretCode, "help") == 0) {
        showValidPasswordRules();
        return 0;
    }

    if (string_len > 4 || string_len < 4) return 0;

    short int validChars = 0;
    for (short int i=0; i<4; i++) {
        for (short int j=0; j<6; j++) {
            if (secretCode[i] == pins[j])  {
                validChars++;
                break;
            }
        }
    }

    if (validChars != 4) return 0;

    return 1;

}

short getPlayerNumberFromString(const char* playerNumber) {
    switch (playerNumber[0]) {
        case '1': return 1;
        case '2': return 2;
    }
    return 0;
}

short validPlayerNumber(const char* playerNumber) {
    size_t string_len = strlen(playerNumber);

    if (string_len == 1 && (playerNumber[0] == '1' || playerNumber[0] == '2'))
        return getPlayerNumberFromString(playerNumber);

    return 0;

}

short parsePlayerNumberString(const char* playerNumber) {
    if (validPlayerNumber(playerNumber))
        return getPlayerNumberFromString(playerNumber);
    else
        printf("Error, you need to input '1' or '2' to indicate which player will crack the code.\n");

    return 0;
}

int main() {

    // Reservamos espacio de sobra para poder ingresar strings grandes evitando segmentation faults
    char secretCode[600];
    char guessCode[600];
    char crackerPlayer[600];
    short cracker;
    short coder;

    clearScreen();
    puts("Welcome to MasterMinds");
    pressToContinue();

    printf("Alright, lets start. ");
    do {
        printf("Which player is cracking the code? [1|2]: ");
        scanf("%s", crackerPlayer);
        cracker = parsePlayerNumberString(crackerPlayer);
    } while (!cracker);

    coder = cracker == 1 ? 2 : 1;
    printf("Good, player %hi will crack and player %hi will code.\n" , cracker, coder);

    pressToContinue();
    clearScreen();

    puts("Hint: Write 'help' to display some tips to formulate the code.");
    do {
        printf("Player %hi, input the secret code [e.g. '****']: ", coder);
        scanf("%s", secretCode);
    } while (!validSecretCode((const char *) &secretCode));

    printf("Ok, the secret code is: %s \n"
           "You should write it down or memorize it ;)\n",
           secretCode );


    pressToContinue();
    clearScreen();

    puts("Hint: Write 'help' to display some tips to crack the code.");
    do {
        printf("Player %hi, input your guess [e.g. '****']: ", cracker);
        scanf("%s", guessCode);
    } while (!validSecretCode((const char *) &guessCode));

    return 0;
}
