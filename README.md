# IOT Devices and their codes (Arduino, ESP32, ESP8266 and the clones)

## ESP32-WROOVER-IB
<pre>
Module: ESP32-WROOVER-IB (Genuine)
Board: Generic board chinese
How to get the module : https://www.aliexpress.com/item/33003556530.html?spm=a2g0o.order_list.0.0.18751802ZgZVNP
How to get the board : https://www.aliexpress.com/item/1005001315677299.html?spm=a2g0o.order_list.0.0.18751802ZgZVNP
Rework station and flux need to be used to solder the module into the board
IDE : Arduino IDE
Arduino board manager : install ESP32 WROOVER (all version)
Azure iothub: bramiothub
Azure iothub hostname: bramiothub.azure-devices.net
device_id : esp8266 (this an iot device which can be created on Azure portal or using azure-cli)
iothub topic: devices/esp8266/messages/events/
Username: </pre>"bramiothub.azure-devices.net/esp8266/?api-version=2021-04-12" <pre>(must be in the format: {iothubhostname}/{device_id}/?api-version=2021-04-12)
Pssword: "SharedAccessSignature sr=bramiothub.azure-devices.net%2Fdevices%2Fesp8266&sig=VMXXXXXXXXX%2BlboIHer%2B0Tp7E%3D&se=1001653269864"
wifi ssid: "CALIPH"
wifi pass: "topsecret"
test message to be ingested : Bram Is OK
</pre>
