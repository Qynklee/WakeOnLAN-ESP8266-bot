#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "WiFiClientSecure.h"
#include "WiFiUDP.h"

#include "WakeOnLan.h"
#include "UniversalTelegramBot.h"
#include "ArduinoJson.h"

// WiFi configuration
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "PASSWORD"

// Telegram Bot Token
#define BOT_TOKEN  "0000000000:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define ALLOWED_ID "000000000"

// MAC address of the target device
#define MAC_ADDR "12:34:56:78:9A:BC"

ESP8266WiFiMulti wifiMulti;
WiFiClientSecure secured_client;
WiFiUDP UDP;
WakeOnLan WOL(UDP);

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

void sendWOL() {
  Serial.println("Send WakeOnLAN!");
  WOL.sendMagicPacket(MAC_ADDR); // send WOL on default port (9)
  delay(300);
}

void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    Serial.println(bot.messages[i].from_id);
    if (bot.messages[i].from_id != ALLOWED_ID) continue;
    
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/wol") {
      sendWOL();
      bot.sendMessage(chat_id, "Magic Packet sent!", "");
    } else if (text == "/ledon") {
      bot.sendMessage(chat_id, "Led is ON.", "");
    } else if (text == "/ledoff") {
      bot.sendMessage(chat_id, "Led is OFF.", "");
    } else if (text == "/ping") {
      String statusMessg = "Pong! Hi " + from_name + "\n";
      statusMessg += "IP Address: ";
      statusMessg += WiFi.localIP().toString();
      statusMessg += "\nMAC Address: ";
      statusMessg +=  WiFi.macAddress();
      statusMessg += "\n";
      bot.sendMessage(chat_id, statusMessg, "");
    } else if (text == "/start") {
      String welcome = "Welcome to **WoL Bot**, " + from_name + ".\n";
      welcome += "Use is restricted to the bot owner.\n\n";
      welcome += "/ledon : Switch the Led ON\n";
      welcome += "/ledoff : Switch the Led OFF\n";
      welcome += "/wol : Send the Magic Packet\n";
      welcome += "/ping : Check the bot status\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

/* =================================== SETUP =================================== */

void setup(){
  // Configure WiFI
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  // Clear the serial port buffer and set the serial port baud rate to 9600.
  Serial.begin(9600);
  delay(10);

  // Add root certificate for api.telegram.org
  secured_client.setTrustAnchors(&cert);

  Serial.print("Connecting to WiFI...");
  while ((wifiMulti.run() != WL_CONNECTED)) { // wait for WiFi connection.
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi connected");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Attention: 255.255.255.255 is denied in some networks
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); 
    
  Serial.println("Retrieving time...");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(150);
    now = time(nullptr);
  }
  Serial.println(now);
}

/*==========================================================================================*/
void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("Response received");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }

  delay(10);
}

