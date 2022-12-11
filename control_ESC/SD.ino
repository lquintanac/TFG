
void initSD()
{

  spiSD.begin(14,2,15,13);
  delay(1000);
  while(!SD.begin(13,spiSD,SDSPEED)){
        SerialBT.println("Card Mount Failed");
        delay(1000);
  }
  uint8_t cardType = SD.cardType();
  
  if(cardType == CARD_NONE){
        SerialBT.println("No SD card attached");
        return;
    }
  
  SerialBT.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      SerialBT.println("MMC");
  } else if(cardType == CARD_SD){
      SerialBT.println("SDSC");
  } else if(cardType == CARD_SDHC){
      SerialBT.println("SDHC");
  } else {
      SerialBT.println("UNKNOWN");
  }
}


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    SerialBT.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        SerialBT.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        SerialBT.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            SerialBT.print("  DIR : ");
            SerialBT.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            SerialBT.print("  FILE: ");
            SerialBT.print(file.name());
            SerialBT.print("  SIZE: ");
            SerialBT.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    SerialBT.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        SerialBT.println("Dir created");
    } else {
        SerialBT.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    SerialBT.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        SerialBT.println("Dir removed");
    } else {
        SerialBT.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    SerialBT.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        SerialBT.println("Failed to open file for reading");
        return;
    }

    SerialBT.print("Read from file: ");
    while(file.available()){
        SerialBT.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    SerialBT.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        SerialBT.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        SerialBT.println("File written");
    } else {
        SerialBT.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, String message){
    SerialBT.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        SerialBT.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        SerialBT.println("Message appended");
    } else {
        SerialBT.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    SerialBT.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        SerialBT.println("File renamed");
    } else {
        SerialBT.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    SerialBT.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        SerialBT.println("File deleted");
    } else {
        SerialBT.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        SerialBT.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        SerialBT.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        SerialBT.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    SerialBT.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}
