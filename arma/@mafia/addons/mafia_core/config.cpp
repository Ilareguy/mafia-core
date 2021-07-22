//config.cpp
class CfgPatches {
    class Mafia_Core {
        name = "Mafia — Core";
        units[] = {};
        weapons[] = {};
        requiredVersion = 1.98;
        requiredAddons[] = {
            "A3_Data_F_Loadorder",
            "A3_Data_F_Curator_Loadorder",
            "A3_Data_F_Kart_Loadorder",
            "A3_Data_F_Bootcamp_Loadorder",
            "A3_Data_F_Heli_Loadorder",
            "A3_Data_F_Mark_Loadorder",
            "A3_Data_F_Exp_A_Loadorder",
            "A3_Data_F_Exp_B_Loadorder",
            "A3_Data_F_Exp_Loadorder",
            "A3_Data_F_Jets_Loadorder",
            "A3_Data_F_Argo_Loadorder",
            "A3_Data_F_Patrol_Loadorder",
            "A3_Data_F_Orange_Loadorder",
            // CBA
            "cba_xeh"
        };
        version = 0.1;
    };
};

class CfgFunctions {
	init = "x\mafia\core\initFunctionsWrapper.sqf";
	//init = "A3\functions_f\initFunctions.sqf";
};

/*
class Mafia {
    class example_dll_project {
        class example_dll_module {
            pluginName = "example_dll";
        };
    };
};
*/

class Extended_PreStart_EventHandlers {
    class Mafia_Core {
        init = "['post_start', _this] call (uiNamespace getVariable 'mafia_fnc_event');";
    };
};

class Extended_PreInit_EventHandlers {
    class Mafia_Core {
        init = "['pre_init',[]] call (uiNamespace getVariable 'mafia_fnc_event');";
    };
};

class Extended_PostInit_EventHandlers {
    class Mafia_Core {
        init = "call compile preprocessFileLineNumbers '\x\mafia\core\post_init.sqf';";
    };
};

class Extended_DisplayUnload_EventHandlers {
    class RscDisplayMission {
        Mafia_MissionEnded = "['mission_ended', []] call (uiNamespace getVariable 'mafia_fnc_event');";
    };
};
