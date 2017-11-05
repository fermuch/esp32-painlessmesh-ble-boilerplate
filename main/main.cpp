#include <Arduino.h>
#include <painlessMesh.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define SERVICE_UUID        "80727b06-686b-40e9-b1b1-429f7736dabd"
#define CHARACTERISTIC_UUID "ce9459f6-ea74-4ccf-aa96-5bb92012a432"

// set up the Mesh
painlessMesh  mesh;
void sendMessage(); // Prototype
Task taskSendMessage(TASK_MILLISECOND * 100, TASK_FOREVER, &sendMessage); // start with a one second interval

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }
  ;

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void sendMessage() {
  if (deviceConnected) {
    Serial.printf("BLE NOTIFY: %d\n", value);
    pCharacteristic->setValue(&value, 1);
    pCharacteristic->notify();
    value++;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // use A0 as entropy source
  randomSeed(analogRead(A0));

  Serial.print("ESP32 SDK: ");
  Serial.println(ESP.getSdkVersion());

  mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION | COMMUNICATION);
  mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
  Serial.println("painlessMesh started");

  // Create the BLE Device
  BLEDevice::init("ESPMESH");
  // Create the BLE Server
  BLEServer *pServer = new BLEServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
  CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
          | BLECharacteristic::PROPERTY_NOTIFY
          | BLECharacteristic::PROPERTY_INDICATE);

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  mesh.scheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

  Serial.println("BLE started");
}

void loop() {
  mesh.update();
}
