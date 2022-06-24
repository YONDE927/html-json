#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    begin = findChar('\"', jsontext);
    if(begin < 0){ return NULL;}
    end = findChar('\"', jsontext + begin);
    if(end < 0){ return NULL;}

    jd->offset = end + 1;

    keylength = end - begin - 1;
    key = malloc(keylength);
    bzero(key, keylength);
    strncpy(key, jsontext + begin + 1, keylength);
    
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
    begin = findChar('\"', jsontext);
    if(begin < 0){ return NULL;}
    end = findChar('\"', jsontext + begin);
    if(end < 0){ return NULL;}
    vallength = end - begin - 1;
    value = malloc(vallength);
    bzero(value, vallength);
    strncpy(value, jsontext + begin + 1, vallength);
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
