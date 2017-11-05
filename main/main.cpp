#include <Arduino.h>
#include <painlessMesh.h>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define SERVICE_UUID        "80727b06-686b-40e9-b1b1-429f7736dabd"
#define CHARACTERISTIC_UUID "ce9459f6-ea74-4ccf-aa96-5bb92012a432"


// set up the Mesh
painlessMesh  mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;

void setup() {
  // use A0 as entropy source
  randomSeed(analogRead(A0));

  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR);
  mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
}

void loop() {
  mesh.update();
}
