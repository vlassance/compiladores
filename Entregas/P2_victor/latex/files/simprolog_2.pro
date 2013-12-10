(irmao :- joao, maria)
(pai :- jose, maria)
(pai :- mario, jose)
(mae :- joana, maria)
(pai :- joaquim, joana)

(pai X,Y :- ((pai X, Z) & (irmao Z, Y)))
(pai X,Y :- ((pai X, Z) & (irmao Y, Z)))

(avo X,Y :- ((pai X, Z) & (pai Z,Y) or (pai X, Z) & (mae Z, Y)))

(?- avo mario, joaquim)
