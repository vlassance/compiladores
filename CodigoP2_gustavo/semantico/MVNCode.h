#define  MAX_MVN_SONS 1000 
typedef struct MVN_abstrac {
    int resolved;
    Token root;
    MVN_abstrac* [MAX_MVN_SONS]; 
}
