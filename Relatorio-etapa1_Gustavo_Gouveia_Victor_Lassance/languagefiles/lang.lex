+0123456789+                    Q0      INT
+0123456789+                    INT     INT
+.+                             INT     FLOAT
+.+                             Q0      FLOAT
+0123456789+                    FLOAT   FLOAT_2
+0123456789+                    FLOAT_2  FLOAT_2
+abcdefghijklmnopqrstuvxywz+    Q0      IDENT
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    Q0      IDENT
+_+                             Q0      IDENT
+0123456789+                    IDENT   IDENT
+abcdefghijklmnopqrstuvxywz+    IDENT   IDENT
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    IDENT   IDENT
+_+                             IDENT   IDENT
#!><-+*/=#                      Q0      OPER
#=#                             OPER    OPER_2
+ +                             Q0      SPACE
+	+                       Q0      SPACE
+
+                               Q0      SPACE
+ +                             SPACE   SPACE
+	+                       SPACE   SPACE
+
+                               SPACE   SPACE 
+()[]{};+                       Q0      DELIM
+"+                             Q0      STR
+\+                             STR     SESCAPE
+"+                             STR     STR_2
@@                              SESCAPE STR
@@                              STR     STR
+'+                             Q0      CHAR
+\+                             CHAR    CESCAPE
+0abtnvfre\’"+                  CESCAPE CHAR_2
@@                              CHAR    CHAR_2
+'+                             CHAR_2  CHAR_3
+#+                             Q0      COMMENTS
+
+                               COMMENTS COMMENTS_2
@@                              COMMENTS COMMENTS


@@                              COMMENTS_2 Q0
@@                              INT     Q0
@@                              IDENT   Q0
@@                              FLOAT_2  Q0
@@                              OPER    Q0
@@                              OPER_2   Q0
@@                              SPACE   Q0
@@                              DELIM   Q0
@@                              STR_2    Q0
@@                              CHAR_3   Q0
