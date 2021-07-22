/*
 * Author: esteldunedain
 * Sends a signal to a Mafia extension
 *
 * Arguments:
 * 0: extension name <STRING>
 * 1: Signal name <SRING>
 * 2: Parameters <ANY>
 *
 * Return value:
 * Success <BOOL>
 *
 * Example:
 * ["z\mafia\build\win32\example_frag\RelWithDebInfo\example_frag.dll", "enableFrag", myParameters] call mafia_fnc_signal
 *
 * Public: No
 *
 */
params ["_extensionName", "_signalName", "_parameters"];

if !(mafia_invoker_ok) exitWith {false};

[_extensionName,_signalName] mafiaSignal _parameters;

//mafia_signal_var set[0, _parameters];
//"mafia" callExtension format ["signal:%1,%2",_extensionName,_signalName];
