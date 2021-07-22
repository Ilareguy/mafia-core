@rem Launch Arma 3 server
start "Arma3" /min /wait arma3server_x64.exe "-mod=@CBA_A3;curator;kart;heli;mark;expansion;jets;argo;orange;tacops;tank;enoch" "-serverMod=@mafia;" "-config=@mafia\server.cfg" -port=2302 -profiles=SC "-cfg=@mafia\basic.cfg" -name=SC -autoInit
