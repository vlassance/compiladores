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
@ /0000

         JP INICIO 
CHAR     K  /0021    ;; string a ser impressa 
         K  /0022
         K  /0023
         K  /5465
         K  /7374
         K  /6500
         K  /0024
         K  /0000
CHAR_SPC K  /0020 
         K  /0000 

STACK_SHOULD_BE K /5374
                K /6163
                K /6b20
                K /7368
                K /6f75
                K /6c64
                K /2062
                K /653a
                K /0000 

INT_1    K  /FFFF
INT_2    K  =5000
INT_3    K  =345
INT_4    K  =12
INT_5    K  =1
INT_ZERO K  =0
INT_FFFF K  /FFFF 
INT_ONE  K  =1
INICIO  LV CHAR       ; carrega o ponteiro da string 
        MM STRING_PTR ; copia o ponteiro para o parametro
        SC P_STRING   ; roda p_string 

        SC P_LINE

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

              SC PRINT_STACK_ADDRS   ;; deve imprimir 0fff 

              LV =4
              MM PUSH_CALL_SIZELV
              LV =0
              MM PUSH_CALL_RET_ADDRS 
              LV =0
              MM PUSH_CALL_TMP_SZ
              LV =0
              MM PUSH_CALL_PAR_SZ 
              ;; read integer from input
              LV INT_1 
              MM STORE_PTR
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
