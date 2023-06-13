<div align="center">
# This project is a ESP8266 version project from:

# [daniele-salvagni/wol-bot-esp32](https://github.com/daniele-salvagni/wol-bot-esp32)

# Wake on Lan 🤖 ESP8266 Telegram Bot

_An embedded telegram bot for ESP8266 to send a WOL packet on the local network._  
_Blog post with more info: https://dan.salvagni.io/b/embedded-telegram-bot-for-wake-on-lan-pc/_

![Wake-on-Lan Telegram Bot](https://user-images.githubusercontent.com/6751621/186778280-31f02a80-b113-4bbb-a5c2-eb89dbb4cd9d.png)

</div>

## 📄 Description
This is a sketch for the **NodeMCU ESP8266-12E with CH340G** board. A telegram bot is hosted on the board and listens for messages, when the `/wol` command is received a Wake-on-Lan magic packet is broadcasted on the local network in order to turn on a target device.

🪙 You can keep it running for about **1$** in electricity per year, the power consumption is less than half a watt.

My ESP8266 board with CH340G:
![image-20230613142712453](https://github.com/Qynklee/WakeOnLAN-ESP8266-bot/blob/main/img/image-20230613142712453.png?raw=true)

## ⚙️ Installation

- Install the following libraries from the Library Manager of the Arduino IDE:
  - [WakeOnLan](https://www.arduino.cc/reference/en/libraries/wakeonlan/): for sending the magic packet
  - [UniversalTelegramBot](https://www.arduino.cc/reference/en/libraries/universaltelegrambot/): for using the Telegram API
  - Add NodeMCU board in BoardManager with this [guide](https://www.instructables.com/Programming-ESP8266-ESP-12E-NodeMCU-Using-Arduino-/) ***(only install ESP8266 2.7.4).***
    ![image-20230613141847766](https://github.com/Qynklee/WakeOnLAN-ESP8266-bot/blob/main/img/image-20230613141847766.png?raw=true)
- Create a new Telegram bot and configure your `BOT_TOKEN` and `ALLOWED_ID`  
  _You can use [@Botfather](https://t.me/botfather) to create a new bot and [@userinfobot](https://t.me/userinfobot) to get your ID_
- Fill your _WiFi configuration_ and the _MAC address_ of the PC you want to power on.
- Compile and flash

## 🔎 Usage

- Use `/start` to get a list of the available commands
- Use the `/wol` command or press the physical button to turn on your PC
- Use the `/ping` command to check if the bot is online

## ✅ TODO List

- [ ] Refactor code
- [ ] Store network configuration in EEPROM
- [ ] Add configuration commands to Telegram bot
