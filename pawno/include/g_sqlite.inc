#if defined G_SQLITE
    #endinput
#else
    #define G_SQLITE
#endif

#if !defined _INC_a_sampdb
    #include <a_sampdb>
#endif

#if !defined OPEN_MP_CONST_CORRECT
    #define DB_Open db_open
    #define DB_Close db_close
    #define DB_FreeResultSet db_free_result
    #define DB_GetRowCount db_num_rows
    #define DB_GetFieldIntByName db_get_field_assoc_int
    #define DB_GetFieldFloatByName db_get_field_assoc_float
    #define DB_GetFieldStringByName db_get_field_assoc
    // Macro pra getar um argumento string (em fun苺es, retirado do https://github.com/GRGServer/SAMP/tree/master)
    #define GetStringArg(%0,%1); \
    for (new i_%0 = 0; ; ++i_%0) \
    { \
        if ((%1[i_%0] = getarg((%0), i_%0)) != EOS) \
            continue; \
        else \
            break; \
    }
    #define MAX_STR_QUERY_LEN 200   // Tamanho maximo de strings no db_executequery
    
    stock DBResult:DB_ExecuteQuery(DB:db, query[], { _, Float, Text3D, COLUMNTYPE, Menu, Text, DB, DBResult, bool, File, hex, bit, bit_byte, Bit }:...){

        if(numargs() > 2){
            new strreturn[MAX_STR_QUERY_LEN];
            new stringquery[MAX_STR_QUERY_LEN];
            new stringarg[MAX_STR_QUERY_LEN];
            new length = strlen(query);
            new cantigo, ultimoc, argid = 2;
            new bool:stringachou = false;
            for(new c; c <= length; c++) {
                ultimoc = c;
                if(query[c] == '%') {

                    if(c + 1 < length) {

                        switch(query[c+1]){
                            case 'i':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%i", getarg(argid));
                                stringachou = true;
                            }
                            case 'd':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%d", getarg(argid));
                                stringachou = true;
                            }
                            case 's':{

                                GetStringArg(argid, stringarg);

                                stringachou = true;
                            }
                            case 'f':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%f", getarg(argid));
                                stringachou = true;
                            }
                            case 'c':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%c", getarg(argid));
                                stringachou = true;
                            }
                            case 'x':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%x", getarg(argid));
                                stringachou = true;
                            }
                            case 'b':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%b ", getarg(argid));
                                stringachou = true;
                            }
                            case 'q':{

                                format(stringarg, MAX_STR_QUERY_LEN, "%q", getarg(argid));
                                stringachou = true;
                            }
                            case '%':{
                                format(stringarg, MAX_STR_QUERY_LEN, "%%");
                                stringachou = true;
                            }
                        }
                    }
                    if(stringachou){

                        strmid(stringquery, query, cantigo, c);
                        strcat(strreturn, stringquery);
                        strcat(strreturn, stringarg);
                        cantigo = c+2;
                        argid++;
                        stringachou = false;
                        c++;
                    }
                }
            }
            if(cantigo < ultimoc){
                strmid(stringquery, query, cantigo, ultimoc+1);
                strcat(strreturn, stringquery);
            }
            return db_query(db, strreturn);
        }

        else return db_query(db, query);
    }
#else
    #define COLUMNTYPE CUSTOM_TAG_TYPES
#endif

#define GSQL_VALUE_INT      COLUMNTYPE:"INTEGER"
#define GSQL_VALUE_FLOAT      COLUMNTYPE:"REAL"
#define GSQL_VALUE_STRING      COLUMNTYPE:"TEXT"
#define GSQL_VALUE_BOOL      COLUMNTYPE:"INTEGER"

new DB:g_DB;

public OnGameModeInit(){

	if((g_DB = DB_Open("database.db")) == DB:0) {
		new File:f = fopen("database.db", io_append);
        fclose(f);
	}
    print("|---------------------------------------------------|");
    print("|         Include g_sqlite por Galarc_Hale          |");
    print("|             carregada com sucesso                 |");
    print("|---------------------------------------------------|");
    return GSQL_OnGameModeInit();
}

public OnGameModeExit(){

    DB_Close(g_DB);
    print("|---------------------------------------------------|");
    print("|           Include g_sqlite desligada              |");
    print("|---------------------------------------------------|");
    return GSQL_OnGameModeExit();
}

stock bool:GSQL_TableExists(const table[]){

    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT * FROM `%s` LIMIT 1;", table);
    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        return false;
    }
    DB_FreeResultSet(query_result);
    return true;
}

stock bool:GSQL_ColumnExists(const table[], const column[]){

    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` LIMIT 1;", column, table);
    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        return false;
    }
    DB_FreeResultSet(query_result);
    return true;
}

stock bool:GSQL_ValueExists(const table[], const column[], const value[]){

    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` WHERE %s = '%s' LIMIT 1;", column, table, column, value);
    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        return false;
    }
    DB_FreeResultSet(query_result);
    return true;
}

stock GSQL_CreateTable(const table[]){

    DB_FreeResultSet(DB_ExecuteQuery(g_DB, "CREATE TABLE %s(id INTEGER PRIMARY KEY AUTOINCREMENT);", table));
    DB_FreeResultSet(DB_ExecuteQuery(g_DB, "INSERT INTO %s DEFAULT VALUES;", table));
}

stock GSQL_CreateColumn(const table[], const column[], const COLUMNTYPE:valuetype[]){

    DB_FreeResultSet(DB_ExecuteQuery(g_DB, "ALTER TABLE %s ADD COLUMN '%s' %s DEFAULT 0;", table, column, valuetype));
}

stock GSQL_SetInt(const table[], const column[], value, const wherecolumn[] = "nullgsql", const where[] = "nullgsql"){

    if(!GSQL_TableExists(table)) GSQL_CreateTable(table);

    if(!GSQL_ColumnExists(table, column)) GSQL_CreateColumn(table, column, GSQL_VALUE_INT);

    new cmp_whc = strcmp(wherecolumn, "nullgsql"), cmp_wh = strcmp(where, "nullgsql");

    if(!cmp_whc && !cmp_wh) { // Caso n達o tenha uma clausula where, apenas para adicionar um valor qualquer

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "INSERT INTO %s (%s) VALUES (%d);", table, column, value));
        return 1;
    }
    else if(cmp_whc && cmp_wh) { // Caso tenha a clausula where, ou seja, update

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "UPDATE %s SET `%s` = %d WHERE `%s` = '%s';", table, column, value, wherecolumn, where));
        return 1;
    }
    else{
        print("Erro na sintaxe de funcao (GSQL_SetInt)");
        return 0;
    }
}

stock GSQL_SetString(const table[], const column[], const value[], const wherecolumn[] = "nullgsql", const where[] = "nullgsql"){

    if(!GSQL_TableExists(table)) GSQL_CreateTable(table);

    if(!GSQL_ColumnExists(table, column)) GSQL_CreateColumn(table, column, GSQL_VALUE_STRING);

    new cmp_whc = strcmp(wherecolumn, "nullgsql"), cmp_wh = strcmp(where, "nullgsql");

    if(!cmp_whc && !cmp_wh) { // Caso n達o tenha uma clausula where, apenas para adicionar um valor qualquer

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "INSERT INTO %s (%s) VALUES ('%s');", table, column, value));
    }
    else if(cmp_whc && cmp_wh) { // Caso tenha a clausula where, ou seja, update

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "UPDATE %s SET `%s` = '%s' WHERE `%s` = '%s';", table, column, value, wherecolumn, where));
    }
    else print("Erro na sintaxe de funcao (GSQL_SetString)");
}

stock GSQL_SetFloat(const table[], const column[], Float:value, const wherecolumn[] = "nullgsql", const where[] = "nullgsql"){

    if(!GSQL_TableExists(table)) GSQL_CreateTable(table);

    if(!GSQL_ColumnExists(table, column)) GSQL_CreateColumn(table, column, GSQL_VALUE_FLOAT);

    new cmp_whc = strcmp(wherecolumn, "nullgsql"), cmp_wh = strcmp(where, "nullgsql");

    if(!cmp_whc && !cmp_wh) { // Caso n達o tenha uma clausula where, apenas para adicionar um valor qualquer

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "INSERT INTO %s (%s) VALUES (%f);", table, column, value));
    }
    else if(cmp_whc && cmp_wh) { // Caso tenha a clausula where, ou seja, update

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "UPDATE %s SET `%s` = '%f' WHERE `%s` = '%s';", table, column, value, wherecolumn, where));
    }
    else print("Erro na sintaxe de funcao (GSQL_SetFloat)");
}

stock GSQL_SetBool(const table[], const column[], bool:value, const wherecolumn[] = "nullgsql", const where[] = "nullgsql"){

    if(!GSQL_TableExists(table)) GSQL_CreateTable(table);

    if(!GSQL_ColumnExists(table, column)) GSQL_CreateColumn(table, column, GSQL_VALUE_BOOL);

    new cmp_whc = strcmp(wherecolumn, "nullgsql"), cmp_wh = strcmp(where, "nullgsql");

    if(!cmp_whc && !cmp_wh) { // Caso n達o tenha uma clausula where, apenas para adicionar um valor qualquer

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "INSERT INTO %s (%s) VALUES (%b);", table, column, value));
    }
    else if(cmp_whc && cmp_wh) { // Caso tenha a clausula where, ou seja, update

        DB_FreeResultSet(DB_ExecuteQuery(g_DB, "UPDATE %s SET `%s` = '%b' WHERE `%s` = '%s';", table, column, value, wherecolumn, where));
    }
    else print("Erro na sintaxe de funcao (GSQL_SetBool)");
}

stock GSQL_GetInt(const table[], const column[], const wherecolumn[], const where[]){

    if(!GSQL_TableExists(table)) {

        printf("Tabela %s nao criada! erro no GSQL_GetInt", table);
        return 0;
    }

    if(!GSQL_ColumnExists(table, column)) {
        
        printf("Coluna %s nao criada! erro no GSQL_GetInt", column);
        return 0;
    }
    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` WHERE `%s` = '%s' LIMIT 1;", column, table, wherecolumn, where);

    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        printf("Erro na sintaxe GSQL_GetInt (tabela:%s, coluna: %s, wherecoluna: %s, where: %s)", table, column, wherecolumn, where);
        return 0;
    }
    else{

        new intrtn = DB_GetFieldIntByName(query_result, column);
        DB_FreeResultSet(query_result);
        return intrtn;
    }
}

stock GSQL_GetString(const table[], const column[], const wherecolumn[], const where[], dest[], sizedest = sizeof dest){

    if(!GSQL_TableExists(table)) {

        printf("Tabela %s nao criada! erro no GSQL_GetString", table);
        return 0;
    }
    if(!GSQL_ColumnExists(table, column)) {

        printf("Coluna %s nao criada! erro no GSQL_GetString", column);
        return 0;
    }
    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` WHERE `%s` = '%s' LIMIT 1;", column, table, wherecolumn, where);

    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        printf("Erro na sintaxe GSQL_GetString (tabela:%s, coluna: %s, wherecoluna: %s, where: %s)", table, column, wherecolumn, where);
        return 0;
    }
    else{

        DB_GetFieldStringByName(query_result, column, dest, sizedest);
        DB_FreeResultSet(query_result);
        return 1;
    }
}

stock Float:GSQL_GetFloat(const table[], const column[], const wherecolumn[], const where[]){

    if(!GSQL_TableExists(table)) {

        printf("Tabela %s nao criada! erro no GSQL_GetFloat", table);
        return 0.0;
    }

    if(!GSQL_ColumnExists(table, column)) {

        printf("Coluna %s nao criada! erro no GSQL_GetFloat", column);
        return 0.0;
    }

    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` WHERE `%s` = '%s' LIMIT 1;", column, table, wherecolumn, where);

    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        printf("Erro na sintaxe GSQL_GetFloat (tabela:%s, coluna: %s, wherecoluna: %s, where: %s)", table, column, wherecolumn, where);
        return 0.0;
    }
    else{

        new Float:floatrtn = DB_GetFieldFloatByName(query_result, column);
        DB_FreeResultSet(query_result);
        return floatrtn;
    }
}

stock bool:GSQL_GetBool(const table[], const column[], const wherecolumn[], const where[]){

    if(!GSQL_TableExists(table)) {

        return !printf("Tabela %s nao criada! erro no GSQL_GetBool", table);
    }

    if(!GSQL_ColumnExists(table, column)) {

        return !printf("Coluna %s nao criada! erro no GSQL_GetBool", column);
    }
    new DBResult:query_result;
    query_result = DB_ExecuteQuery(g_DB, "SELECT `%s` FROM `%s` WHERE `%s` = '%s' LIMIT 1;", column, table, wherecolumn, where);

    if(DB_GetRowCount(query_result) < 1){

        DB_FreeResultSet(query_result);
        return !printf("Erro na sintaxe GSQL_GetBool (tabela:%s, coluna: %s, wherecoluna: %s, where: %s)", table, column, wherecolumn, where);
    }
    else{

        new bool:boolrtn = bool:DB_GetFieldIntByName(query_result, column);
        DB_FreeResultSet(query_result);
        return boolrtn;
    }
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit GSQL_OnGameModeInit
forward GSQL_OnGameModeInit();

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit GSQL_OnGameModeExit
forward GSQL_OnGameModeExit();