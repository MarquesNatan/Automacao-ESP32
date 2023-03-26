/******************************************************************************/
#ifndef FILE_H
#define FILE_H
/******************************************************************************/
#include "FS.h"
/******************************************************************************/
bool FileSystemStart( void );
bool FileSystemCreateDir( fs::FS &fs, const char *path );
void FileSystemListDir( fs::FS &fs, const char * dirname, uint8_t levels );
void FileSystemWriteFile( fs::FS &fs, const char * path, const char * message );
void FileSystemReadFile( fs::FS &fs, const char * path );
void FileSystemDeleteFile( fs::FS &fs, const char * path );
String FileSystemFindLastWrite( fs::FS &fs, const char * path, uint8_t buffer[], uint8_t offset);
void FileSystemAppendFile( fs::FS &fs, const char * path, const char * message );
bool FileSystemFileExists( fs::FS &fs, const char * path );
bool FileSystemCreateFile( fs::FS &fs, const char * path );
uint8_t FileSystemGetInterval(fs::FS &fs, const char * path, uint8_t lineStart, uint8_t lineEnd, uint8_t lineSize, uint8_t buffer[]);
/******************************************************************************/
#endif /* FILE_H */
/******************************************************************************/