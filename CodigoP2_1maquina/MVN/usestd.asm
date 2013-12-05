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
EXAMPLE_STACK        <
EXAMPLE_STACK_ARG    <
READ_INT             <
STORE_PTR            <
GETS                 <
STORE_PTR_IO         <
@ /0000


             LV PLEASE_INSERT_A_STR 
             MM STRING_PTR
             SC P_STRING 

             LV BUFFER_STR
             MM STORE_PTR_IO 
             SC GETS 

             LV BUFFER_STR 
             MM STRING_PTR
             SC P_STRING

         JP INICIO 

BUFFER_STR        K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 
                  K  /0000 

CHAR              K  /0021    ;; string a ser impressa 
                  K  /0022
                  K  /0023
                  K  /5465
                  K  /7374
                  K  /6500
                  K  /0024
                  K  /0000
CHAR_SPC          K  /0020 
                  K  /0000 

STACK_SHOULD_BE   K /5374
                  K /6163
                  K /6b20
                  K /7368
                  K /6f75
                  K /6c64
                  K /2062
                  K /653a
                  K /0000 

INSERT_A_NUMBER   K  /506c
                  K  /6561
                  K  /7365
                  K  /2077
                  K  /7269
                  K  /7465
                  K  /2061
                  K  /206e
                  K  /756d
                  K  /6265
                  K  /722c
                  K  /2077
                  K  /6520
                  K  /7769
                  K  /6c6c
                  K  /2063
                  K  /6f75
                  K  /6e74
                  K  /2072
                  K  /6563
                  K  /7572
                  K  /7369
                  K  /7665
                  K  /6c79
                  K  /2075
                  K  /6e74
                  K  /696c
                  K  /2069
                  K  /7420
                  K  /6765
                  K  /7473
                  K  /2074
                  K  /6f20
                  K  /7a65
                  K  /726f
                  K  /3a20
                  K  /0000 

PLEASE_INSERT_A_STR K /706C 
                    K /6561 
                    K /7365 
                    K /2069 
                    K /6E73 
                    K /6572 
                    K /7420 
                    K /6120 
                    K /7374 
                    K /7269 
                    K /6E67 
                    K /3A0A
                    K /0000

INT_1             K  /FFFF
INT_4             K  =12
INT_ZERO          K  =0
INT_FFFF          K  /FFFF 
INT_ONE           K  =1
INICIO  LV CHAR       ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        SC P_LINE

        ;; imprime de 0 a 12 

              SC P_LINE 
              LD INT_ZERO 
              MM INT_1 

LOOP_INTEGERS LD INT_1  
              MM TO_BE_PRINTED
              SC P_INT_ZERO

              ;; IMPRIME LINHA 
              SC P_LINE 
              
              LD INT_1 
              +  INT_ONE 
              MM INT_1 
              LD INT_1
              -  INT_4 
              JZ BREAK_LOOP_INTEGERS
              JP LOOP_INTEGERS

              
BREAK_LOOP_INTEGERS SC P_LINE

;;; TEST STACK!!!!  
;;; (stack starts at fff)

              LV STACK_SHOULD_BE 
              MM STRING_PTR 
              SC P_STRING 
              LV /FFF 
              MM TO_BE_PRINTED
              SC P_INT_ZERO 
              SC P_LINE 

              LV INSERT_A_NUMBER  ;; ask for a number
              MM STRING_PTR
              SC P_STRING
              SC P_LINE

              LV =1
              MM PUSH_CALL_SIZELV
              LV =0
              MM PUSH_CALL_RET_ADDRS 
              LV =0
              MM PUSH_CALL_TMP_SZ
              LV =0
              MM PUSH_CALL_PAR_SZ 
              ;; read integer from input
              LV INT_1 
              MM STORE_PTR_IO
              SC READ_INT 

              LD INT_1 
              MM EXAMPLE_STACK_ARG
              SC PUSH_CALL           ;; primeiro dah push  
              SC EXAMPLE_STACK       ;; depois chama.. 

              LV STACK_SHOULD_BE 
              MM STRING_PTR 
              SC P_STRING 
              LV /FFF 
              MM TO_BE_PRINTED
              SC P_INT_ZERO 
              SC P_LINE 

FIM     HM /00 
# INICIO  
