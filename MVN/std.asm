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
MOVE_CONST          K /9000           ; instruction to move to memory
ZERO                K /0000 
TWO                 K /0002
STRING_PTR          K /0000           ; ponteiro para o comeco da string 

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

