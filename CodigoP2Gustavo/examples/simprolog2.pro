(irmao :- joao, maria)
(pai :- jose, maria)
(pai :- mario, jose)

(pai X,Y :- ((pai X, Z) & (irmao Z, Y)))
(pai X,Y :- ((pai X, Z) & (irmao Y, Z)))

(avo X,Y :- ((pai X,Z) & (pai Z,Y)))

(?- avo mario, joao)
