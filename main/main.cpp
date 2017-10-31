#include <Arduino.h>
#include <painlessMesh.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define SERVICE_UUID        "80727b06-686b-40e9-b1b1-429f7736dabd"
#define CHARACTERISTIC_UUID "ce9459f6-ea74-4ccf-aa96-5bb92012a432"


// set up the Mesh
painlessMesh  mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;

// set up BLE
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// callbacks for BLE
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("New BLE connection!");
      pCharacteristic->setValue(mesh.subConnectionJson().c_str());
	  Serial.printf("New bluetooth value: %s\n", mesh.subConnectionJson().c_str());
	  pCharacteristic->notify();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("BLE device disconnected.");
    }
};

void changedConnectionCallback() {
  nodes = mesh.getNodeList();
  Serial.println("changedConnectionCallback()");
  Serial.printf("Num nodes: %d\n", nodes.size());
  if (deviceConnected) {
	pCharacteristic->setValue(mesh.subConnectionJson().c_str());
	Serial.printf("New bluetooth value: %s\n", mesh.subConnectionJson().c_str());
	pCharacteristic->notify();
  }
}

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR);
  mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);

  // set up ble server
  String bleName = "ESP32 ";
  bleName += mesh.getNodeId();
  BLEDevice::init(bleName.c_str());
  BLEServer *pServer = new BLEServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
	CHARACTERISTIC_UUID,
	BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());

  // advertise BLE to the world
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // use A0 as entropy source
  // randomSeed(analogRead(A0));
}

void loop() {
  mesh.update();
}
