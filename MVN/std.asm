; Biblioteca de ambiente
; ======================

; PRINT ASCII
; PONEASCII <adrr>
PONEASCII > 
TO_BE_PRINTED >
P_STRING >
STRING_PTR >
P_INT_ZERO >
; relocavel
& /0000
; alocando para K
TO_BE_PRINTED       K /0000 
TO_BE_PRINTED_TMP   K /0000 
LOAD_CONST          K /8000           ; instruction to load 
MOVE_CONST          K /9000           ; instruction to move to memory
ZERO                K /0000 
TWO                 K /0002
STRING_PTR          K /0000           ; ponteiro para o comeco da string 
TMP_1               K /0000  
TMP_2               K /0000  
;; VARIAVEIS GLOBAIS
;; tamanho da pilha = 2FF 
;;   | ptr to old_stack_head  | \___ STACK_PTR
;;   |      local var         |  
;;   |         ...            |   
;;   |      temporaries       |   
;;   |      savedregist       |   
;;   |         ...            |   
;;   |      returnaddrs       | \___ STACK_FRAME   
;;   |      parameters        |   
;;   |         ...            |   

STACK_PTR           K /0FFF
STACK_FRAME         K /0FFF 

;; FIM VARIAVEIS GLOBAIS 

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

;; *** P_INT_ZERO TO_BE_PRINTED ***
;;  Imprime um inteiro (com zeros à esquerda)
;; ex:  
;;  INT_2 K  =345 
;;        LD INT_2
;;        MM TO_BE_PRINTED 
;;        SC P_INT_ZERO
;; imprime 00345
;;
;;
;; Esta funcao esta com o loop inline
;; sendo simples e robusta

P_INT_ZERO          JP /000
P_INT_INIT          JP P_INT_REAL_INIT
ZERO_BASE           K /30
;; bases para a conversao:
INT_POT_1           K =10000   
INT_POT_2           K =1000 
INT_POT_3           K =100  
INT_POT_4           K =10 
INT_POT_5           K =1 
P_INT_REAL_INIT     LD TO_BE_PRINTED       ;; PRIMEIRO CHAR
                    MM TMP_1
                    /  INT_POT_1 
                    +  ZERO_BASE 
                    PD /100                     ;; imprime 
                    LD TMP_1  
                    /  INT_POT_1
                    *  INT_POT_1 
                    MM TMP_2                     
                    LD TMP_1
                    -  TMP_2
                    MM TMP_1   
                    /  INT_POT_2                ;; segundo char 
                    +  ZERO_BASE 
                    PD /100                     ;; imprime 
                    LD TMP_1  
                    /  INT_POT_2
                    *  INT_POT_2 
                    MM TMP_2                     
                    LD TMP_1
                    -  TMP_2
                    MM TMP_1 
                    /  INT_POT_3                ;; terceiro char 
                    +  ZERO_BASE 
                    PD /100                     ;; imprime 
                    LD TMP_1  
                    /  INT_POT_3
                    *  INT_POT_3 
                    MM TMP_2                     
                    LD TMP_1
                    -  TMP_2
                    MM TMP_1 
                    /  INT_POT_4                ;; quarto char 
                    +  ZERO_BASE 
                    PD /100                     ;; imprime 
                    LD TMP_1  
                    /  INT_POT_4
                    *  INT_POT_4 
                    MM TMP_2                     
                    LD TMP_1
                    -  TMP_2
                    MM TMP_1 
                    /  INT_POT_5                ;; quinto char 
                    +  ZERO_BASE 
                    PD /100                     ;; imprime 
                    LD TMP_1  
                    /  INT_POT_5
                    *  INT_POT_5 
                    MM TMP_2                     
                    LD TMP_1
                    -  TMP_2
                    MM TMP_1 
                    RS P_INT_ZERO

;; ****  PUSH_CALL PUSH_CALL_SIZELV 
;;       PUSH_CALL_RET_ADDRS PUSH_CALL_TMP_SZ   
;;       PUSH_CALL_PAR_SZ ****
;;    Empilha um espaco de memoria 
;; para a pilha. 
;;;;;;; \/ this is not working yet... TODO(gpgouveia) 
;;;PUSH_CALL_SIZELV        K /000 
;;;PUSH_CALL_RET_ADDRS     K /000 
;;;PUSH_CALL_TMP_SZ        K /000  
;;;PUSH_CALL_PAR_SZ        K /000 
;;;
;;;;; definicao:
;;;PUSH_CALL           JP /000 ; retorno 
;;;PUSH_CALL_INIT      LD STACK_PTR          ;; get STACK_PTR
;;;                    +  MOVE_CONST         ;; get Instruction to move to stack_ptr
;;;                    MM MRKR_PC_SAVE_HEAD  ;; put composed instruction on marker
;;;                    LD STACK_PTR          ;; load stack_ptr 
;;;MRKR_PC_SAVE_HEAD   K /0000               ;; move old STACK_PTR to the address 
;;;                                          ;; it points to 
;;;                    LD STACK_PTR 
;;;                    -  PUSH_CALL_SIZELV
;;;                    -  PUSH_CALL_RET_ADDRS
;;;                    -  PUSH_CALL_TMP_SZ
;;;                    -  PUSH_CALL_PAR_SZ 
;;;                    -  TWO  ;; return addrs
;;;                    MM STACK_PTR          ;; saves new STACK_PTR 
;;;
;;;                    
;;;

# PONEASCII_INIT

