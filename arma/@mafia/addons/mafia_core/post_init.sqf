//post_init.sqf

if !(mafia_invoker_ok) exitWith {};

diag_log text format["Mafia Post-Init..."];
["post_init",[]] call (uiNamespace getVariable "mafia_fnc_event");
diag_log text format["Mafia Post-Init Finished..."];