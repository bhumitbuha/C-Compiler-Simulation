#include "Step2Reader.h"
#include <string.h>

BufferPointer grevityReaderCreate(grevity_intg size) {
    BufferPointer readerPointer = NULL;

    if (size <= 0) {
        return NULL;
    }

    readerPointer = (BufferPointer)calloc(1, sizeof(struct Buffer));
    if (readerPointer == NULL) {
        return NULL;
    }

    grevity_strg content = (grevity_strg)malloc(size);
    if (content == NULL) {
        free(readerPointer);
        return NULL;
    }

    readerPointer->content = content;
    readerPointer->size = size;

    readerPointer->flags.isEmpty = 1;
    readerPointer->flags.isFull = 0;
    readerPointer->flags.isRead = 0;
    readerPointer->flags.isMoved = 0;

    readerPointer->position.read = 0;
    readerPointer->position.wrte = 0;
    readerPointer->position.mark = 0;

    memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
    readerPointer->numReaderErrors = 0;
    readerPointer->checkSum = 0;

    return readerPointer;
}

BufferPointer grevityReaderAddChar(BufferPointer const readerPointer, grevity_char ch) {
    grevity_strg tempReader = NULL;
    grevity_intg newSize = 0;

    if (readerPointer == NULL) return NULL;

    if (readerPointer->position.wrte < readerPointer->size) {
        readerPointer->content[readerPointer->position.wrte++] = ch;
        readerPointer->flags.isEmpty = 0;
    } else {
        newSize = readerPointer->size * 2;
        if (newSize <= 0 || newSize > READER_MAX_SIZE) return NULL;

        tempReader = (grevity_strg)realloc(readerPointer->content, newSize);
        if (tempReader == NULL) return NULL;

        if (tempReader != readerPointer->content) readerPointer->flags.isMoved = 1;

        readerPointer->content = tempReader;
        readerPointer->size = newSize;
        readerPointer->content[readerPointer->position.wrte++] = ch;
        readerPointer->flags.isEmpty = 0;
    }

    readerPointer->flags.isFull = (readerPointer->position.wrte >= readerPointer->size);

    if ((unsigned char)ch < NCHAR) {
        readerPointer->histogram[(unsigned char)ch]++;
    } else {
        readerPointer->numReaderErrors++;
    }

    readerPointer->checkSum = (readerPointer->checkSum + (unsigned char)ch) % 256;

    return readerPointer;
}

grevity_boln grevityReaderClear(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;

    readerPointer->position.read = 0;
    readerPointer->position.wrte = 0;
    readerPointer->flags.isEmpty = 1;
    readerPointer->flags.isFull = 0;
    readerPointer->numReaderErrors = 0;
    memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
    readerPointer->checkSum = 0;

    return GREVITY_TRUE;
}

grevity_boln grevityReaderFree(BufferPointer const readerPointer) {
    if (readerPointer != NULL) {
        free(readerPointer->content);
        free(readerPointer);
        return GREVITY_TRUE;
    }
    return GREVITY_FALSE;
}

grevity_boln grevityReaderIsFull(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;
    return readerPointer->flags.isFull;
}

grevity_boln grevityReaderIsEmpty(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;
    return readerPointer->flags.isEmpty;
}

grevity_boln grevityReaderSetMark(BufferPointer const readerPointer, grevity_intg mark) {
    if (readerPointer == NULL || mark < 0 || mark >= readerPointer->position.wrte) return GREVITY_FALSE;
    readerPointer->position.mark = mark;
    return GREVITY_TRUE;
}

grevity_intg grevityReaderPrint(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return 0;
    for (grevity_intg i = 0; i < readerPointer->position.wrte; i++) {
        putchar(readerPointer->content[i]);
    }
    return readerPointer->position.wrte;
}

grevity_intg grevityReaderLoad(BufferPointer const readerPointer, grevity_strg fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return READER_ERROR;
    }

    grevity_intg ch;
    grevity_intg count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (grevityReaderAddChar(readerPointer, (grevity_char)ch) == NULL) {
            fclose(file);
            return READER_ERROR;
        }
        count++;
    }
    fclose(file);
    return count;
}

grevity_intg grevityReaderLoadFromString(BufferPointer const readerPointer, grevity_strg text) {
    if (readerPointer == NULL || text == NULL) return READER_ERROR;
    grevity_intg count = 0;
    for (grevity_intg i = 0; text[i] != '\0'; i++) {
        if (grevityReaderAddChar(readerPointer, text[i]) == NULL) {
            return READER_ERROR;
        }
        count++;
    }
    return count;
}

grevity_intg grevityReaderChecksum(BufferPointer readerPointer) {
    grevity_intg checksum = 0;
    for (grevity_intg i = 0; i < readerPointer->position.wrte; i++) {
        checksum += readerPointer->content[i];
    }
    readerPointer->checkSum = checksum % 256;
    return checksum;
}

grevity_intg grevityReaderGetSize(BufferPointer const readerPointer) {
    return readerPointer->size;
}

grevity_char grevityReaderGetChar(BufferPointer const readerPointer) {
    if (readerPointer == NULL || readerPointer->position.read >= readerPointer->position.wrte) {
        return READER_TERMINATOR;
    }
    return readerPointer->content[readerPointer->position.read++];
}

grevity_strg grevityReaderGetContent(BufferPointer const readerPointer, grevity_intg pos) {
    if (readerPointer == NULL || pos < 0 || pos >= readerPointer->position.wrte) {
        return NULL;
    }
    return &readerPointer->content[pos];
}

grevity_intg grevityReaderGetPosRead(BufferPointer const readerPointer) {
    return readerPointer->position.read;
}

grevity_intg grevityReaderGetPosWrte(BufferPointer const readerPointer) {
    return readerPointer->position.wrte;
}

grevity_intg grevityReaderGetPosMark(BufferPointer const readerPointer) {
    return readerPointer->position.mark;
}

grevity_byte grevityReaderGetFlags(BufferPointer const readerPointer) {
    return *(grevity_byte*)&readerPointer->flags;
}

void grevityReaderPrintStat(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return;
    printf("\nFull ASCII Character Usage Table:\n");
    printf("+-------+-----+-------+\n");
    printf("| ASCII | CHR | COUNT |\n");
    printf("+-------+-----+-------+\n");
    for (int i = 0; i < NCHAR; i++) {
        char symbol = (i >= 32 && i <= 126) ? (char)i : '.';
        printf("| %5d |  %c  | %5d |\n", i, symbol, readerPointer->histogram[i]);
    }
    printf("+-------+-----+-------+\n");
}

grevity_intg grevityReaderNumErrors(BufferPointer const readerPointer) {
    return readerPointer->numReaderErrors;
}

grevity_boln grevityReaderRecover(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;
    readerPointer->position.read = 0;
    readerPointer->position.mark = 0;
    return GREVITY_TRUE;
}

grevity_boln grevityReaderRetract(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;
    if (readerPointer->position.read <= 0) return GREVITY_FALSE;
    readerPointer->position.read--;
    return GREVITY_TRUE;
}

grevity_boln grevityReaderRestore(BufferPointer const readerPointer) {
    if (readerPointer == NULL) return GREVITY_FALSE;
    readerPointer->position.read = readerPointer->position.mark;
    return GREVITY_TRUE;
}

grevity_void displayBuffer(BufferPointer ptr_Buffer) {
    if (ptr_Buffer == NULL) {
        printf("Buffer is NULL.\n");
        return;
    }
    printf("\nPrinting buffer parameters:\n");
    printf("Buffer Size: %d\n", grevityReaderGetSize(ptr_Buffer));
    printf("Current Size: %d\n", grevityReaderGetPosWrte(ptr_Buffer));
    printf("First Symbol: %c\n", (ptr_Buffer->position.wrte > 0) ? ptr_Buffer->content[0] : ' ');

    printf("Flags: \n");
    printf("  isEmpty: %d\n", ptr_Buffer->flags.isEmpty);
    printf("  isFull: %d\n", ptr_Buffer->flags.isFull);
    printf("  isRead: %d\n", ptr_Buffer->flags.isRead);
    printf("  isMoved: %d\n", ptr_Buffer->flags.isMoved);

    grevityReaderPrintStat(ptr_Buffer);

    printf("Number of Errors: %d\n", ptr_Buffer->numReaderErrors);
    printf("Checksum: %d\n", ptr_Buffer->checkSum);
}
