P_STRING             <
STRING_PTR           < 
P_INT_ZERO           < 
TO_BE_PRINTED        <
P_LINE               <
PUSH_CALL            <
PUSH_CALL_SIZELV     < 
PUSH_CALL_RET_ADDRS  < 
PUSH_CALL_TMP_SZ     <
PUSH_CALL_PAR_SZ     <
PRINT_STACK_ADDRS    < 
POP_CALL             <
READ_INT             <
STORE_PTR            <
GETS                 <
STORE_PTR_IO         <
WORD_TO_SAVE         <
WORD_TO_GET          <
GET_WORD_LOCAL_VAR   <
SAVE_WORD_TO_LOCAL_VAR <
ORIGIN_PTR <
POP_CALL_FCT <
PUSH_ARITH           <        
POP_ARITH            <
SUM_ARITH            <
SUB_ARITH            <
DIV_ARITH            <
MUL_ARITH            <
AND_ARITH            <
OR_ARITH             <
NOT_ARITH            <
@ /0000
CZAR_INICIO_CODE        JP CZAR_INICIO 
CONST_VAR_0             K =6  ; const int fat_10_rec = 6;
GLOBAL_VAR_0            K =0  ; decl int retorno;
ONE                     K /001

FUNCTION_0_RETURN       K =0 
FUNCTION_0_ARG_0        K =0  ; int n
FUNCTION_0_TMP_0        K =0
FUNCTION_0_LOCAL_VAR_0  K =1  ;     decl int retorno = 1;
FUNCTION_0              JP /000 ; int fatorial_recursivo(int n) {
                        ; SC PRINT_STACK_ADDRS
                        ;;;; PREAMBULO
                        LV =0
                        MM WORD_TO_SAVE 
                        LV FUNCTION_0_ARG_0 
                        MM ORIGIN_PTR
                        SC SAVE_WORD_TO_LOCAL_VAR ;; vai pra pilha 
                        ;;;; FIM PREAMBULO 
                        
                        LV =1 
                        MM FUNCTION_0_LOCAL_VAR_0 

                        LV =1
                        MM WORD_TO_SAVE 
                        LV FUNCTION_0_LOCAL_VAR_0
                        MM ORIGIN_PTR
                        SC SAVE_WORD_TO_LOCAL_VAR ;; vai pra pilha 


                        LD FUNCTION_0_ARG_0 ; n >= 1
                        SC PUSH_ARITH 
                        LD ONE
                        SC PUSH_ARITH
                        SC SUB_ARITH 
                        SC POP_ARITH 

                        JN FUNCTION_0_LABEL_0 ; if (n >= 1) {
                        
                        LV =0 
                        MM WORD_TO_GET 
                        LV FUNCTION_0_ARG_0 
                        MM STORE_PTR
                        SC GET_WORD_LOCAL_VAR 

                        LD FUNCTION_0_ARG_0 ; n - 1
                        SC PUSH_ARITH 
                        LD ONE 
                        SC PUSH_ARITH
                        SC SUB_ARITH 
                        SC POP_ARITH 


                        MM FUNCTION_0_ARG_0
                        LV =2
                        MM PUSH_CALL_SIZELV
                        SC PUSH_CALL
                        SC FUNCTION_0 ; call fatorial_recursivo (n - 1);
                        LD FUNCTION_0_RETURN 
                        MM FUNCTION_0_TMP_0
                        

                        LV =0 
                        MM WORD_TO_GET 
                        LV FUNCTION_0_ARG_0 
                        MM STORE_PTR
                        SC GET_WORD_LOCAL_VAR 

                        LD FUNCTION_0_ARG_0 ; n * call fatorial_recursivo (n - 1);
 
                        SC PUSH_ARITH 
                        LD FUNCTION_0_TMP_0 
                        SC PUSH_ARITH
                        SC MUL_ARITH

                        SC POP_ARITH ; set retorno = *
                        MM FUNCTION_0_LOCAL_VAR_0 

                        LV =1
                        MM WORD_TO_SAVE 
                        LV FUNCTION_0_LOCAL_VAR_0
                        MM ORIGIN_PTR
                        SC SAVE_WORD_TO_LOCAL_VAR ;; vai pra pilha 

FUNCTION_0_LABEL_0      *  ONE
                        LV =1 
                        MM WORD_TO_GET 
                        LV FUNCTION_0_LOCAL_VAR_0
                        MM STORE_PTR
                        SC GET_WORD_LOCAL_VAR 

                        LD FUNCTION_0_LOCAL_VAR_0 
                        MM FUNCTION_0_RETURN 

                        LV FUNCTION_0
                        MM POP_CALL_FCT 
                        SC POP_CALL ;; trickery!
                        RS FUNCTION_0   ;     return retorno;
                        K  /7777 

CTR                     K =0
DBG                     * ONE
                        LV /003C
                        PD /100
                        LD CTR
                        MM TO_BE_PRINTED
                        SC P_INT_ZERO
                        LV /003E
                        PD /100
                        LV /000A
                        PD /100
                        LD CTR
                        +  ONE
                        MM CTR
                        RS DBG 


;; INIT PROG ===============================================
CZAR_INICIO             * ONE ;; stub instruction
                       ; SC PRINT_STACK_ADDRS
                        LD CONST_VAR_0
                        MM FUNCTION_0_ARG_0 
                        LV =2
                        MM PUSH_CALL_SIZELV

                        SC PUSH_CALL           ;; primeiro dah push  
                        SC FUNCTION_0       ;; depois chama.. 
                        LD FUNCTION_0_RETURN 
                        MM GLOBAL_VAR_0

                        LD GLOBAL_VAR_0 
                        MM TO_BE_PRINTED
                        SC P_INT_ZERO

FIM                     HM /00 
# CZAR_INICIO_CODE
