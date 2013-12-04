import fnmatch
import os

LDOC = """ 
\\documentclass[12pt]{article}
%s
\\begin{document}
%s
\\end{document} 
"""



LSTLIST = """ 
\\usepackage{listings}
\\usepackage{color}
\\usepackage{graphicx}
\\usepackage{float}

\\definecolor{mygreen}{rgb}{0,0.6,0}
\\definecolor{mygray}{rgb}{0.5,0.5,0.5}
\\definecolor{mymauve}{rgb}{0.58,0,0.82}

\\lstset{ 
  backgroundcolor=\\color{white},
  basicstyle=\\ttfamily\\tiny,
  breakatwhitespace=false,         
  breaklines=true,                 
  captionpos=b,                    
  commentstyle=\\color{mygreen},    
  escapeinside={\\%*}{*)},          
  extendedchars=true,              
  frame=single,                    
  keepspaces=true,                 
  keywordstyle=\\color{blue},       
  language=Octave,                 
  morekeywords={=,->},            
  numbers=left,                    
  numbersep=5pt,                   
  numberstyle=\\tiny\\color{mygray}, 
  rulecolor=\\color{black},         
  showspaces=false,                
  showstringspaces=false,          
  showtabs=false,                  
  stepnumber=2,                    
  stringstyle=\\color{mymauve},     
  tabsize=2,                       
  title=\\lstname
}

"""

files = ["""
\\lstinputlisting[title=WIRTH]{../WIRTH.txt}
"""]

for file in os.listdir('../output/'):
    if fnmatch.fnmatch(file, '*.mdfa'):
        filename = file.split('.')[0]
        files.append("""
\\lstinputlisting[title=AFD %s]{../output/%s}
""" % (filename, file))

files.append("""
\\begin{itemize}
""")

for file in os.listdir('../images/'):
    if fnmatch.fnmatch(file, '*.png'):
        filename = file.split('.')[0]
        files.append("""
\\item AFD %s

\\begin{figure}[H]
\\centering 
\\includegraphics[width=0.8\\textwidth]{../images/%s.png}  
\\caption{Autômato %s}
\\end{figure}
""" % (filename, filename, filename))

files.append("""
\\end{itemize}
""")

print(LDOC % (LSTLIST, "".join(files)))
