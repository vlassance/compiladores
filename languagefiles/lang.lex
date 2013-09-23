+0123456789+                    Q0      INT
+0123456789+                    INT     INT
+.+                             INT     FLOAT
+.+                             Q0      FLOAT
+0123456789+                    FLOAT   FLOAT2
+0123456789+                    FLOAT2  FLOAT2
+abcdefghijklmnopqrstuvxywz+    Q0      LETTER
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    Q0      LETTER
+_+                             Q0      LETTER
+0123456789+                    LETTER  FLOAT2
+abcdefghijklmnopqrstuvxywz+    LETTER  LETTER
+ABCDEFGHIJKLMNOPQRSTUVXYWZ+    LETTER  LETTER
+_+                             LETTER  LETTER
#-+*/=#                         Q0      OPER
#=#                             OPER    OPER2
+ +                             Q0      SPACE
+	+                       Q0      SPACE
+
+                               Q0      SPACE 
+()[]{};+                       Q0      DELIM
+"+                             Q0      STR
+\+                             STR     SESCAPE
+"+                             STR     STR2
++                              SESCAPE STR
++                              STR     STR
+'+                             Q0      CHAR
+\+                             CHAR    CESCAPE
++                              CESCAPE CHAR2
++                              CHAR    CHAR2
+'+                             CHAR2   CHAR3

