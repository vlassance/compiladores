P_STRING                <
STRING_PTR              < 
P_INT_ZERO              < 
TO_BE_PRINTED           <
P_LINE                  <
PUSH_CALL               <
PRINT_STACK_ADDRS       < 
POP_CALL                <
READ_INT                <
STORE_PTR               <
GETS                    <
STORE_PTR_IO            <
WORD_TO_BE_SAVED        <
SAVE_WORD_ON_STACK      <
LOAD_WORD_FROM_STACK    <
ORIGIN_PTR              <
POP_CALL_FCT            <
PUSH_ARITH              <        
POP_ARITH               <
SUM_ARITH               <
SUB_ARITH               <
DIV_ARITH               <
MUL_ARITH               <
AND_ARITH               <
OR_ARITH                <
NOT_ARITH               <
GEQ_OPER_ARITH          <
LEQ_OPER_ARITH          <
DBG                     <
@ /0000
CZAR_INICIO_CODE        JP CZAR_INICIO 
CZAR_STUB               K =1   
;;============================================================

CONST_VAR_0             K =6  ; const int fat_10_rec = 6;
GLOBAL_VAR_0            K =0  ; decl int retorno;
_CONST_NUM_1            K =0001

FUNCTION_0_RETURN       K =0  ; int  
FUNCTION_0_ARG_0        K =0  ; int n
FUNCTION_0_TMP_0        K =0  ; function return 
FUNCTION_0_LOCAL_VAR_0  K =0  ; decl int retorno = 1;
                        K =4  ; int fatorial_recursivo(int n) {
FUNCTION_0              JP /000 ; 
                        LV FUNCTION_0 
                        SC PUSH_CALL 

                        LV =1                   ; retorno = 1;
                        MM WORD_TO_BE_SAVED 
                        LV =3                   ; FUNCTION_0_LOCAL_VAR_0
                        SC SAVE_WORD_ON_STACK 
                        ;; (n >= 1)
                        LV =2                   ; FUNCTION_0_ARG_0  
                        SC LOAD_WORD_FROM_STACK  
                        SC PUSH_ARITH 
                        LD _CONST_NUM_1 
                        SC PUSH_ARITH
                        SC GEQ_OPER_ARITH 
                        SC POP_ARITH  
                        JZ FUNCTION_0_LABEL_0

                        LV =2 
                        SC LOAD_WORD_FROM_STACK 
                        SC PUSH_ARITH 
                        LD _CONST_NUM_1 
                        SC PUSH_ARITH 
                        SC SUB_ARITH 
                        SC POP_ARITH 

                        MM FUNCTION_0_ARG_0
                        SC FUNCTION_0           ; call fatorial_recursivo (n - 1);
                        LD FUNCTION_0_RETURN    
                        MM WORD_TO_BE_SAVED
                        LV =1
                        SC SAVE_WORD_ON_STACK 
                        
                        LV =2                   ; n * call fatorial_recursivo (n - 1);
                        SC LOAD_WORD_FROM_STACK 
                        SC PUSH_ARITH 
                        LV =1
                        SC LOAD_WORD_FROM_STACK 
                        SC PUSH_ARITH 
                        SC MUL_ARITH 
                        
                        SC POP_ARITH ; set retorno = *
                        MM WORD_TO_BE_SAVED 
                        LV =3 
                        SC SAVE_WORD_ON_STACK
FUNCTION_0_LABEL_0      *  CZAR_STUB
                        LV =3
                        SC LOAD_WORD_FROM_STACK 
                        SC PUSH_ARITH 
                        SC POP_ARITH 
                        MM FUNCTION_0_RETURN 
                        LV FUNCTION_0
                        SC POP_CALL     ;; trickery!
                        RS FUNCTION_0   ;     return retorno;
;; INIT PROG ===============================================
CZAR_INICIO             * CZAR_STUB ;; stub instruction
                        LD CONST_VAR_0
                        MM FUNCTION_0_ARG_0 
                        LV /666
                        SC FUNCTION_0       ;; depois chama.. 
                        LD FUNCTION_0_RETURN 
                        MM GLOBAL_VAR_0
                        LD GLOBAL_VAR_0 
                        MM TO_BE_PRINTED
                        SC P_INT_ZERO
FIM                     HM /00 
# CZAR_INICIO_CODE
