poll_config: primaire.exe secondaire.exe trafic.exe
	gcc poll_config.c -o poll_config.exe
primaire.exe:primaire.c
	gcc  primaire.c -o primaire.exe
secondaire.exe :secondaire.c
	gcc  secondaire.c -o secondaire.exe
trafic.exe :trafic.c
	gcc  trafic.c -o trafic.exe
