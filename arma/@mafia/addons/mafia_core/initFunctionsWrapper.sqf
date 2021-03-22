if (!call (uiNamespace getVariable ["MAFIA_BOOT_DONE",{false}])) then {
    #include "\x\mafia\core\boot.sqf";
    uiNamespace setVariable ['MAFIA_BOOT_DONE', compileFinal 'true'];
};

if (!isNil {_this}) then  {
    _this call compile preProcessFileLineNumbers "\A3\functions_f\initFunctions.sqf";
} else {
    call compile preprocessFileLineNumbers '\x\mafia\core\lib.sqf';
    call compile preProcessFileLineNumbers "\A3\functions_f\initFunctions.sqf";
};
