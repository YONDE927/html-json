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

enum JsonPhase {
    JPBEGIN,
    JPKEY,
    JPVAL,
    JPEND,
};

char JsonWords[] = {'\"',',', '{', '}', '[', ']',':'};

struct foundSymbol {
    int offset;
    char word;
};

struct JsonNode {
    char* key;
    char* textval;
    int numval;
    struct JsonNode* jsonval;
    struct JsonNode* listval;
    struct JsonNode* next;
    int indent;
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

int readSymbol(struct JsonDisc* jd, struct foundSymbol* symbol) {
    int i, j, found;

    if (jd == NULL) { return -1; }
    char* jsontext = jd->text + jd->offset;

    if (symbol == NULL) { return -1; }

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

    if (found == 0) { return -1; }
    return 0;
}

char* sliceJsonText(char* jsontext, int begin, int end) {
    char* str;
    int maxlen, length;

    if (jsontext == NULL) { return NULL; }

    maxlen = strlen(jsontext);
    if ((begin < 0) | (end < 0) | (begin > end)) { return NULL; }
    if ((begin > maxlen) | (end > maxlen)) { return NULL; }

    length = end - begin + 1;
    str = malloc(length + 1);
    if (str == NULL) { return NULL; }

    strncpy(str, jsontext + begin, length);
    str[length] = 0;
    return str;
}

struct JsonNode* readJsonNode(struct JsonDisc* jd) {
    int rc;
    struct foundSymbol symbol;

    //begin;
    rc = readSymbol(jd, &symbol);
    if (rc < 0) { return NULL; }
    if (symbol.word != '{') { return NULL; }

    //key;
    rc = readSymbol(jd, &symbol);
    if (rc < 0) { return NULL; }

    //val;

    //end;
}

int main() {
    char* sample = "{\"GlossSee\": \"markup\"}";
    struct JsonDisc* jd;
    struct foundSymbol* symbol;

    jd = constructJsonDisc(sample);
    if (jd == NULL) { exit(0); }

    symbol = readSymbol(jd);
    if (symbol == NULL) { exit(0); }
    printf("symbol: \'%c\' at %d\n", symbol->word, symbol->offset);

    symbol = readSymbol(jd);
    if (symbol == NULL) { exit(0); }
    printf("symbol: \'%c\' at %d\n", symbol->word, symbol->offset);

    symbol = readSymbol(jd);
    if (symbol == NULL) { exit(0); }
    printf("symbol: \'%c\' at %d\n", symbol->word, symbol->offset);

    symbol = readSymbol(jd);
    if (symbol == NULL) { exit(0); }
    printf("symbol: \'%c\' at %d\n", symbol->word, symbol->offset);
    
    getchar();

    return 0;
}