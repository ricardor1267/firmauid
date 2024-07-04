#include "UIDManager.h"
#include <EEPROM.h>

// Función para leer el UID desde la EEPROM
void readUIDFromEEPROM(byte *uid) {
  for (int i = 0; i < 8; i++) {
    uid[i] = EEPROM.read(8 + i);
  }
}

// Función para convertir el UID a una cadena de texto, ignorando ceros iniciales
String convertUIDToString(byte *uid) {
  String uidString = "";
  bool leadingZeros = true; // Flag para ignorar ceros iniciales
  for (int i = 0; i < 8; i++) {
    if (uid[i] != 0 || !leadingZeros) {
      if (uid[i] < 0x10) {
        uidString += "0"; // Añadir un cero a los valores menores a 0x10 para mantener el formato
      }
      uidString += String(uid[i], HEX);
      if (i < 7) {
        uidString += "-";
      }
      leadingZeros = false; // Una vez que encontramos el primer byte no cero, deshabilitamos el flag
    }
  }
  // Eliminar el guion final si existe
  if (uidString.endsWith("-")) {
    uidString.remove(uidString.length() - 1);
  }
  return uidString;
}

// Función para crear y guardar el UID en la EEPROM
void createAndSaveUID() {
  // Leer el contador desde la EEPROM
  unsigned int counter;
  EEPROM.get(0, counter);

  // Incrementar el contador
  counter++;

  // Escribir el contador de nuevo en la EEPROM
  EEPROM.put(0, counter);

  // Inicializar el generador de números aleatorios
  randomSeed(analogRead(0));

  // Generar un identificador único basado en el tiempo, número aleatorio y el contador
  unsigned long currentTime = millis();
  unsigned int randomValue = random(0, 65535);

  byte uniqueID[8];
  uniqueID[0] = (currentTime >> 24) & 0xFF;
  uniqueID[1] = (currentTime >> 16) & 0xFF;
  uniqueID[2] = (currentTime >> 8) & 0xFF;
  uniqueID[3] = currentTime & 0xFF;
  uniqueID[4] = (randomValue >> 8) & 0xFF;
  uniqueID[5] = randomValue & 0xFF;
  uniqueID[6] = (counter >> 8) & 0xFF;
  uniqueID[7] = counter & 0xFF;

  // Escribir el identificador único en la EEPROM (a partir de la posición 8)
  for (int i = 0; i < 8; i++) {
    EEPROM.write(8 + i, uniqueID[i]);
  }

  Serial.print("Unique ID written to EEPROM: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(uniqueID[i], HEX);
    if (i < 7) {
      Serial.print("-");
    }
  }
  Serial.println();
}

// Función para obtener o crear el UID y devolverlo como cadena
String getOrCreateUID() {
  byte firstByte;
  EEPROM.get(8, firstByte);

  // Si el primer byte no es 0xFF, asumimos que el UID ya está escrito
  if (firstByte != 0xFF) {
    byte readID[8];
    readUIDFromEEPROM(readID);
    return convertUIDToString(readID);
  } else {
    createAndSaveUID();
    byte newID[8];
    readUIDFromEEPROM(newID);
    return convertUIDToString(newID);
  }
}
