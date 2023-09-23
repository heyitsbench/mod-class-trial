#include "class_trial_loader.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class TrialAnnounce : public PlayerScript {
public:
    TrialAnnounce() : PlayerScript("TrialAnnounce") { }

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

class TrialOperation : public PlayerScript {
public:
    TrialOperation() : PlayerScript("TrialOperation") { }

    static void imbueTrial(Player* player)
    {
    }
};

using namespace Acore::ChatCommands;

class TrialCommands : public CommandScript {
public:
    TrialCommands() : CommandScript("TrialCommands") { }

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
        if (sConfigMgr->GetOption<bool>("ClassTrialEnable", false))
        {
            handler->SendSysMessage(ALERT_MODULE_DISABLED);
            return true;
        }
        TrialOperation::imbueTrial(handler->GetPlayer());
        return true;
    }

private:
    enum moduleStrings
    {
        ALERT_MODULE_DISABLED = 40051
    };
};

void Add_class_trial()
{
    new TrialOperation();
    new TrialAnnounce();
}

void AddSC_class_trial_commandscript()
{
    new TrialCommands();
}
