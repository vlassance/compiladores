; Biblioteca de ambiente
; ======================
; exports
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
;; ARITH: 
PUSH_ARITH              >            
POP_ARITH               >  
SUM_ARITH               > 
SUB_ARITH               >  
DIV_ARITH               >   
MUL_ARITH               >   
AND_ARITH               >   
OR_ARITH                >   
NOT_ARITH               >   
;; imports
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

COUNT_IS_STR      K /436f
                  K /756e
                  K /7465
                  K /7220
                  K /6973
                  K /3a00
                  K /0000 

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
FUNCTION_BEING_CALLED           K /000 
SIZE_OF_CONTEXT                 K /000 
PUSH_CALL_2                     JP /000 
                                MM FUNCTION_BEING_CALLED 
                                -  TWO 
                                +  LOAD_CONST 
                                MM LOAD_SIZE_OF_CONTEXT 
LOAD_SIZE_OF_CONTEXT            JP /000 
                                MM SIZE_OF_CONTEXT 
ITER_AND_COPY_CONTEXT           LD SIZE_OF_CONTEXT
                                JZ NO_MORE_CONTEXT_COPIES 
                                
                                LD FUNCTION_BEING_CALLED 
                                -  TWO 
                                -  SIZE_OF_CONTEXT
                                -  SIZE_OF_CONTEXT 
                                ;;;; TODO HERE 
                                                ;;;; acho que agnt tem que
                                                ;;;; fazer isso depois 


                                LD SIZE_OF_CONTEXT
                                -  ONE 
                                MM SIZE_OF_CONTEXT 
NO_MORE_CONTEXT_COPIES          LD FUNCTION_BEING_CALLED  ;; Will copy the 
                                                          ;; return address 
                                +  LOAD_CONST     ;; TODO VERIFY THIS THING RIGHT HERE 
                                MM LOAD_PUSH_CALL ;; WE MAY NEED TO SUM UP 2 IN ORDER TO JUMP
                                                  ;; OVER SOME INSTR 
                                LD STACK_PTR 
                                -  TWO 
                                +  MOVE_CONST 
                                MM MOVE_PUSH_CALL 
LOAD_PUSH_CALL                  JP /000 
MOVE_PUSH_CALL                  JP /000             
                                LD STACK_PTR 
                                -  TWO 
                                -  TWO 
                                -  SIZE_OF_CONTEXT
                                -  SIZE_OF_CONTEXT 
                                MM TMP_1 
                                LD TMP_1
                                +  MOVE_CONST
                                MM MRKR_PC_SAVE_HEAD
                                LD STACK_PTR 
MRKR_PC_SAVE_HEAD               JP /000 
                                LD TMP_1 
                                MM STACK_PTR
                                RS PUSH_CALL
                 



;; **** POP_CALL ****
                    K /6666
POP_CALL_FCT        K /0000             
POP_CALL            JP /000 ; retorno
;;                    SC PRINT_STACK_ADDRS ;; TODO remove
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


;; pilha aritimética:
ARIT_STACK_ZERO K /0000   ;; 0
                K /0000   ;; 1
                K /0000   ;; 2
                K /0000   ;; 3
                K /0000   ;; 4
                K /0000   ;; 5
                K /0000   ;; 6
                K /0000   ;; 7
                K /0000   ;; 8
                K /0000   ;; 9
                K /0000   ;; 10
                K /0000   ;; 11
                K /0000   ;; 12
                K /0000   ;; 13
                K /0000   ;; 14
                K /0000   ;; 15
                K /0000   ;; 16
                K /0000   ;; 17
                K /0000   ;; 18
                K /0000   ;; 19
                K /0000   ;; 20

ARIT_PTR_STACK  K ARIT_STACK_ZERO

PUSH_ARITH      JP /000
                MM TMP_1
                LD ARIT_PTR_STACK 
                +  TWO
                MM ARIT_PTR_STACK
                +  MOVE_CONST
                MM OP_PUSH_ARITH
                LD TMP_1
OP_PUSH_ARITH   JP /000 
                RS PUSH_ARITH 


POP_ARITH       JP /000 
                LD ARIT_PTR_STACK 
                -  TWO
                MM ARIT_PTR_STACK 
                +  TWO 
                +  LOAD_CONST 
                MM OP_POP_ARITH
OP_POP_ARITH    JP /000
                RS POP_ARITH 

FLIP_ARITH      JP /000 
                MM TMP_4 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH
                MM TMP_3
                LD TMP_2 
                SC PUSH_ARITH 
                LD TMP_3 
                SC PUSH_ARITH 
                LD TMP_4 
                RS FLIP_ARITH

;;; BIN OPER:

SUM_ARITH       JP /000 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH
                +  TMP_2 
                SC PUSH_ARITH
                RS SUM_ARITH 

SUB_ARITH       JP /000 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH 
                -  TMP_2 
                SC PUSH_ARITH
                RS SUB_ARITH 

DIV_ARITH       JP /000 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH 
                /  TMP_2 
                SC PUSH_ARITH
                RS DIV_ARITH 

MUL_ARITH       JP /000 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH 
                *  TMP_2 
                SC PUSH_ARITH
                RS MUL_ARITH 

AND_ARITH       JP /000 
                SC POP_ARITH 
                MM TMP_2
                SC POP_ARITH 
                JZ PUSH_ZERO_AND_ARITH
                LD TMP_2 
                JZ PUSH_ZERO_AND_ARITH 
                LV /001
                SC PUSH_ARITH 
                RS AND_ARITH
PUSH_ZERO_AND_ARITH  LV /000 
                     SC PUSH_ARITH
                     RS AND_ARITH 

OR_ARITH        JP /000 
                SC POP_ARITH
                MM TMP_2 
                SC POP_ARITH 
                JZ ONE_ZERO_OR_ARITH
                LV /001
                SC PUSH_ARITH 
                RS OR_ARITH 
ONE_ZERO_OR_ARITH LD TMP_2 
                  JZ PUSH_ZERO_OR_ARITH 
                  LV /001 
                  SC PUSH_ARITH 
                  RS OR_ARITH 
PUSH_ZERO_OR_ARITH LV /000 
                   SC PUSH_ARITH 
                   RS OR_ARITH 

EQ_OPER_ARITH       JP /000 
                    SC SUB_ARITH 
                    JZ EQ_OPER_ARITH_EQUAL 
                    LD ZERO
                    SC PUSH_ARITH
                    RS EQ_OPER_ARITH
EQ_OPER_ARITH_EQUAL LD ONE 
                    SC PUSH_ARITH 
                    RS EQ_OPER_ARITH

NEQ_OPER_ARITH       JP /000 
                     SC EQ_OPER_ARITH 
                     SC NOT_ARITH 
                     RS NEQ_OPER_ARITH 

LT_OPER_ARITH         JP /000            ;; a1 < a2
                      SC SUB_ARITH 
                      JN LT_OPER_ARITH_RET_ONE
                      LD ZERO
                      SC PUSH_ARITH 
                      RS LT_OPER_ARITH
LT_OPER_ARITH_RET_ONE LD ONE 
                      SC PUSH_ARITH 
                      RS LT_OPER_ARITH

GEQ_OPER_ARITH        JP /000     ;; a1 >= a2 
                      SC LT_OPER_ARITH 
                      SC NOT_ARITH 
                      RS GEQ_OPER_ARITH 

GT_OPER_ARITH         JP /000    ;; a1 > a2 
                      SC FLIP_ARITH 
                      SC LT_OPER_ARITH 
                      RS GT_OPER_ARITH 


LEQ_OPER_ARITH        JP /000     ;; a1 <= a2 
                      SC GT_OPER_ARITH 
                      SC NOT_ARITH 
                      RS LEQ_OPER_ARITH 
                      
;; UNARY OPER:

NOT_ARITH           JP /000 
                    SC POP_ARITH 
                    JZ PUSH_ONE_NOT_ARITH
                    LV /000 
                    SC PUSH_ARITH 
                    RS NOT_ARITH 
PUSH_ONE_NOT_ARITH  LV /001 
                    SC PUSH_ARITH
                    RS NOT_ARITH


# START_STD_LIB_PADDING 
