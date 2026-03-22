#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void criarHomebrew(char* nome) {
    char path[256];
    snprintf(path, sizeof(path), "sdmc:/3ds/%s", nome);

    mkdir(path, 0777);

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/main.c", path);

    FILE* f = fopen(filePath, "w");

    if (f != NULL) {
        fprintf(f,
            "#include <3ds.h>\n"
            "#include <stdio.h>\n\n"
            "int main() {\n"
            "    gfxInitDefault();\n"
            "    consoleInit(GFX_TOP, NULL);\n\n"
            "    printf(\"Hello from %s!\\n\");\n\n"
            "    while (aptMainLoop()) {\n"
            "        hidScanInput();\n"
            "        if (hidKeysDown() & KEY_START) break;\n"
            "        gfxFlushBuffers();\n"
            "        gfxSwapBuffers();\n"
            "        gspWaitForVBlank();\n"
            "    }\n\n"
            "    gfxExit();\n"
            "    return 0;\n"
            "}",
            nome
        );
        fclose(f);
    }
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    swkbdInit();

    char nome[50] = "";

    printf("Homebrew Builder\n\n");
    printf("Pressione A para criar\n");

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_A) {
            SwkbdState swkbd;
            swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, 20);

            if (swkbdInputText(&swkbd, nome, sizeof(nome)) == SWKBD_BUTTON_CONFIRM) {
                criarHomebrew(nome);
                printf("\nCriado com sucesso!");
            }
        }

        if (kDown & KEY_START) break;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
