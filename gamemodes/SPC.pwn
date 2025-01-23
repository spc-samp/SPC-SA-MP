// Bibliotecas da GM

#include <a_samp> // Biblioteca padrão do samp
#include <g_sqlite> // Biblioteca de salvamento g_sql, que salva em sqlite (por Galarc_Hale)
#include <izcmd> // Processador de comandos (originalmente por Zeex, e YashasSamaga pelo remake)
#include <samp_bcrypt> // Plugin de encriptação, utilizado nas senhas dos players
#define SSCANF_NO_NICE_FEATURES
#include <sscanf2> // Plugin de separação de strings (por Y_Less)
#include <foreach> // Biblioteca de iterators (foreach), normalmente usado como loop em ids de players logados

// Constantes para dialogs

#define Dialog_Registro				(0)
#define Dialog_Login				(1)

// Macros/Defines

#define CALLBACK::%0(%1)			forward %0(%1); \
									public %0(%1)

#define NAMESERVER					"hostname [SPC] Gamemode Base"
// Variáveis globais

enum playerInfos {

	pName[MAX_PLAYER_NAME],
	pKey[BCRYPT_HASH_LENGTH],
	pAdmin,
};

new PlayerInfo[MAX_PLAYERS][playerInfos];

new SenhaDigitada[MAX_PLAYERS][24];

main()
{
	print("\n----------------------------------");
	print(" SPC GameMode SA-MP");
	print("----------------------------------\n");
}

public OnGameModeInit()
{
	SetGameModeText("SPC Base 1.0");
	SendRconCommand(NAMESERVER);
	UsePlayerPedAnims();
	ResetVarsAdministrativo();
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746);
	return 0;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid) {

		case Dialog_Registro:{

			if(!response){ ShowDialogLogin(playerid); return 1; }
			if(!strlen(SenhaDigitada[playerid])) {

				if(!strlen(inputtext)) {

					ShowDialogRegister(playerid);
					SendClientMessage(playerid, -1, "Voce deve colocar uma senha para registrar-se");
					return 1;
				}
				if(4 > strlen(inputtext) > 24) return SendClientMessage(playerid, -1, "A senha deve ter entre 4 e 24 caracteres");
				format(SenhaDigitada[playerid], 24, inputtext);
				ShowDialogRegister2(playerid);
			}
			else {

				if(strcmp(SenhaDigitada[playerid], inputtext)) {
					
					SendClientMessage(playerid, -1, "Senhas não se coincidem! Se registre novamente");
					format(SenhaDigitada[playerid], 1, "");
					ShowDialogRegister(playerid);
					return 1;
				}
				bcrypt_hash(playerid, "OnPlayerRegister", inputtext, BCRYPT_COST);
			}
		}

		case Dialog_Login:{

			bcrypt_verify(playerid, "HashVerifyPassLogin", inputtext, PlayerInfo[playerid][pKey]);
		}
	}
	return 1;
}

stock ShowDialogRegister(playerid) {

	ShowPlayerDialog(playerid, Dialog_Registro, DIALOG_STYLE_PASSWORD, "Registrando", "Digite uma senha abaixo para se registrar", "Registrar", "Cancelar");
}

stock ShowDialogRegister2(playerid) {

	ShowPlayerDialog(playerid, Dialog_Registro, DIALOG_STYLE_PASSWORD, "Registrando", "Repita sua senha", "Registrar", "Cancelar");
}

stock ShowDialogLogin(playerid) {

	ShowPlayerDialog(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "Logando", "Digite sua senha abaixo", "Logar", "Cancelar");
}

public OnPlayerConnect(playerid)
{
	TogglePlayerSpectating(playerid, true);
	ResetVars(playerid);
	GetPlayerName(playerid, PlayerInfo[playerid][pName], MAX_PLAYER_NAME);
	if(GSQL_ValueExists("player", "nome", PlayerInfo[playerid][pName])) {

		GSQL_GetString("player", "senha", "nome", PlayerInfo[playerid][pName], PlayerInfo[playerid][pKey], BCRYPT_HASH_LENGTH);
		ShowDialogLogin(playerid);
	}
	else {

		ShowDialogRegister(playerid);
	}
	return 1;
}

CALLBACK::OnPlayerRegister(playerid) {

	new Hashed[BCRYPT_HASH_LENGTH];
	bcrypt_get_hash(Hashed);
	format(PlayerInfo[playerid][pKey], BCRYPT_HASH_LENGTH, Hashed);
	GSQL_SetString("player", "nome", PlayerInfo[playerid][pName]);
	GSQL_SetString("player", "senha", PlayerInfo[playerid][pKey], "nome", PlayerInfo[playerid][pName]);
	OnPlayerLogin(playerid);
}

CALLBACK::HashVerifyPassLogin(playerid, bool:success) {

	if(success) {

		OnPlayerLogin(playerid);
	}
	else {

		SendClientMessage(playerid, -1, "Senha incorreta!");
		ShowDialogLogin(playerid);
	}
}

CALLBACK::OnPlayerLogin(playerid) {

	CarregarDados(playerid);
	SetSpawnInfo(playerid, 0, 119, 1958.3783, 1343.1572, 15.3746, 160.0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	TogglePlayerSpectating(playerid, false);
	return 1;
}

stock CarregarDados(playerid) {

	PlayerInfo[playerid][pAdmin] = GSQL_GetInt("player", "adminnivel", "nome", PlayerInfo[playerid][pName]);
}

stock ResetVars(playerid){

	format(SenhaDigitada[playerid], 1, "");
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}


// Variaveis e constantes do Administrativo

#define MAX_VEHICLES_ADM		(20) // Maximo de veiculos criados com /veh

new veiculo_admin[MAX_VEHICLES_ADM] = INVALID_VEHICLE_ID;

// Funções do Administrativo

stock ComandoAdmin(playerid, niveladmin = 1) { // Usado em comandos admin, para saber se o player tem nivel o suficiente, ou não tem nenhum nivel

	if(PlayerInfo[playerid][pAdmin] == 0) return SendClientMessage(playerid, -1, "Você não pode usar um comando admin"), 0;
	if(PlayerInfo[playerid][pAdmin] < niveladmin) return SendClientMessage(playerid, -1, "Você não tem permissão para usar esse comando"), 0;
	return 1;
}

stock ResetVarsAdministrativo() { // Arrays do samp tem um problema, ao atribuir somente a celula 0 pega o valor, então essa função fará o trabalho que o samp não faz

	for(new i; i < MAX_VEHICLES_ADM; i++) veiculo_admin[i] = INVALID_VEHICLE_ID;
}

stock SendChatAdmin(niveladmin, color_message, const message[], { _, Float, Text3D, COLUMNTYPE, Menu, Text, DB, DBResult, bool, File, hex, bit, bit_byte, Bit }:...) {

	if(numargs() > 3) {

        new strreturn[144];
        new stringmessage[144];
        new stringarg[144];
        new length = strlen(message);
        new cantigo, ultimoc, argid = 3;
        new bool:stringachou = false;
        for(new c; c <= length; c++) {
            ultimoc = c;
            if(message[c] == '%') {

                if(c + 1 < length) {

                    switch(message[c+1]){
                        case 'i':{

                            format(stringarg, 144, "%i", getarg(argid));
                            stringachou = true;
                        }
                        case 'd':{

                            format(stringarg, 144, "%d", getarg(argid));
                            stringachou = true;
                        }
                        case 's':{

                            GetStringArg(argid, stringarg);
                            stringachou = true;
                        }
                        case 'f':{

                            format(stringarg, 144, "%f", getarg(argid));
                            stringachou = true;
                        }
                        case 'c':{

                            format(stringarg, 144, "%c", getarg(argid));
                            stringachou = true;
                        }
                        case 'x':{

                            format(stringarg, 144, "%x", getarg(argid));
                            stringachou = true;
                        }
                        case 'b':{

                            format(stringarg, 144, "%b ", getarg(argid));
                            stringachou = true;
                        }
                        case 'q':{

                            format(stringarg, 144, "%q", getarg(argid));
                            stringachou = true;
                        }
                        case '%':{
                            format(stringarg, 144, "%%");
                            stringachou = true;
                        }
                    }
                }
                if(stringachou){

                    strmid(stringmessage, message, cantigo, c);
                    strcat(strreturn, stringmessage);
                    strcat(strreturn, stringarg);
                    cantigo = c+2;
                    argid++;
                    stringachou = false;
                    c++;
                }
            }
        }
        if(cantigo < ultimoc){
            strmid(stringmessage, message, cantigo, ultimoc+1);
            strcat(strreturn, stringmessage);
        }
		foreach(new i: Player) {
			if(PlayerInfo[i][pAdmin] >= niveladmin) SendClientMessage(i, color_message, strreturn);
		}
	}
	else {

		foreach(new i: Player) {
			if(PlayerInfo[i][pAdmin] >= niveladmin) SendClientMessage(i, color_message, message);
		}
	}
}

// Comandos

CMD:pegaradmin(playerid) {

	PlayerInfo[playerid][pAdmin] = 10;
	GSQL_SetInt("player", "adminnivel", PlayerInfo[playerid][pAdmin], "nome", PlayerInfo[playerid][pName]);
	return 1;
}

CMD:veh(playerid, params[]) {

	if(!ComandoAdmin(playerid, 4)) return 1;
	new vehmodel;
	if(sscanf(params, "i", vehmodel)) return SendClientMessage(playerid, -1, "MODO DE USO: /veh [Modelo de veiculo]");
	if(400 > vehmodel > 611) return SendClientMessage(playerid, -1, "ERRO | Use modelos entre 400 e 611");
	new Float:X, Float:Y, Float:Z, Float:A;

	GetPlayerPos(playerid, X, Y, Z);
	GetPlayerFacingAngle(playerid, A);

	for(new i; i < MAX_VEHICLES_ADM; i++) {
		if(veiculo_admin[i] == INVALID_VEHICLE_ID) {

			veiculo_admin[i] = CreateVehicle(vehmodel, X, Y, Z, A, -1, -1, -1);
			return 1;
		}
	}
	SendClientMessage(playerid, -1, "Número máximo de veículos admin atingidos!");
	return 1;
}

CMD:destruirvehs(playerid) {

	if(!ComandoAdmin(playerid, 6)) return 1;
	new quantiavehs;
	for(new i; i < MAX_VEHICLES_ADM; i++) {

		if(veiculo_admin[i] != INVALID_VEHICLE_ID) {
			DestroyVehicle(veiculo_admin[i]);
			veiculo_admin[i] = INVALID_VEHICLE_ID;
			quantiavehs++;
		}
	}
	SendChatAdmin(1, -1, "O Admin %s destruiu todos os veiculos admin criados (%d)", PlayerInfo[playerid][pName], quantiavehs);
	return 1;
}