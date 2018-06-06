#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int leftMotorForward = 2;     /* GPIO2(D4) -> IN3   */
int rightMotorForward = 15;   /* GPIO15(D8) -> IN1  */
int leftMotorBackward = 0;    /* GPIO0(D3) -> IN4   */
int rightMotorBackward = 13;  /* GPIO13(D7) -> IN2  */
int LdrPin = A0;
int ledPin=16;//D0
int sensorValue=0;
int hiz=700;
/* define L298N or L293D enable pins */
int rightMotorENB = 14; /* GPIO14(D5) -> Motor-A Enable */
int leftMotorENB = 12;  /* GPIO12(D6) -> Motor-B Enable */


const char* ssid = "iPhone";
const char* password = "dereli123";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup() {
pinMode(leftMotorForward, OUTPUT);
pinMode(rightMotorForward, OUTPUT);
pinMode(leftMotorBackward, OUTPUT);
pinMode(rightMotorBackward, OUTPUT);
pinMode( ledPin, OUTPUT );
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Gelen Mesaj [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1'){
digitalWrite(leftMotorForward,HIGH);
digitalWrite(rightMotorForward,HIGH);
digitalWrite(leftMotorBackward,LOW);
digitalWrite(rightMotorBackward,LOW);
analogWrite(rightMotorENB, hiz);
analogWrite(leftMotorENB, hiz);
  } 
  if ((char)payload[0] == '2'){
digitalWrite(leftMotorBackward,HIGH);
digitalWrite(rightMotorBackward,HIGH);
digitalWrite(leftMotorForward,LOW);
digitalWrite(rightMotorForward,LOW);
analogWrite(rightMotorENB, hiz);
analogWrite(leftMotorENB, hiz);
  }
  if ((char)payload[0] == '3'){
digitalWrite(leftMotorForward,LOW);
digitalWrite(rightMotorForward,HIGH);
digitalWrite(rightMotorBackward,LOW);
digitalWrite(leftMotorBackward,HIGH);
analogWrite(rightMotorENB, hiz);
analogWrite(leftMotorENB, hiz);
  } 
  if ((char)payload[0] == '4'){
digitalWrite(leftMotorForward,HIGH);
digitalWrite(rightMotorForward,LOW);
digitalWrite(rightMotorBackward,HIGH);
digitalWrite(leftMotorBackward,LOW);
analogWrite(rightMotorENB, 700);
analogWrite(leftMotorENB, 700);
  } 
  if ((char)payload[0] == '5'){
digitalWrite(leftMotorForward,LOW);
digitalWrite(leftMotorBackward,LOW);
digitalWrite(rightMotorForward,LOW);
digitalWrite(rightMotorBackward,LOW);
analogWrite(rightMotorENB, 0);
analogWrite(leftMotorENB, 0);
  } 
  if ((char)payload[0] == '6'){
    sensorValue=analogRead(LdrPin);
  if(sensorValue<300)
  {
    digitalWrite(ledPin, HIGH); 
  }
  else
  {
    digitalWrite(ledPin, LOW);                                         
  }
  } 
  if ((char)payload[0] == '7'){
    hiz=700;
  } 
  if ((char)payload[0] == '8'){
    hiz=500;      
  } 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mesajCikis", "test mesaj");
      // ... and resubscribe
      client.subscribe("mesajGiris");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "iot publish test #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("mesajCikis", msg);
  }
}
