P_STRING <
STRING_PTR < 

@ /0000

        JP INICIO 
CHAR    K  /0021    ;; string a ser impressa 
        K  /0022
        K  /0023
        K  /0024
        K  /0000

INICIO  LV CHAR       ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 
        HM /00 
# INICIO  
