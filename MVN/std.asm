; Biblioteca de ambiente
; ======================

; PRINT ASCII
; PONEASCII <adrr>
PONEASCII > 
TO_BE_PRINTED >
P_STRING >
STRING_PTR >
; relocavel
& /0000
; alocando para K
TO_BE_PRINTED       K /0000 
TO_BE_PRINTED_TMP   K /0000 
LOAD_CONST          K /8000           ; instruction to load 
ZERO                K /0000 
TWO                 K /0002
STRING_PTR          K /0000           ; ponteiro para o comeco da string 

; inicio da rotina
PONEASCII           JP /000           ; alocando para o endereço de retorno  
PONEASCII_INIT      LD TO_BE_PRINTED  ; carregar a variavel 
                    PD /100           ; imprimir na tela 
                    RS PONEASCII      ; retornar a rotina  

;; ****  P_STRING &STRING_PTR ****
;;   Imprime a string apontada por STRING_PTR ate
;; o caractere /000  

P_STRING            JP /000           ; endereço de retorno 
PSTRINGINIT         LD STRING_PTR 
                    MM TO_BE_PRINTED_TMP 
LOAD_TO_BE_PRINTED  LD TO_BE_PRINTED_TMP
                    + LOAD_CONST    
                    MM LABELLOAD 
LABELLOAD           K /0000 
                    JZ P_STRING_END  ; se zero vamos para o final!
                    PD /100 
                    LD TO_BE_PRINTED_TMP
                    +  TWO
                    MM TO_BE_PRINTED_TMP
                    JP LOAD_TO_BE_PRINTED
P_STRING_END        RS P_STRING 

# PONEASCII_INIT

