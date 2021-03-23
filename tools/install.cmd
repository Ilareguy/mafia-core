@rem Install development files to the Arma 3 directory
Xcopy /E /I /Y "arma\@mafia\*" "%ARMA3_SERVER_DIR%\@mafia\"
Xcopy /E /I /Y "arma\MPMissions\*" "%ARMA3_SERVER_DIR%\MPMissions\"

@rem Pack PBOs
cd "%ARMA3_SERVER_DIR%"
PBOConsole.exe -pack "@mafia\addons\mafia_core" "@mafia\addons\mafia_core.pbo"
PBOConsole.exe -pack "MPMissions\mafia.VR" "MPMissions\mafia.VR.pbo"
