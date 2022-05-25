/* $Id: esp32_mqtts.ino 1 2022-05-25 04:26:14Z bpahlawa $
 Created 22-MAR-2021
 $Author: bpahlawa $
 $Date: 2022-05-25 12:26:14 +0800 (Wed, 25 May 2022) $
 $Revision: 1 $
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>


const char* ssid     = "CALIPH";     // your network SSID (name of wifi network)
const char* password = "topsecret"; // your network password



const char* Server = "bramiothub.azure-devices.net";

//iot topic has to be in a format of devices/<device_id>/messages/events/
//if device_id is not created yet, then run the following command on azure-cli
//az iot hub device-identity create -n bramiothub -d esp8266
const char* topic = "devices/esp8266/messages/events/";


//Get the username as {iothubhostname}/{device_id}/?api-version=2021-04-12
const char* MQTTUsername="bramiothub.azure-devices.net/esp8266/?api-version=2021-04-12";
//Get the password from sas using the following command in azure-cli
//az iot hub generate-sas-token -d esp8266 -n bramiothub 
const char* MQTTPass="SharedAccessSignature sr=bramiothub.azure-devices.net%2Fdevices%2Fesp8266&sig=VMXXXXXXXXX%2BlboIHer%2B0Tp7E%3D&se=1001653269864";



/* Baltimore CyberTrust Root */
const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\r\n"
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\r\n"
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\r\n"
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\r\n"
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\r\n"
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\r\n"
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\r\n"
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\r\n"
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\r\n"
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\r\n"
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\r\n"
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\r\n"
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\r\n"
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\r\n"
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\r\n"
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\r\n"
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\r\n"
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\r\n"
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\r\n"
"-----END CERTIFICATE-----\r\n";


WiFiClientSecure secureClient;
PubSubClient client(secureClient);


int pocitadlo = 0;
long lastMsg = 0;
char msg[20];
int counter = 0;
//device_id as clientId
String clientId = "esp8266";

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTTUsername,MQTTPass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("UART ready");
  secureClient.setCACert(ca_cert);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(Server, 8883);
  client.setCallback(callback);
}
void loop() {
String message = "Bram Is OK";

  if (!client.connected()) {
    reconnect();
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.loop();
  if (millis() - lastMsg > 10000) {
    //this is just a counter
    char pocitadlo_pole[8];
    pocitadlo  = pocitadlo + 1;
    dtostrf(pocitadlo, 1, 2, pocitadlo_pole);
    message.concat(pocitadlo_pole);
    Serial.print(message.c_str());
    client.publish(topic, message.c_str());
    lastMsg = millis();
  }
}
