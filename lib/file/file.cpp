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
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("Erro ao montar sistema de arquivos.\n");
        #endif /* FILE_SYSTEM_DEBUG */    
        
        return false;
    }
    #if FILE_SYSTEM_DEBUG == true
        else 
        {
            Serial.println("Sistema de Arquivos iniciado com sucesso.\n");
        }
    #endif /* FILE_SYSTEM_DEBUG */    

    return true;
}
/******************************************************************************/
bool FileSystemCreateDir( fs::FS &fs, const char *path )
{
    #if FILE_SYSTEM_DEBUG == true
        Serial.printf("Creating Dir: %s\n", path);
    #endif /* FILE_SYSTEM_DEBUG */

    if(path != NULL)
    {
        if(LittleFS.mkdir(path))
        {
            #if FILE_SYSTEM_DEBUG == true
                Serial.println("Diretório criado com sucesso.\n");
            #endif /* FILE_SYSTEM_DEBUG */

            return true;
        }
        else
        {
            #if FILE_SYSTEM_DEBUG == true
                Serial.println("Erro ao criar diretório.\n");
            #endif /* FILE_SYSTEM_DEBUG */

            return false;
        }
    }

    return false;

}
/******************************************************************************/
void FileSystemListDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- failed to open directory");
        #endif /* FILE_SYSTEM_DEBUG */

        return;
    }

    if(!root.isDirectory())
    {    
        #if FILE_SYSTEM_DEBUG == true
            Serial.println(" - not a directory");
        #endif /* FILE_SYSTEM_DEBUG */
        
        return;
    }

    File file = root.openNextFile();
    while(file)
    {
        if(file.isDirectory())
        {
            #if FILE_SYSTEM_DEBUG == true
                Serial.print("  DIR : ");
            #endif /* FILE_SYSTEM_DEBUG */

            #if CONFIG_LITTLEFS_FOR_IDF_3_2  && FILE_SYSTEM_DEBUG == true
                Serial.println(file.name());
            #else
                #if FILE_SYSTEM_DEBUG == true
                    Serial.print(file.name());
                #endif /* FILE_SYSTEM_DEBUG */

                time_t t= file.getLastWrite();
                struct tm * tmstruct = localtime(&t);
                #if FILE_SYSTEM_DEBUG == true
                    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
                #endif /* FILE_SYSTEM_DEBUG */
            #endif  

            if(levels)
            {
                FileSystemListDir(fs, file.name(), levels -1);
            }
        }
        else
        {
            #if FILE_SYSTEM_DEBUG == true
                Serial.print("  FILE: ");
                Serial.print(file.name());
                Serial.print("  SIZE: ");
            #endif /* FILE_SYSTEM_DEBUG */

            #if CONFIG_LITTLEFS_FOR_IDF_3_2
                Serial.println(file.size());
            #else

                #if FILE_SYSTEM_DEBUG == true
                    Serial.print(file.size());
                #endif /* FILE_SYSTEM_DEBUG */
                time_t t= file.getLastWrite();
                struct tm * tmstruct = localtime(&t);
                
                #if FILE_SYSTEM_DEBUG == true
                    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900, ( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
                #endif /* FILE_SYSTEM_DEBUG */
            #endif
        }

        file = root.openNextFile();
    }
}
/******************************************************************************/
void FileSystemWriteFile( fs::FS &fs, const char * path, const char * message ){
    
    #if FILE_SYSTEM_DEBUG == true
        Serial.printf("Writing file: %s\r\n", path);
    #endif /* FILE_SYSTEM_DEBUG */

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- failed to open file for writing\n");
        #endif /* FILE_SYSTEM_DEBUG */
        return;
    }
    
    if(file.print(message))
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- file written\n");
        #endif /* FILE_SYSTEM_DEBUG */
    }
    else
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- write failed\n");
        #endif /* FILE_SYSTEM_DEBUG */
    }
    file.close();
}
/******************************************************************************/
void FileSystemAppendFile( fs::FS &fs, const char * path, const char * message )
{

    #if FILE_SYSTEM_DEBUG == true
        Serial.printf("Appending to file: %s\r\n", path);
    #endif /* FILE_SYSTEM_DEBUG */

    File file = fs.open(path, FILE_APPEND);

    if(!file)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- failed to open file for appending");
        #endif /* FILE_SYSTEM_DEBUG */
        return;
    }

    if(file.print(message))
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- message appended");
        #endif /* FILE_SYSTEM_DEBUG */

    }
    else
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- append failed");
        #endif /* FILE_SYSTEM_DEBUG */
    }

    file.close();
}
/******************************************************************************/
void FileSystemReadFile( fs::FS &fs, const char * path )
{
    #if FILE_SYSTEM_DEBUG == true
        Serial.printf("Reading file: %s\r\n", path);
    #endif /* FILE_SYSTEM_DEBUG */

    File file = fs.open(path);
    if(!file || file.isDirectory()){

        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- failed to open file for reading");
        #endif /* FILE_SYSTEM_DEBUG */

        return;
    }

    #if FILE_SYSTEM_DEBUG == true
        Serial.println("- read from file:\n");
    #endif /* FILE_SYSTEM_DEBUG */

    while(file.available())
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.write(file.read());
        #endif /* FILE_SYSTEM_DEBUG */
    }

    file.close();
}
/******************************************************************************/
void FileSystemDeleteFile( fs::FS &fs, const char * path )
{
    #if FILE_SYSTEM_DEBUG == true
        Serial.printf("Deleting file: %s\r\n", path);
    #endif /* FILE_SYSTEM_DEBUG */

    if(fs.remove(path))
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- file deleted");
        #endif /* FILE_SYSTEM_DEBUG */
    }
    else
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("- delete failed");
        #endif /* FILE_SYSTEM_DEBUG */
    }
}
/******************************************************************************/
String FileSystemFindLastWrite( fs::FS &fs, const char * path, uint8_t buffer[], uint8_t offset )
{
    File file = fs.open(path, FILE_READ);

    if(!file)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("Erro ao tentar abrir arquivo");
        #endif /* FILE_SYSTEM_DEBUG */

        return "";
    }

    if(file.seek((-1 * offset), SeekEnd))
    {
        while(file.available())
        {
            #if FILE_SYSTEM_DEBUG == true
                // Serial.write(file.read());
            #endif /* FILE_SYSTEM_DEBUG */

            file.read(buffer, offset);

        }

        return "";
    }
    else 
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("Erro na função file.Seek()");
        #endif /* FILE_SYSTEM_DEBUG */
    }

    file.close();
    return "";
}
/******************************************************************************/
bool FileSystemFileExists( fs::FS &fs, const char * path )
{
    if(fs.exists(path))
    {
        return true;
    }
    
    return false;
}
/******************************************************************************/
bool FileSystemCreateFile( fs::FS &fs, const char * path )
{
    File file = fs.open(path, "w+");

    if(!file)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.println("O arquivo não existe ou o caminho está errado.\n");
        #endif /* FILE_SYSTEM_DEBUG */    
        return false;
    }

    return true;
}
/******************************************************************************/
uint8_t FileSystemGetInterval(fs::FS &fs, const char * path, uint8_t lineStart, uint8_t lineEnd, uint8_t lineSize, uint8_t buffer[])
{

    uint8_t lines = 0;

    File file; 
    file = fs.open(path, "r");

    if(!path)
    {
        #if FILE_SYSTEM_DEBUG == true
            Serial.printf("Erro no ao tentar abrir o arquivo. %s\n", path);
        #endif /* FILE_SYSTEM_DEBUG */   
    }

   if(lineSize != 0)
   {
        /* Ponteiro para a linha especificada */
        file.seek(lineStart * lineSize);
        lines = file.read(buffer, (lineEnd - lineStart) * lineSize);
   }
   else 
   {

   }


   file.close();

   return lines;
}
/******************************************************************************/