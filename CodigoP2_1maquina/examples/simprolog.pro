(listaFatos :- a, 1, 3)
(fatoSimples :- 3)
(verdades :- verdade)
(pai a,b :- verdade)   
(pai b,c :- verdade)   
(avo X,Y :- ((Pai x,z) & (Pai z,y)))
(?- avo pai, pai)