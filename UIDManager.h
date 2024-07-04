#ifndef UIDMANAGER_H
#define UIDMANAGER_H

#include <Arduino.h>

void readUIDFromEEPROM(byte *uid);
String convertUIDToString(byte *uid);
void createAndSaveUID();
String getOrCreateUID();

#endif // UIDMANAGER_H
