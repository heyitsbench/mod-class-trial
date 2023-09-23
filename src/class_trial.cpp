#include "class_trial_loader.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class TrialOperation : public PlayerScript {
public:
    TrialOperation() : PlayerScript("TrialOperation") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("AnnounceEnable", true))
            ChatHandler(player->GetSession()).SendSysMessage(ALERT_MODULE_PRESENCE);

        if (!isTrialCharacter(player))
            return; // Not a class trial, don't do anything.
        if (sConfigMgr->GetOption<bool>("AllowCustomize", true))
            player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
        if (sConfigMgr->GetOption<bool>("AllowRaceChange", true))
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
        if (sConfigMgr->GetOption<uint32>("TrialPlaytimeLimit", 0) > player->m_Played_time[PLAYED_TIME_TOTAL])
            trialDelete(player);
        return;
    };

    static void imbueTrial(Player* player)
    {
        enlistTrial(player);
    }

    static void enlistTrial(Player* player)
    {
        WorldDatabase.Execute("INSERT INTO mod_classtrial_characters (GUID) VALUES ({})", player->GetGUID().ToString());
        LOG_DEBUG("module", "Enlisted character {} with a class trial.", player->GetGUID().ToString());
    }

    static bool isTrialCharacter(Player* player)
    {
        LOG_DEBUG("module", "Checking for trial status for character {}.", player->GetGUID().ToString());
        QueryResult result = WorldDatabase.Query("SELECT GUID FROM mod_classtrial_characters WHERE GUID = {}", player->GetGUID().ToString());
        if (result)
            return true;
        else
            return false;
    }

    static void trialDelete(Player* player)
    {
        uint32 account = player->GetSession()->GetAccountId();
        ObjectGuid guid = player->GetGUID();
        player->GetSession()->KickPlayer();
        player->DeleteFromDB(guid.GetCounter(), account, true, true);
    }

private:
    enum moduleStrings
    {
        ALERT_MODULE_PRESENCE = 40050
    };
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
}

void AddSC_class_trial_commandscript()
{
    new TrialCommands();
}
