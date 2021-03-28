//lib.sqf

if (canSuspend) exitWith {diag_log "ERROR! Mafia initialization called from scheduled script";};

mafia_invoker_ok = false;

MAFIA_DUMMY = [1,2,3];

mafia_fnc_exportOpList = {
    private _version = format["%1 %2.%3 - %4", (productVersion select 0), (productVersion select 2), (productVersion select 3), (productVersion select 4)];
    "mafia" callExtension ("export_ptr_list:" + _version);
};

mafia_fnc_callWrapper = {
    scopeName "main";
    params ["_args", "_code"];
    private _res = [_x] apply {_args call _code} select 0;
    missionNamespace setVariable ["MAFIA_CALL_RETURN", _res];
};

mafia_fnc_isNilWrapper = {
    (missionNamespace getVariable "MAFIA_CALL_ARGS") params ["_args", "_code"];
    missionNamespace setVariable ["MAFIA_CALL_RETURN", if (isNil "_args") then {call _code} else {_args call _code}];
};

diag_log text "Mafia Invoker SQF handler initializing...";
private _res = "mafia" callExtension "init_invoker:";
diag_log text format["Mafia Invoker SQF handler initializing result: %1", _res];

//Check if invoker is working
private _res = "mafia" callExtension "test_invoker:";
if(_res == profileNameSteam) then {
    mafia_invoker_ok = true;
};
diag_log text format["Mafia Invoker test result: %1 == %2", _res, profileNameSteam];


if(mafia_invoker_ok) then {
    mafia_fnc_signal = compileFinal preProcessFileLineNumbers "\x\mafia\core\signal.sqf";

    addMissionEventHandler ["EachFrame", "mafiaOnFrame"]; //Register our PFH

    diag_log text "Mafia Invoker initialized.";
    diag_log text format["Mafia Pre-Init..."];
    ["pre_pre_init",[]] call (uiNamespace getVariable "mafia_fnc_event");
    diag_log text format["Mafia Pre-Init Completed."];
} else {
    diag_log text "Mafia Invoker initialization failed.";
    mafia_fnc_signal =  compileFinal "";

    if ((uiNamespace getVariable ["mafia_fnc_event", scriptNull]) isEqualType scriptNull) then {
        uiNamespace setVariable ["mafia_fnc_event", compileFinal ""];
    };
};
