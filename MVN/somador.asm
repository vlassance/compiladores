; Somador
; *******
; Somador que recebe duas entradas, nas posições
; ENTRADA1 e ENTRADA2, e coloca o resultado da
; soma na posição SAIDA (externa).


SOMADOR >
ENTRADA1 >
ENTRADA2 >
SAIDA <

& /0000                 ; Origem relocável
; Entradas do programa.
ENTRADA1 K /0000
ENTRADA2 K /0000

; Programa
SOMADOR JP /000         ; Ponto de entrada da subrotina
INICIO LD ENTRADA1
       +  ENTRADA2
       MM SAIDA         ; Colocando na saída
       RS SOMADOR       ; Retornando
# INICIO

