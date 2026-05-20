#ifndef READER_H_
#define READER_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Compilers.h"

#define READER_ERROR (-1)
#define READER_TERMINATOR '\0'
#define READER_DEFAULT_SIZE 100
#define READER_MAX_SIZE 10000
#define NCHAR 128
#define CHARSEOF (-1)

typedef struct position {
    grevity_intg read;
    grevity_intg wrte;
    grevity_intg mark;
} Position;

typedef struct flag {
    grevity_boln isEmpty;
    grevity_boln isFull;
    grevity_boln isRead;
    grevity_boln isMoved;
} Flag;

struct Buffer {
    grevity_strg content;
    grevity_intg size;
    Flag flags;
    Position position;
    grevity_intg histogram[NCHAR];
    grevity_intg numReaderErrors;
    grevity_byte checkSum;
};
typedef struct Buffer* BufferPointer; // This matches Compilers.h!

BufferPointer grevityReaderCreate(grevity_intg size);
BufferPointer grevityReaderAddChar(BufferPointer const, grevity_char);
grevity_boln grevityReaderClear(BufferPointer const);
grevity_boln grevityReaderFree(BufferPointer const);
grevity_boln grevityReaderIsFull(BufferPointer const);
grevity_boln grevityReaderIsEmpty(BufferPointer const);
grevity_boln grevityReaderSetMark(BufferPointer const, grevity_intg);
grevity_intg grevityReaderPrint(BufferPointer const);
grevity_intg grevityReaderLoad(BufferPointer const, grevity_strg);
grevity_intg grevityReaderLoadFromString(BufferPointer const, grevity_strg);
grevity_boln grevityReaderRecover(BufferPointer const);
grevity_boln grevityReaderRetract(BufferPointer const);
grevity_boln grevityReaderRestore(BufferPointer const);
grevity_char grevityReaderGetChar(BufferPointer const);
grevity_strg grevityReaderGetContent(BufferPointer const, grevity_intg);
grevity_intg grevityReaderGetPosRead(BufferPointer const);
grevity_intg grevityReaderGetPosWrte(BufferPointer const);
grevity_intg grevityReaderGetPosMark(BufferPointer const);
grevity_intg grevityReaderGetSize(BufferPointer const);
grevity_byte grevityReaderGetFlags(BufferPointer const);
void grevityReaderPrintStat(BufferPointer const);
grevity_intg grevityReaderNumErrors(BufferPointer const);
grevity_intg grevityReaderChecksum(BufferPointer const);

#endif // READER_H_
