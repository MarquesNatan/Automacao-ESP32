/******************************************************************************/
#include "include/file.h"
#include "../../src/system/include/system_defs.h"

#include "Arduino.h"

#include "FS.h"
#include "LittleFS.h"
/******************************************************************************/
bool FileSystemStart( void )
{
    if(!LittleFS.begin(true))
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("Erro ao montar sistema de arquivos.\n");
        #endif /* DEBUG_FILE_SYSTEM */    
        
        return false;
    }

    return true;
}
/******************************************************************************/
void FileSystemCreateDir( fs::FS &fs, const char *path )
{
    #if DEBUG_FILE_SYSTEM == true
        Serial.printf("Creating Dir: %s\n", path);
    #endif /* DEBUG_FILE_SYSTEM */

    if(path != NULL)
    {
        if(LittleFS.mkdir(path))
        {
            #if DEBUG_FILE_SYSTEM == true
                Serial.println("Diretório criado com sucesso.\n");
            #endif /* DEBUG_FILE_SYSTEM */
        }
        else
        {
            #if DEBUG_FILE_SYSTEM == true
                Serial.println("Erro ao criar diretório.\n");
            #endif /* DEBUG_FILE_SYSTEM */
        }
    }

}
/******************************************************************************/
void FileSystemListDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root)
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- failed to open directory");
        #endif /* DEBUG_FILE_SYSTEM */

        return;
    }

    if(!root.isDirectory())
    {    
        #if DEBUG_FILE_SYSTEM == true
            Serial.println(" - not a directory");
        #endif /* DEBUG_FILE_SYSTEM */
        
        return;
    }

    File file = root.openNextFile();
    while(file)
    {
        if(file.isDirectory())
        {
            #if DEBUG_FILE_SYSTEM == true
                Serial.print("  DIR : ");
            #endif /* DEBUG_FILE_SYSTEM */

            #if CONFIG_LITTLEFS_FOR_IDF_3_2  && DEBUG_FILE_SYSTEM == true
                Serial.println(file.name());
            #else
                #if DEBUG_FILE_SYSTEM == true
                    Serial.print(file.name());
                #endif /* DEBUG_FILE_SYSTEM */

                time_t t= file.getLastWrite();
                struct tm * tmstruct = localtime(&t);
                #if DEBUG_FILE_SYSTEM == true
                    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
                #endif /* DEBUG_FILE_SYSTEM */
            #endif  

            if(levels)
            {
                FileSystemListDir(fs, file.name(), levels -1);
            }
        }
        else
        {
            #if DEBUG_FILE_SYSTEM == true
                Serial.print("  FILE: ");
                Serial.print(file.name());
                Serial.print("  SIZE: ");
            #endif /* DEBUG_FILE_SYSTEM */

            #if CONFIG_LITTLEFS_FOR_IDF_3_2
                Serial.println(file.size());
            #else

                #if DEBUG_FILE_SYSTEM == true
                    Serial.print(file.size());
                #endif /* DEBUG_FILE_SYSTEM */
                time_t t= file.getLastWrite();
                struct tm * tmstruct = localtime(&t);
                
                #if DEBUG_FILE_SYSTEM == true
                    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900, ( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
                #endif /* DEBUG_FILE_SYSTEM */
            #endif
        }

        file = root.openNextFile();
    }
}
/******************************************************************************/
void FileSystemWriteFile( fs::FS &fs, const char * path, const char * message ){
    
    #if DEBUG_FILE_SYSTEM == true
        Serial.printf("Writing file: %s\r\n", path);
    #endif /* DEBUG_FILE_SYSTEM */

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- failed to open file for writing\n");
        #endif /* DEBUG_FILE_SYSTEM */
        return;
    }
    
    if(file.print(message))
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- file written\n");
        #endif /* DEBUG_FILE_SYSTEM */
    }
    else
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- write failed\n");
        #endif /* DEBUG_FILE_SYSTEM */
    }
    file.close();
}
/******************************************************************************/
void appendFile( fs::FS &fs, const char * path, const char * message )
{

    #if DEBUG_FILE_SYSTEM == true
        Serial.printf("Appending to file: %s\r\n", path);
    #endif /* DEBUG_FILE_SYSTEM */

    File file = fs.open(path, FILE_APPEND);

    if(!file)
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- failed to open file for appending");
        #endif /* DEBUG_FILE_SYSTEM */
        return;
    }

    if(file.print(message))
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- message appended");
        #endif /* DEBUG_FILE_SYSTEM */

    }
    else
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- append failed");
        #endif /* DEBUG_FILE_SYSTEM */
    }

    file.close();
}
/******************************************************************************/
void FileSystemReadFile( fs::FS &fs, const char * path )
{
    #if DEBUG_FILE_SYSTEM == true
        Serial.printf("Reading file: %s\r\n", path);
    #endif /* DEBUG_FILE_SYSTEM */

    File file = fs.open(path);
    if(!file || file.isDirectory()){

        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- failed to open file for reading");
        #endif /* DEBUG_FILE_SYSTEM */

        return;
    }

    #if DEBUG_FILE_SYSTEM == true
        Serial.println("- read from file:\n");
    #endif /* DEBUG_FILE_SYSTEM */

    while(file.available())
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.write(file.read());
        #endif /* DEBUG_FILE_SYSTEM */
    }

    file.close();
}
/******************************************************************************/
void FileSystemDeleteFile( fs::FS &fs, const char * path )
{
    #if DEBUG_FILE_SYSTEM == true
        Serial.printf("Deleting file: %s\r\n", path);
    #endif /* DEBUG_FILE_SYSTEM */

    if(fs.remove(path))
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- file deleted");
        #endif /* DEBUG_FILE_SYSTEM */
    }
    else
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("- delete failed");
        #endif /* DEBUG_FILE_SYSTEM */
    }
}
/******************************************************************************/
void FileSystemFindText( fs::FS &fs, const char * path, const char *textToFind )
{

}
/******************************************************************************/
String FileSystemFindLastWrite( fs::FS &fs, const char * path, uint8_t buffer[], uint8_t offset )
{
    File file = fs.open(path, FILE_READ);

    if(!file)
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("Erro ao tentar abrir arquivo");
        #endif /* DEBUG_FILE_SYSTEM */

        return "";
    }

    if(file.seek((-1 * offset), SeekEnd))
    {
        while(file.available())
        {
            #if DEBUG_FILE_SYSTEM == true
                // Serial.write(file.read());
            #endif /* DEBUG_FILE_SYSTEM */

            file.read(buffer, offset);

        }

        return "";
    }
    else 
    {
        #if DEBUG_FILE_SYSTEM == true
            Serial.println("Erro na função file.Seek()");
        #endif /* DEBUG_FILE_SYSTEM */
    }

    file.close();
    return "";
}
/******************************************************************************/