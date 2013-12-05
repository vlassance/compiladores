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
\\usepackage{pdfpages}
\\usepackage{lmodern}			% Usa a fonte Latin Modern			
\\usepackage[T1]{fontenc}		% Selecao de codigos de fonte.
\\usepackage[utf8]{inputenc}		% Codificacao do documento (conversão automática dos acentos)

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
\\begin{figure}[H]
\\includegraphics[width=1.3\\textwidth,angle=90]{./transdutor.pdf}  
\\caption{Transdutor Léxico}
\\end{figure}
""")

for file in os.listdir('../images/'):
    if fnmatch.fnmatch(file, '*.png'):
        filename = file.split('.')[0]
        files.append("""

\\begin{figure}[H]
\\centering 
\\includegraphics[width=1.3\\textwidth,angle=90]{../images/%s.png}  
\\caption{Autômato %s}
\\end{figure}
""" % (filename, filename))

print(LDOC % (LSTLIST, "".join(files)))
