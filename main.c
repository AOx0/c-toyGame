#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tgmath.h>

#define CRACKER 1
#define CODER 2

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
    printf("\nInput any character to continue: ");
    scanf("%s", dummy);
}

/**
 * Procedimiento que muestra una ayuda básica que sirve en ambos modos para darse a una idea de cómo de ve el código a crear/adivinar
 */
void showValidPasswordRules() {
    printf(
            "------------\nPassword rules:\n\n"
            "    - The password can contain six 'pins': *, +, &, %%, #, @ \n"
            "    - It needs to have a length of 4.\n"
            "    - You can repeat any pin as many times as you wish\n"
            "    - You may want to make it hard for the other player ;)\n"
            "    - Valid examples are: \n"
            "        ****\n"
            "        #*++\n"
            "        ##*%%\n------------\n");
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

    if (strcmp(secretCode, "p") == 0 || strcmp(secretCode, "pins") == 0) {
        puts("* + & % # @");
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
 * Función que valida que el código ingresado por el jugador es válido, ya sea para adivinar o crear.
 * @param secretCode: Apuntador a un char array que contiene el input con un código del juego.
 * @return 1 para indicar que el código es válida y 0 para indicar que no lo es.
 */
short validCorrectPinsCode(const char* pinsCode) {
    char pins[] = {'+', '-'};

    size_t string_len = strlen(pinsCode);


    if (string_len != 4) return 0;

    short int validChars = 0;
    for (short int i=0; i<4; i++) {
        for (short int j=0; j<2; j++) {
            if (pinsCode[i] == pins[j])  {
                validChars++;
                break;
            }
        }
    }

    if (validChars != 4) return 0;

    return 1;

}

/**
 * Verifica que el código ingresado por el jugador para indicar a la IA el número correcto de pines sea valido
 * @param pinsCode: Arreglo de caracteres con el código.
 * @return 1 si es válido y 0 si no lo es
 */
short parsePinsCode(const char* pinsCode) {
    short correctFormat = validCorrectPinsCode(pinsCode);
    if (!correctFormat) {
        puts("Wrong format: Make sure to write the correct format: ^[+|-][+|-][+|-][+|-]$.");
        puts("    e.g. ++-- indicates the AI has guessed two pins of the code you created");
        puts("    e.g. --+- indicates the AI has guessed one pin of the code you created");
    }
    return correctFormat;
}

/**
 * Analiza un string para comprobar si el usuario eligió que desea el modo CRACKER o CODER
 * @param gameMode: Apuntador a un char array que contiene el input con el modo de juego.
 * @return CRACKER (1) para indicar que el modo de juego es de CRACKER o CODER (0).
 */
short getGameModeFromString(const char* gameMode) {
    if (!strcmp(gameMode, "crack")) return CRACKER;
    else if (!strcmp(gameMode, "code")) return CODER;
    else return 0;
}

/**
 * Función para validar que el string ingresado por el usuario para indicar el modo de juego sea de tamaño 4 o 5,
 * si no es del tamaño esperado no vale la pena buscar si es igual a CRACKER o CODER
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
 * si lo es entonces busca en dicho strong si el usuario eligió CODER o CRACKER.\n
 * Si no lo es muestra un mensaje de error-ayuda
 * @param gameMode: String que contiene el modo de juego deseado. Se espera 'crack' o 'code'
 * @return Devuelve le modo de juego (CRACKER, CODER) o 0 para error
 */
short parseGameModeString(const char* gameMode) {

    if (validGameModeInputSize(gameMode))
        return getGameModeFromString(gameMode);
    else
        puts("Error, you need to input 'crack' or 'code'.");

    return 0;
}

/**
 * Función que verifica si el código ingresado es válido.
 * @param playerNumber: String que contiene el código ingresado por el usuario
 * @return Devuelve 1 si el código es correcto o 0 si no
 */
short parseCodeString(const char* code) {

    if (validSecretCode(code))
        return 1;
    else
        puts("Error, wrong secret code format");

    return 0;
}

/**
 * Procedimiento que modifica un arreglo de caracteres vacío para que contenga un código generado aleatoriamente.\n\n
 *
 * Se usa tanto en el modo cracker, para generar el código que debe adivinar el usuario, como en el modo coder para
 * generar los intentos de la IA de adivinar la contraseña generada por el usuario.
 * @param secretCode: El arreglo como apuntador donde guardará el código generado
 */
void AIGenerateCode(char * secretCode) {
    srand(time(NULL));
    const char pins[] = {'*', '+', '&', '%', '#', '@'};

    do {
        for (short i=0; i<4; i++) {
            short pin_index = rand() % 6;
            secretCode[i] = pins[pin_index];

        }
    } while (!validSecretCode(secretCode));
}

/**
 * Para cuando mejoremos la IA haremos que intuya qué caracteres no debe usar, puede servir para eso.
 * @param secretCode: El código creado por el jugador
 * @param doNotUse: La lista de caracteres que no incluirá en la generación de la contraseña
 */
void AIGenerateCodeWithCustomPins(char * secretCode, char * doNotUse) {
    srand(time(NULL));
    const char pins[] = {'*', '+', '&', '%', '#', '@'};
    size_t len = strlen(doNotUse);

    do {
        for (short i=0; i<4; i++) {
            short pin_index = rand() % 6;
            for (int j=0; j < len; j++) {
                if (pins[pin_index] == doNotUse[j]) {
                    i--;
                    continue;
                }
            }
            secretCode[i] = pins[pin_index];

        }
    } while (!validSecretCode(secretCode));
}

/**
 * Función que obtiene el número de coincidencias de caracteres entre dos strings. Se usa para saber el número de pins
 * correctos que el usuario ingresa en el modo cracker.
 * @param secretCode: String con el código generado por la IA
 * @param codeGuess: String que contiene el código que ingresó el usuario para adivinar.
 * @return: short que contiene la cuenta de coincidencias.
 */
short AICountCorrectChars(const char * secretCode, const char * codeGuess) {
    short valid_count = 0;
    for (short i=0; i<4; i++) {
        if (secretCode[i] == codeGuess[i]) {
            valid_count++;
        }
    }
    return valid_count;
}

/**
 * Función que muestra con el formato ^[+|-][+|-][+|-][+|-]$ con caracteres ordenados de forma aleatoria
 * al usuario el número correcto de pins en su intento.\n\n
 * También devuelve el número de pines bien colocados.
 * @param secretCode: String con el código generado por la IA
 * @param codeGuess: String que contiene el código que ingresó el usuario para adivinar.
 * @return Short que indica el número de coincidencias.
 */
short AIShowValidPins(char * secretCode, char * codeGuess) {
    char listOfPinsToDisplay[5] = "----";
    int usedIndexes[4] = {4,4,4,4};

    short correct = AICountCorrectChars(secretCode, codeGuess);

    if (strcmp(secretCode, codeGuess) == 0) {
        puts("Correct, you win!");
        return 4;
    }

    for (int i=0; i<correct; i++) {
        listOfPinsToDisplay[i] = '+';
    }

    srand(time(NULL));

    for (short i=0; i<4; i++) {
        short pin_index = rand() % 4;

        short used = 0;
        for (int j=0; j<4; j++ ) {
            if (pin_index == usedIndexes[j]) {
                used = 1;
            }
        }
        if (used) {
            i--;
            continue;
        } else {
            printf("%c", listOfPinsToDisplay[pin_index]);
            usedIndexes[i] = pin_index;
        }
    }
    printf("\n");

    return correct;
}

/**
 * Ejecuta todos los procedimientos del modo CRACKER
 */
void crackerMode() {
    char guessCode[600];
    char secretCode[5] = {};
    char exampleCode[5] = {};
    int correctSum = 0;
    int lastCorrect;

    AIGenerateCode(secretCode);

    puts("The AI has generated the code. Time for you to guess it.");

    puts("Hint: Write 'help' to display some tips to crack the code.");
    puts("Hint: Write 'pins' to display the valid pins (chars) that exist in the game.");
    puts("Hint: You have 10 guesses, choose your pins wisely");
    puts("");

    for (short try=0; try<10; try++) {
        do {
            AIGenerateCode(exampleCode);
            printf("Player, input your guess [e.g. '%s']: ", exampleCode);
            scanf("%s", guessCode);
        } while (!parseCodeString((const char *) &guessCode));

        lastCorrect = AIShowValidPins(secretCode, guessCode);
        correctSum += lastCorrect;
        if (lastCorrect == 4) return;
    }

    int average = round(correctSum/10.0) + 1;

    switch (average) {
        case 1:
            puts("Not even close LMAO");
            printf("The code was: %s\n", secretCode);
            return;
        case 2:
            puts("Not this time");
            printf("The code was: %s\n", secretCode);
            return;
        case 3:
            puts("Ah, almost");
            printf("The code was: %s\n", secretCode);
            return;
        default:
            puts("You lose :/");
            printf("The code was: %s\n", secretCode);
            return;
    }
}



/**
 * Ejecuta todos los procedimientos del modo CODER
 */
void coderMode() {
    char secretCode[600];
    char guessCode[5];
    char correctPins[600];


    puts("Hint: Write 'help' to display some tips to formulate the code.");
    puts("Hint: Write 'pins' to display the valid pins (chars) that exist in the game.");

    do {
        printf("Player, input the secret code for the AI to guess [e.g. '****']: ");
        scanf("%s", secretCode);
    } while (!parseCodeString((const char *) &secretCode));

    printf("Ok, the secret code is: %s\n", secretCode);
    printf("Now the AI will try to guess\n\n");


    for (short try=0; try<10; try++) {
        short correctNumberOfPins = 0;

        AIGenerateCode(guessCode);
        printf("The AI guessed: %s\n", guessCode );

        do {
            printf("Input the correct/incorrect format of its guess [e.g. +-++]: ");
            scanf("%s", correctPins);
        } while (!parsePinsCode(correctPins));


        for (int i=0; i<4; i++) {
            if (correctPins[i] == '+') correctNumberOfPins++;
        }

        if (correctNumberOfPins == 4) {
            puts("The AI wins!");
            return;
        }
    }

    puts("You win! (AI is trash tbh)");


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
        gameModeSelected = parseGameModeString(gameMode);
    } while (!gameModeSelected);

    clearScreen();

    if (gameModeSelected == CRACKER) {
        printf("Good, you will crack the code the AI will make.\n\n");
        printf(
                "The AI will output the number of correct pins each time:\n"
                "    +  1 correct\n"
                "    -  1 incorrect\n"
                "\ne.g. '--++' Means that you have two correct pins.\n\n"
                "The position of the correct/incorrect output pins is random.\n"
                "Thus, '--++' does not indicate if the correct pins are the last two.\n"
                "It informs you have two correct pins at any position within your guess code.\n");
        puts("");
        pressToContinue();
        clearScreen();
        crackerMode();
    } else {
        puts("Good, you will code and the AI will try to guess.");
        pressToContinue();
        clearScreen();
        coderMode();
    }


    return 0;
}
