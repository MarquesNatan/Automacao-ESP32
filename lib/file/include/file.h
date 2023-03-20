/******************************************************************************/
#ifndef FILE_H
#define FILE_H
/******************************************************************************/
#include "FS.h"
/******************************************************************************/
bool FileSystemStart( void );
void FileSystemCreateDir( fs::FS &fs, const char *path );
void FileSystemListDir( fs::FS &fs, const char * dirname, uint8_t levels );
void FileSystemWriteFile( fs::FS &fs, const char * path, const char * message );
void FileSystemReadFile( fs::FS &fs, const char * path );
void FileSystemDeleteFile( fs::FS &fs, const char * path );
/******************************************************************************/
#endif /* FILE_H */
/******************************************************************************/