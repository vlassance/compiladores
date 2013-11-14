; Biblioteca de ambiente
; ======================


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
READ_INT                <
PONEASCII               < 
TO_BE_PRINTED           <
P_STRING                <
STRING_PTR              <
P_INT_ZERO              <
P_LINE                  <
STORE_PTR_IO            <
; relocavel
& /0000
START_STD_LIB_PADDING K /0000 
; alocando para K
SHIFT_BYTE          K  /0100
LOAD_CONST          K /8000   ; instruction to load 
LOADV_CONST         K /3000   ; instruction to load 
MOVE_CONST          K /9000   ; instruction to move to memory
ZERO                K /0000 
TWO                 K /0002
ONE                 K /0001
TEN                 K =10
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

                    LV =1
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
;; *** HIGH_LOW HIGH_V *** 
HIGH_V              JP /000 
LOW_V               JP /000 
HIGH_LOW            JP /000 
                    LD HIGH_V 
                    MM LOW_V 
                    LD LOW_V 
                    *  SHIFT_BYTE 
                    /  SHIFT_BYTE 
                    MM LOW_V 
                    LD HIGH_V
                    /  SHIFT_BYTE
                    MM HIGH_V

                    LD LOW_V
                    /  SHIFT_BYTE
                    *  SHIFT_BYTE 
                    -  LOW_V
                    MM LOW_V
                    LD ZERO 
                    -  LOW_V
                    MM LOW_V 

                    LD HIGH_V
                    /  SHIFT_BYTE
                    *  SHIFT_BYTE 
                    -  HIGH_V
                    MM HIGH_V
                    LD ZERO 
                    -  HIGH_V
                    MM HIGH_V 

                    RS HIGH_LOW 

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
                            LD ORIGIN_PTR
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
