# Gamemode base para SA-MP da SPC

## Esta é uma GameMode básica, ou seja, somente com os sistemas básicos e "obrigatórios" em que todos os servidores tem por padrão.
## Nessa GM, atualmente, poderemos ver alguns sistemas feitos do 0 por nossa equipe. Estes sistemas estão listados abaixo:

### 1: Sistema de login e registro em dialog, salvamento no sqlite (usando a include g_sqlite) e encriptação de senha por bcrypt;
### 2: Sistema básico de admins, com algumas funções adicionadas e comandos;
### 2.1: Função ComandoAdmin. É usada em comandos destinado para administradores. Essa função já envia mensagens de erro de acordo com o nivel de admin requisitado ao comando;
### 2.2: Função SendChatAdmin. É utilizada para enviar mensagens para players que são admins de um respectivo nível acima. É possível formatar dentro dessa mesma função, ex:
```
SendChatAdmin(1, 0xADD8E6FF, "O Admin %s destruiu os veículos do servidor", PlayerInfo[playerid][pName]);
```
#### Onde somente os 3 primeiros argumentos são obrigatórios (1° nível de admin minimo para ver a mensagem, 2° a cor da mensagem, 3° a mensagem em texto) os outros argumentos serão somente para
#### as placeholders, caso haja;
### 2.3: Função ResetVarsAdministrativo. Somente para resetar variáveis globais usadas no sistema administrativo;
### 2.4: Comando /veh. Utilizado para criar um veículo admin no servidor (/veh [modelo do veículo]) ex: /veh 411 (criará um infernus);
### 2.5: Comando /destruirvehs. Utilizado para destruir todos os veículos criados no servidor com /veh.

## Também há includes e plugins externos de outras autorias, sendo eles:

### 1: izcmd. Include com código atualizado a partir do antigo zcmd (feito por ZeeX). Link: https://github.com/YashasSamaga/I-ZCMD;
### 2: samp_bcrypt. Plugin de encriptação usando o bcrypt para o samp. Link: https://github.com/Sreyas-Sreelal/samp-bcrypt;
### 3: sscanf. Plugin de separação de strings (semelhante ao padrão do c). Link: https://github.com/Y-Less/sscanf;
### 4: Foreach. Biblioteca de Iterators para samp, em sua maioria é utilizado para fazer loop nos ids de players conectados. Link: https://github.com/karimcambridge/samp-foreach

## Configurações na gm
### Atualmente, existe apenas uma "configuração" na gm, que é o macro de NAMESERVER, que muda o nome do servidor ao ser ligado (envia o comando no console hostname [SPC] Gamemode Base,
### sendo que a partir do primeiro espaço após o hostname, será definido o nome do servidor, neste caso o nome será "[SPC] Gamemode Base"

## Compilador
### Atualmente é usado o compilador padrão do samp para compilar a GM.
