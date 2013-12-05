PONEASCII               > 
TO_BE_PRINTED           >
P_STRING                >
STRING_PTR              >
P_INT_ZERO              >
P_LINE                  >
STORE_PTR_IO            >
READ_INT                >
GETS                    > 



& /0000
START_STD_LIB_PADDING K /0000

SHIFT_BYTE          K /0100
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
;;*** P_LINE***
;; imprime uma linha nova 
                    K  /6665
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
                    +  LOAD_CONST    
                    MM LABELLOAD 
LABELLOAD           K  /0000 
                    JZ P_STRING_END  ; se zero vamos para o final!
                    PD /100 
                    LD TO_BE_PRINTED_TMP
                    +  TWO
                    MM TO_BE_PRINTED_TMP
                    JP LOAD_TO_BE_PRINTED
P_STRING_END        RS P_STRING 

;; *** READ_INT STORE_PTR_IO ***
;; doesnt care about buffers, should have a trailing char at the end of the
;; stream otherwise it will just discard it.. 
STORE_PTR_IO        JP /000
ZERO_M_ONE          K  /002F
NINE_P_ONE          K  /0039

LOW                 K  /0000
HIGH                K  /0000
GO_IF_NUMBER        K  /0000 
TO_BE_TRIMMED       K  /0000 
TBT_TMP             K  /0000 

TRIM_INT            JP /000
                    LD TO_BE_TRIMMED
                    /  SHIFT_BYTE  
                    *  SHIFT_BYTE
                    MM TBT_TMP 
                    LD TO_BE_TRIMMED
                    -  TBT_TMP 
                    MM TO_BE_TRIMMED 
                    RS TRIM_INT 

READ_INT_WORD       JP /000
                    GD /000 
                    MM TMP_3 
                    LD TMP_3
                    /  SHIFT_BYTE
                    MM TO_BE_TRIMMED 
                    SC TRIM_INT 
                    LD TO_BE_TRIMMED
                    MM HIGH 

                    LD TMP_3
                    MM TO_BE_TRIMMED 
                    SC TRIM_INT
                    LD TO_BE_TRIMMED
                    MM LOW
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
NOT_NUMBER          LD STORE_PTR_IO
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
                    -  ONE 
                    +  TMP_4
                    MM TMP_4

                    LD GO_IF_NUMBER
                    MM END_READ_INT
END_READ_INT        JP /000
       
;; *** GETS STORE_PTR_IO ***
;; Existe um problema de buffer aqui... nao vamos 
;; trata-lo 
LAST_CONTROL_CHAR_P_ONE    K /0021
ARRAY_POS_BYTE  JP /000
GETS            JP /000
                LD STORE_PTR_IO
                MM ARRAY_POS_BYTE
GETS_LOOP       GD /000
                MM HIGH_V
                SC HIGH_LOW 
                LD HIGH_V 
                -  LAST_CONTROL_CHAR_P_ONE 
                JN RETURN_GETS 
                LD ARRAY_POS_BYTE 
                +  MOVE_CONST 
                MM MOVE_HIGH_V
                LD HIGH_V 
MOVE_HIGH_V     JP /000 

                LD ARRAY_POS_BYTE
                +  TWO 
                MM ARRAY_POS_BYTE 

                LD LOW_V 
                -  LAST_CONTROL_CHAR_P_ONE 
                JN RETURN_GETS 
                LD ARRAY_POS_BYTE 
                +  MOVE_CONST 
                MM MOVE_LOW_V
                LD LOW_V 
MOVE_LOW_V      JP /000 

                LD ARRAY_POS_BYTE
                +  TWO 
                MM ARRAY_POS_BYTE 

                JP GETS_LOOP 

RETURN_GETS     LD ARRAY_POS_BYTE 
                +  MOVE_CONST 
                MM MOVE_ZERO
                LV =000  
MOVE_ZERO       JP /000 

                LD ARRAY_POS_BYTE
                +  TWO 
                MM ARRAY_POS_BYTE 
                RS GETS


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

# START_STD_LIB_PADDING 
