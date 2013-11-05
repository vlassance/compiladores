; Biblioteca de ambiente
; ======================

; PRINT ASCII
; PONEASCII <adrr>
PONEASCII > 
TOBEPRINTED >
; relocavel
& /0000
; alocando para K
TOBEPRINTED K /0000 
; inicio da rotina
PONEASCII   JP /000         ; alocando para o endereço de retorno  
            LD TOBEPRINTED  ; carregar a variavel
            PD /100         ; imprimir na tela
            RS PONEASCII    ; retornar a rotina 

