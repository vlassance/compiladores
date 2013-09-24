+0123456789+                    Q0      INT
+0123456789+                    INT     INT
+.+                             INT     FLOAT
+.+                             Q0      FLOAT
+0123456789+                    FLOAT   FLOAT2
+0123456789+                    FLOAT2  FLOAT2
+abcdefghijklmnopqrstuvxywz+    Q0      IDENT
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    Q0      IDENT
+_+                             Q0      IDENT
+0123456789+                    IDENT   IDENT
+abcdefghijklmnopqrstuvxywz+    IDENT   IDENT
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    IDENT   IDENT
+_+                             IDENT   IDENT
#-+*/=#                         Q0      OPER
#=#                             OPER    OPER2
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
+"+                             STR     STR2
@@                              SESCAPE STR
@@                              STR     STR
+'+                             Q0      CHAR
+\+                             CHAR    CESCAPE
@@                              CESCAPE CHAR2
@@                              CHAR    CHAR2
+'+                             CHAR2   CHAR3


@@                              INT     Q0
@@                              IDENT   Q0
@@                              FLOAT2  Q0
@@                              OPER    Q0
@@                              OPER2   Q0
@@                              SPACE   Q0
@@                              DELIM   Q0
@@                              STR2    Q0
@@                              CHAR3   Q0
