#include "class_trial_loader.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

void Add_class_trial()
{
}

class announce : public PlayerScript {

public:
    announce() : PlayerScript("announce") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("AnnounceEnable", true))
            ChatHandler(player->GetSession()).SendSysMessage(ALERT_MODULE_PRESENCE);

        return;
    }

private:
    enum moduleStrings
    {
        ALERT_MODULE_PRESENCE = 40050
    };
};

void AddSC_class_trial_commandscript()
{
}
