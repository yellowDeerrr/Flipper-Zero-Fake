#pragma once

#include <Networks/Types/AttackTypes.h>
#include <esp_wifi.h>

#define DEFAULT_SECONDS_DEAUTH 10

class AttackDeauth {
private:
    DeauthAttackState state;
    deauth_frame_t frame;
    
public:
    AttackDeauth(const MacAddress& sourceAP, const MacAddress& targetSTA);

    void start(unsigned int seconds = DEFAULT_SECONDS_DEAUTH);
    // void stop();
    attack_status_t getStatus();

private:
    // void buildDeauthFrame();
    void sendDeauthFrame();
};