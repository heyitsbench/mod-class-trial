#include "class_trial_loader.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class trialannounce : public PlayerScript {
public:
    trialannounce() : PlayerScript("trial-announce") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("AnnounceEnable", true))
            ChatHandler(player->GetSession()).SendSysMessage(ALERT_MODULE_PRESENCE);

        return;
    };

private:
    enum moduleStrings
    {
        ALERT_MODULE_PRESENCE = 40050
    };
};

class applyTrial : public PlayerScript {
public:
    applyTrial() : PlayerScript("applyTrial") { }

    static void imbueTrial(Player* player)
    {
    }
};

using namespace Acore::ChatCommands;

class class_trial_commandscript : public CommandScript {
public:
    class_trial_commandscript() : CommandScript("class_trial_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "classtrial", trialHelper, SEC_PLAYER, Console::No },
        };
        return commandTable;
    }

    static bool trialHelper(ChatHandler* handler)
    {
        applyTrial::imbueTrial(handler->GetPlayer());
        return true;
    }
};

void Add_class_trial()
{
    new applyTrial();
    new trialannounce();
}

void AddSC_class_trial_commandscript()
{
    new class_trial_commandscript();
}
