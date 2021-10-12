#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cracker 1
#define coder 2

/**
 * Procedimiento que borra los contenidos de la pantalla. Ejecuta `clear` o `cls` dependiendo del sistema operativo.\n\n
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
 * Procedimiento que escribe el mensaje "Input any character to continue: " y espera la respuesta por teclado.
 * Útil para dar tiempo al usuario a ver un mensaje en la pantalla antes de borrarla y continuar el juego
 */
void pressToContinue() {
    char dummy[600];
    printf("Input any character to continue: ");
    scanf("%s", dummy);
}

/**
 * Procedimiento que muestra una ayuda básica que sirve en ambos modos para darse a una idea de cómo de ve el codigo a crear/adivinar
 */
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

/**
 * Función que valida que el código ingresado por el jugador es válido, ya sea para adivinar o crear.
 * @param secretCode: Apuntador a un char array que contiene el input con un código del juego.
 * @return 1 para indicar que el código es válida y 0 para indicar que no lo es.
 */
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

/**
 * Analiza un string para comprobar si el usuario eligió que desea el modo cracker o coder
 * @param gameMode: Apuntador a un char array que contiene el input con el modo de juego.
 * @return cracker (1) para indicar que el modo de juego es de cracker o coder (0).
 */
short getPlayerNumberFromString(const char* gameMode) {
    if (!strcmp(gameMode, "crack")) return cracker;
    else if (!strcmp(gameMode, "code")) return coder;
    else return 0;
}

/**
 * Función para validar que el string ingresado por el ususario para indicar el modo de juego sea de tamaño 4 o 5,
 * si no es del tamaño esperado no vale la pena buscar si es igual a cracker o coder
 * @param gameMode: Apuntador a un char array que contiene el input con el modo de juego.
 * @return 1 para indicar que es válido y 0 para indicar que no lo es
 */
short validGameModeInputSize(const char* gameMode) {
    size_t string_len = strlen(gameMode);
    if (string_len == 5 || string_len == 4) return 1;
    return 0;
}

/**
 * Función que verifica si el tamaño del string ingresado por el usuario es del tamaño esperado,
 * si lo es entonces busca en dicho strong si el usuario eligió coder o cracker.\n
 * Si no lo es muestra un mensaje de error-ayuda
 * @param playerNumber
 * @return
 */
short parsePlayerNumberString(const char* playerNumber) {

    if (validGameModeInputSize(playerNumber))
        return getPlayerNumberFromString(playerNumber);
    else
        puts("Error, you need to input 'crack' or 'code'.");

    return 0;
}

/**
 * Ejecuta todos los procedimientos del modo cracker
 */
void crackerMode() {
    char guessCode[600];

    puts("Hint: Write 'help' to display some tips to crack the code.");
    do {
        printf("Player, input your guess [e.g. '****']: ");
        scanf("%s", guessCode);
    } while (!validSecretCode((const char *) &guessCode));
}

/**
 * Ejecuta todos los procedimientos del modo coder
 */
void coderMode() {
    char secretCode[600];

    puts("Hint: Write 'help' to display some tips to formulate the code.");
    do {
        printf("Player, input the secret code [e.g. '****']: ");
        scanf("%s", secretCode);
    } while (!validSecretCode((const char *) &secretCode));

    printf("Ok, the secret code is: %s \n"
           "You should write it down or memorize it ;)\n",
           secretCode );
}


int main() {

    // Reservamos espacio de sobra para poder ingresar strings grandes evitando segmentation faults


    char gameMode[600];
    short gameModeSelected;

    clearScreen();
    puts("Welcome to MasterMinds");
    pressToContinue();

    printf("Alright, lets start. ");
    do {
        printf("Do you want to crack or code? [crack|code]: ");
        scanf("%s", gameMode);
        gameModeSelected = parsePlayerNumberString(gameMode);
    } while (!gameModeSelected);

    if (gameModeSelected == cracker) {
        puts("Good, you will crack the code the AI will make.");
        crackerMode();
    } else {
        puts("Good, you will code and the AI will try to guess.");
        coderMode();
    }

    return 0;
}
