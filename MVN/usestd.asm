P_STRING <
STRING_PTR < 
P_INT_ZERO < 
TO_BE_PRINTED <
@ /0000

         JP INICIO 
CHAR     K  /0021    ;; string a ser impressa 
         K  /0022
         K  /0023
         K  /0024
         K  /0000
CHAR_SPC K  /0020 
         K  /0000 
        
INT_1   K  /FFFF
INT_2   K  =5000
INT_3   K  =345
INT_4   K  =12
INT_5   K  =1

INICIO  LV CHAR       ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        LD INT_1 
        MM TO_BE_PRINTED 
        SC P_INT_ZERO
        ;; IMPRIME ESPACO 
        LV CHAR_SPC   ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 


        LD INT_2
        MM TO_BE_PRINTED 
        SC P_INT_ZERO

        ;; IMPRIME ESPACO 
        LV CHAR_SPC   ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        LD INT_3
        MM TO_BE_PRINTED 
        SC P_INT_ZERO
  

        ;; IMPRIME ESPACO 
        LV CHAR_SPC   ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        LD INT_4
        MM TO_BE_PRINTED 
        SC P_INT_ZERO


        ;; IMPRIME ESPACO 
        LV CHAR_SPC   ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        LD INT_5
        MM TO_BE_PRINTED 
        SC P_INT_ZERO


        HM /00 
# INICIO  
