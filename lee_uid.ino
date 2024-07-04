#include <EEPROM.h>
#include "UIDManager.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Espera a que el puerto serie se conecte. Solo necesario para USB nativo
  }

  String uidString = getOrCreateUID();
  Serial.print("Unique ID: ");
  Serial.println(uidString);
}

void loop() {
  // No hay nada que hacer en el bucle
}
