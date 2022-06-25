#include <stdio.h>

#ifdef 	_WIN64
#include <windows.h>
#define sleep Sleep
#endif

enum JsonNodeType {
    JNTNONE,
    JNTTEXT,
    JNTJSON,
    JNTLIST,
};

enum JsonConType {
    JCTNONE,
    JCTNEXT,
    JCTEND,
    JCTLISTEND
};

char JsonWords[] = {'\"',',', '{', '}', '[', ']',':'};

struct foundSymbol {
    int offset;
    char word;
};

struct JsonNode {
    int indent;
    char* data;
    struct JsonNode* key;
    struct JsonNode* value;
    struct JsonNode* next;
};

struct Json {
    struct JsonNode* head;    
};

struct JsonDisc {
    int offset;
    int size;
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
    jd->size = strlen(jsontext);
    
    jd->json = malloc(sizeof(struct Json));
    if(jd->json == NULL){
        puts("malloc Json fail");
        return NULL;
    }

    return jd;
}

struct foundSymbol* readSymbol(struct JsonDisc* jd) {
    int i, j, found;

    if (jd == NULL) { return NULL; }
    char* jsontext = jd->text + jd->offset;

    struct foundSymbol* symbol = malloc(sizeof(struct foundSymbol));
    if (symbol == NULL) { return NULL; }

    found = 0;
    for (i = 0; i < (jd->size - jd->offset); i++) {
        if (found) { break; }
        for (j = 0; j < 7; j++) {
            if (jsontext[i] == JsonWords[j]) {
                symbol->offset = i + jd->offset;
                symbol->word = jsontext[i];
                found = 1;
            }
        }
    }

    jd->offset += i + 1;

    if (found == 0) {
        free(symbol);
        return NULL;
    }
    return symbol;
}