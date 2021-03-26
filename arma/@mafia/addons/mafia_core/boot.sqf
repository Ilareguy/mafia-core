//boot.sqf
mafia_params_var = [];
diag_log text format["Initializing Mafia"];
private _res = "mafia" callExtension "init:";
diag_log text format["Mafia initialization part 1/3: %1", _res];
_res = "mafia" callExtension format["init_patch:%1", (productVersion select 3)]; // find a patch
"mafia" callExtension "invoker_begin_register:";

_registerTypesResult = (call compile "mafiaRegisterTypes parsingNamespace") param [0, false];

diag_log text format["Mafia initialization part 2/3: %1", _registerTypesResult];

/*private _mafia_projects = configFile >> "Mafia";
for "_i" from 0 to (count _mafia_projects)-1 do {
    private _project = _mafia_projects select _i;
    if(isClass _project) then {
        for "_x" from 0 to (count _project)-1 do {
            private _module = _project select _x;
            if(isClass _module) then {
                private _plugin_name = getText(_module >> "pluginName");
                if(_plugin_name != "") then {
                    diag_log text format["Mafia Loading Persistent Plugin: %1", _plugin_name];
                    _cert = getText(_module >> "certificate");
                    if (_cert != "") then {
                        "mafia" callExtension ("persistent_module:" + _plugin_name+","+_cert);
                    } else {
                        "mafia" callExtension ("persistent_module:" + _plugin_name);
                    };
                };
            };
        };
    };
};

"mafia" callExtension "load_modules:";*/

if (_registerTypesResult) then {
    uiNamespace setVariable ["mafia_fnc_event", compileFinal preprocessFileLineNumbers "\x\mafia\core\event.sqf"];
} else {
    uiNamespace setVariable ["mafia_fnc_event", compileFinal ""];
};

["pre_start",[]] call (uiNamespace getVariable "mafia_fnc_event");

_res = "mafia" callExtension "invoker_end_register:";

diag_log text format["Mafia initialization part 3/3: %1", _res];
