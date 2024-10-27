#include <WiFi.h>
#include <M5Unified.h>
#include "sMQTTBroker.h"

class MyBroker : public sMQTTBroker {
   public:
    bool onConnect(sMQTTClient *client, const std::string &username,
                   const std::string &password) {
        String message =
            "onConnect clientId=" + String(client->getClientId().c_str()) +
            " username=" + username.c_str();
        M5.Lcd.fillRect(0, 0, 320, 80, BLACK);  // 画面をクリア
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("%s\n", message.c_str());  // M5.Lcdに出力
        return true;
    }

    void onRemove(sMQTTClient *client) {
        String message =
            "onRemove clientId=" + String(client->getClientId().c_str());
        M5.Lcd.fillRect(0, 0, 320, 80, BLACK);  // 画面をクリア
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("%s\n", message.c_str());  // M5.Lcdに出力
    }

    void onPublish(sMQTTClient *client, const std::string &topic,
                   const std::string &payload) {
        String message =
            "onPublish clientId=" + String(client->getClientId().c_str()) +
            " topic=" + topic.c_str() + " payload=" + payload.c_str();
        M5.Lcd.fillRect(0, 0, 320, 80, BLACK);  // 画面をクリア
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("%s\n", message.c_str());  // M5.Lcdに出力
    }

    bool onEvent(sMQTTEvent *event) {
        switch (event->Type()) {
            case NewClient_sMQTTEventType: {
                sMQTTNewClientEvent *e = (sMQTTNewClientEvent *)event;
                e->Login();
                e->Password();
            } break;

            case LostConnect_sMQTTEventType:
                WiFi.reconnect();
                break;
        }
        return true;
    }
};

MyBroker broker;

const char *ssid = "Buffalo-G-3066";
const char *password = "vbjc3jyxh7h8c";

void startWiFiClient() {
    M5.Lcd.fillScreen(BLACK);  // 画面をクリア
    M5.Lcd.setTextSize(2);     // テキストサイズを設定
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("Connecting to Wi-Fi...\n");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");  // 接続中のメッセージ
    }

    String successMessage =
        "WiFi connected!\nIP address: " + WiFi.localIP().toString();
    M5.Lcd.fillRect(0, 0, 320, 80, BLACK);  // 画面をクリア
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("%s\n", successMessage.c_str());  // 成功メッセージを表示
}

void setup() {
    M5.begin();                // M5Stackの初期化
    M5.Lcd.fillScreen(BLACK);  // 画面をクリア
    M5.Lcd.setTextSize(2);     // テキストサイズを設定

    startWiFiClient();  // Wi-Fi接続
    const unsigned short mqttPort = 1883;
    broker.init(mqttPort);  // MQTTブローカーの初期化

    // ポート情報の表示
    M5.Lcd.fillRect(0, 80, 320, 80, BLACK);  // 画面をクリア
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.printf("MQTT broker started on port: %d\n", mqttPort);
}

void loop() {
    broker.update();  // ブローカーの更新
    M5.update();
    delay(1000);
}
