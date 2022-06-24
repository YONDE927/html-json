#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef 	_WIN64
#include <windows.h>
#define sleep Sleep
#endif

struct JsonNode {
    char* key;
    char* value;
    struct JsonNode* next;
    struct JsonNode* child;
};

struct Json {
    struct JsonNode* head;    
};

struct JsonDisc {
    int offset;
    char* text;
    struct Json* json;
};

struct JsonDisc* constructJsonDisc(char* jsontext){
    struct JsonDisc* jd;

    jd = malloc(sizeof(struct JsonDisc));
    if(jd == NULL){
        puts("malloc JsocDisc fail");
        return NULL;
    }

    jd->offset = 0;
    jd->text = jsontext;
    
    jd->json = malloc(sizeof(struct Json));
    if(jd->json == NULL){
        puts("malloc Json fail");
        return NULL;
    }

    return jd;
}

int findChar(char key, char* text){
    int length;

    length = strlen(text);
    for(int i = 0; i < length; i++){
        if(text[i] == key){
            return i;
        }
    }
    return -1;
}

char* readJsonKey(struct JsonDisc* jd){
    int length, keylength;
    int begin, end;
    char* key, *jsontext;

    jsontext = jd->text + jd->offset;

    length = strlen(jsontext);

    //キーの最初を見つける
    begin = findChar('\"', jsontext) + 1;
    if(begin < 0){ return NULL;}
    end = findChar('\"', jsontext + begin) - 1 + begin;
    if(end < 0){ return NULL;}

    jd->offset = end + 1;

    keylength = end - begin + 1;
    key = malloc(keylength + 1);
    if(key == NULL){
        return NULL;
    }
    memset(key, 0, keylength + 1);
    strncpy(key, jsontext + begin, keylength);

    return key;
}

int typeJsonValue(struct JsonDisc* jd){
    int length;
    int flag;
    char* jsontext;
    
    jsontext = jd->text + jd->offset;
    length = strlen(jsontext);
    //"か{のどちらかを先に見つける
    flag = 0;
    for(int i = 0; i < length; i++){
        if(jsontext[i] == '\"'){
            flag = 1;
            break;
        }
        if(jsontext[i] == '{'){
            flag = 2;
            break;
        }
    }
    return flag;
}

int typeJsonContinuousValue(struct JsonDisc* jd){
    int length;
    int flag;
    char* jsontext;
    
    jsontext = jd->text + jd->offset;
    length = strlen(jsontext);
    //"か{のどちらかを先に見つける
    flag = 0;
    for(int i = 0; i < length; i++){
        if(jsontext[i] == ','){
            flag = 1;
            break;
        }
        if(jsontext[i] == '}'){
            flag = 2;
            break;
        }
    }
    return flag;
}

char* readJsonValue(struct JsonDisc* jd){
    int length, vallength;
    int begin, end, flag;
    char* value, *jsontext;
    
    jsontext = jd->text + jd->offset;
    length = strlen(jsontext);
    //普通の文字列の場合
    begin = findChar('\"', jsontext) + 1;
    if(begin < 0){ return NULL;}
    end = findChar('\"', jsontext + begin) - 1 + begin;
    if(end < 0){ return NULL;}
    vallength = end - begin - 1;
    value = malloc(vallength + 1);
    if (value == NULL) {
        return NULL;
    }
    memset(value, 0, vallength + 1);
    strncpy(value, jsontext + begin, vallength);
    value[vallength] = 0;
    jd->offset = end + 1;
    return 0;
}

int main(){
    char* samplejson = "{\n\"glossary\": \"hello\"}";
    char* key;
    struct JsonDisc* jd;


    jd = constructJsonDisc(samplejson);
    if(jd == NULL){ return -1;}
    key = readJsonKey(jd);
    printf("key: %s\n", key);
    return 0;
}
