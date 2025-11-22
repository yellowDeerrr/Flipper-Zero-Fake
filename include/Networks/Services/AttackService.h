#include "Networks/Types/AttackTypes.h"

#include "Networks/Attacks/AttackDeauth.h"

class AttackService {
private:
    attack_t currentType = attack_t::None;
    attack_status_t currentStatus = attack_status_t::Idle;
    unsigned long startMillis = 0;
    unsigned long elapsedMillis = 0;

    AttackDeauth* deauthAttack = nullptr;

public:
    AttackService();
    ~AttackService();

    void startDeauthAttack(const MacAddress& sourceAP, const MacAddress& targetSTA, unsigned int seconds = DEFAULT_SECONDS_DEAUTH);
    // void startHandshakeAttack(const MacAddress& sourceAP, const MacAddress& targetSTA);
    // void startPMKIDAttack(const MacAddress& sourceAP, const MacAddress& targetSTA);

    void stopAttack();

    attack_t getCurrentType() const;
    attack_status_t getCurrentStatus() const;
    unsigned long getExecutionTime();
};