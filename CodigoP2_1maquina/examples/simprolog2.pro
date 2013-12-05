(irmao :- joao, maria)
(pai :- jose, maria)
(pai :- mario, jose)

(pai X,Y :- ((Pai x, z) & (Irmao z, y)))
(pai X,Y :- ((Pai x, z) & (Irmao y, z)))

(avo X,Y :- ((Pai x,z) & (Pai z,y)))

(?- avo mario, joao23a, 45a)
