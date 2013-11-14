; Biblioteca de ambiente
; ======================

; PRINT ASCII
; PONEASCII <adrr>
PONEASCII               > 
TO_BE_PRINTED           >
P_STRING                >
STRING_PTR              >
P_INT_ZERO              >
P_LINE                  >
PUSH_CALL_SIZELV        > 
PUSH_CALL_RET_ADDRS     > 
PUSH_CALL_TMP_SZ        > 
PUSH_CALL_PAR_SZ        > 
PUSH_CALL               > 
POP_CALL                > 
SAVE_WORD_TO_LOCAL_VAR  >
WORD_TO_SAVE            >
ORIGIN_PTR              >
GET_WORD_LOCAL_VAR      >
WORD_TO_GET             > 
STORE_PTR               >
PRINT_STACK_ADDRS       >
POP_CALL_FCT            >
EXAMPLE_STACK           >
EXAMPLE_STACK_ARG       >
READ_INT                >
; relocavel
& /0000
START_STD_LIB_PADDING K /0000 
; alocando para K
TO_BE_PRINTED       K /0000 
TO_BE_PRINTED_TMP   K /0000 
LOAD_CONST          K /8000   ; instruction to load 
LOADV_CONST         K /3000   ; instruction to load 
MOVE_CONST          K /9000   ; instruction to move to memory
ZERO                K /0000 
TWO                 K /0002
ONE                 K /0001
TEN                 K =10
STRING_PTR          K /0000   ; ponteiro para o comeco da string 
TMP_1               K /0000  
TMP_2               K /0000  
TMP_3               K /0000  
TMP_4               K /0000  
;; VARIAVEIS GLOBAIS
;; comeco da pilha = FFF 
;; tamanho da pilha = 2FF 
;;   | ptr to old_stack_head  | \___ STACK_PTR
;;   |      savedregist       |   
;;   |         ...            |   
;;   |      local var         |  
;;   |         ...            |   
;;   |      temporaries       |  
;;   |      parameters        |   
;;   |         ...            |  
;;   |     ref parameters     |  ____ OLD STACK_PTR
;;   |      returnaddrs       | /     (STACK_PTR points here)
STACK_PTR           K /0FFF
;;STACK_FRAME         K /0FFF 

;; FIM VARIAVEIS GLOBAIS 

;; **** P_LINE ****
;; imprime uma linha
;;

COUNT_IS      K /436f
              K /756e
              K /7465
              K /7220
              K /6973
              K /3a00
              K /0000 

EXAMPLE_STACK_ARG   K  /0000
EXAMPLE_STACK       JP /000 
                    SC PRINT_STACK_ADDRS   ;; deve imprimir 0fff 
                    ;;; SALVAR ARGUMENTOS na pilha 
                    LV =0
                    MM WORD_TO_SAVE 
                    LV EXAMPLE_STACK_ARG 
                    MM ORIGIN_PTR 
                    SC SAVE_WORD_TO_LOCAL_VAR 
                    ;;;; CORPO DA FUNCAO 
                    ;;; CARREGANDO UM VALOR DA PILHA 
                    LV =0
                    MM WORD_TO_GET 
                    LV EXAMPLE_STACK_ARG 
                    MM STORE_PTR
                    SC GET_WORD_LOCAL_VAR 
                    ;;; IMPRIME 
                    LV COUNT_IS
                    MM STRING_PTR 
                    SC P_STRING  ;; inline fct, no need to stack 
                    LD EXAMPLE_STACK_ARG 
                    MM TO_BE_PRINTED 
                    SC P_INT_ZERO
                    SC P_LINE

                    LD EXAMPLE_STACK_ARG
                    JZ RETURN_EXAMPLE_STACK

                    LD EXAMPLE_STACK_ARG 
                    -  ONE 
                    MM EXAMPLE_STACK_ARG 

                    LV =4
                    MM PUSH_CALL_SIZELV
                    LV =0
                    MM PUSH_CALL_RET_ADDRS 
                    LV =0
                    MM PUSH_CALL_TMP_SZ
                    LV =0
                    MM PUSH_CALL_PAR_SZ 
                    SC PUSH_CALL

                    SC EXAMPLE_STACK  ;; chamada recursiva
                    ;;;; FIM DO CORPO DA FUNCAO 
RETURN_EXAMPLE_STACK LV EXAMPLE_STACK
                    MM POP_CALL_FCT 
                    SC POP_CALL ;; trickery!

                    SC PRINT_STACK_ADDRS   ;; deve imprimir 0fff 
                    RS EXAMPLE_STACK


; MISCELANEA: 
;;*** P_LINE***
;; imprime uma linha nova 
P_LINE              JP /000           ; alocando para o endereço de retorno  
                    LV /00A  ; carregar a variavel 
                    PD /100           ; imprimir na tela 
                    RS P_LINE         ; retornar a rotina  

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

;; *** READ_INT STORE_PTR ***
;; doesnt care about buffers, should have a trailing char at the end of the
;; stream otherwise it will just discard it.. 




ZERO_M_ONE          K /0029
NINE_P_ONE          K /0039
FOUR                K =64
LOW                 K /0000
HIGH                K /0000
GO_IF_NUMBER        K /0000 
TO_BE_TRIMMED       K /0000 
TBT_TMP             K /0000 

TRIM_INT            JP /000
                    LD TO_BE_TRIMMED
                    /  FOUR 
                    *  FOUR
                    MM TBT_TMP 
                    LD TO_BE_TRIMMED
                    -  TBT_TMP 
                    MM TO_BE_TRIMMED 
                    RS TRIM_INT 

READ_INT_WORD       JP /000
                    GD /000 
                    MM TMP_3 
                    LD TMP_3
                    /  FOUR 
                    MM TO_BE_TRIMMED 
                    SC TRIM_INT 
                    LD TO_BE_TRIMMED
                    MM LOW 

                    LD TMP_3
                    MM TO_BE_TRIMMED 
                    SC TRIM_INT
                    LD TO_BE_TRIMMED
                    MM HIGH
                    
                    LD TMP_3
                    MM TO_BE_PRINTED
                    SC P_INT_ZERO

                    LD HIGH
                    MM TO_BE_PRINTED
                    SC P_INT_ZERO
                    LD LOW 
                    MM TO_BE_PRINTED
                    SC P_INT_ZERO


                    RS READ_INT_WORD

READ_INT            JP /000 
                    LV =0 
                    MM TMP_4
READ_INT_LOOP       SC READ_INT_WORD 
                    LD HIGH 
                    MM TMP_3 
                    LV CONT1
                    MM GO_IF_NUMBER
                    JP IF_NUMBER_CONTINUE 
CONT1               LD LOW
                    MM TMP_3 
                    LV READ_INT_LOOP
                    MM GO_IF_NUMBER
                    JP IF_NUMBER_CONTINUE 
NOT_NUMBER          LD STORE_PTR 
                    +  MOVE_CONST 
                    MM MOVE_READ_INT
                    LD TMP_4 
MOVE_READ_INT       JP /000 
                    RS READ_INT 

IF_NUMBER_CONTINUE  LD TMP_3 
                    -  ZERO_M_ONE 
                    JN NOT_NUMBER 
                    LD NINE_P_ONE 
                    -  TMP_3
                    JN NOT_NUMBER  

                    LD TMP_4
                    *  TEN 
                    MM TMP_4 

                    LD TMP_3 
                    -  ZERO_M_ONE
                    +  ONE 
                    +  TMP_4
                    MM TMP_4
                    LD GO_IF_NUMBER
                    MM END_READ_INT
END_READ_INT        JP /000
                     
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

PUSH_CALL_SIZELV        K /000 
PUSH_CALL_RET_ADDRS     K /000 
PUSH_CALL_TMP_SZ        K /000  
PUSH_CALL_PAR_SZ        K /000 

;;  PUSH_CALL 
;;  CALLEE_FCT
;;    POP_CALL 

PUSH_CALL           JP /000 
                    LD PUSH_CALL  ;; get return addrs 
                    +  TWO ;; return address of the callee
                    +  LOADV_CONST
                    MM LOAD_RETURN_ADDRS 
                    LD STACK_PTR
                    -  TWO        ;; new return addrs  
                    +  MOVE_CONST 
                    MM MOVE_RETURN_ADDRS 
LOAD_RETURN_ADDRS   JP /000  
MOVE_RETURN_ADDRS   JP /000   ;; return addrs salvo 
                    LD STACK_PTR          
                    -  TWO 
                    -  TWO 
                    -  PUSH_CALL_SIZELV
                    -  PUSH_CALL_RET_ADDRS
                    -  PUSH_CALL_TMP_SZ
                    -  PUSH_CALL_PAR_SZ 
                    -  TWO  ;; return addrs
                    MM TMP_1
                    LD TMP_1 
                    +  MOVE_CONST 
                    MM MRKR_PC_SAVE_HEAD 
                    LD STACK_PTR 
MRKR_PC_SAVE_HEAD   JP /000 
                    LD TMP_1 
                    MM STACK_PTR
                    RS PUSH_CALL
;; **** POP_CALL ****

POP_CALL_FCT        K /0000             
POP_CALL            JP /000 ; retorno 
POP_CALL_INIT       LD STACK_PTR 
                    +  LOAD_CONST 
                    MM MRKR_PC_LOAD_HEAD 
MRKR_PC_LOAD_HEAD   JP /000 
                    MM STACK_PTR 
                    LD STACK_PTR 
                    -  TWO
                    +  LOAD_CONST 
                    MM LOAD_RETURN_ADDRS_2
                    LD POP_CALL_FCT 
                    +  MOVE_CONST 
                    MM MOVE_RETURN_ADDRS_2
LOAD_RETURN_ADDRS_2 JP /000 
MOVE_RETURN_ADDRS_2 JP /000  ;; engana a funcao para ela pensar que ela 
                             ;; tem que retornar para esse valor 
                    RS POP_CALL

;; **** SAVE_WORD_TO_LOCAL_VAR WORD_TO_SAVE ORIGIN_PTR ****


WORD_TO_SAVE            K /000
ORIGIN_PTR              K /000 

SAVE_WORD_TO_LOCAL_VAR      JP /000 
                            LD STACK_PTR
                            + TWO          ;; first word 
                            + WORD_TO_SAVE 
                            + WORD_TO_SAVE  ;; WORD_TO_GET * 2
                            + MOVE_CONST   ;; 
                            MM MOVE_WORD_LOCAL_VAR_2
                            LD STORE_PTR
                            + LOAD_CONST 
                            MM LOAD_WORD_LOCAL_VAR_2
LOAD_WORD_LOCAL_VAR_2       JP /000 ;; 8FROMPTR
MOVE_WORD_LOCAL_VAR_2       JP /000 ;; 9TOPTR
                            RS SAVE_WORD_TO_LOCAL_VAR

;; **** GET_WORD_LOCAL_VAR WORD_TO_GET STORE_PTR ****


WORD_TO_GET        K /000 
STORE_PTR          K /000

GET_WORD_LOCAL_VAR          JP /000 
                            LD STACK_PTR
                            + TWO          ;; first word 
                            + WORD_TO_GET  
                            + WORD_TO_GET  ;; WORD_TO_GET * 2
                            + LOAD_CONST   ;; 
                            MM LOAD_WORD_LOCAL_VAR  
                            LD STORE_PTR
                            + MOVE_CONST 
                            MM MOVE_WORD_LOCAL_VAR
LOAD_WORD_LOCAL_VAR         JP /000 ;; 8FROMPTR
MOVE_WORD_LOCAL_VAR         JP /000 ;; 9TOPTR
                            RS GET_WORD_LOCAL_VAR
                            
STACK_WAS_STR K  /5374 
              K  /6163 
              K  /6b20
              K  /0077 
              K  /6173 
              K  /3a20
              K  /0000

PRINT_STACK_ADDRS JP /000 
                  LV STACK_WAS_STR   ; carrega o ponteiro da string 
                  MM STRING_PTR ; copia o ponteiro para o parametro
                  SC P_STRING   ; roda p_string 
                  LD STACK_PTR 
                  MM TO_BE_PRINTED
                  SC P_INT_ZERO 
                  SC P_LINE 
                  RS PRINT_STACK_ADDRS

# START_STD_LIB_PADDING 
