4a entrega
==========

Gustavo Pacianotto Gouveia
Victor Lassance de Oliveira
---------------------------

Esta entrega contém o ambiente de execução da linguagem CZAR para a MVN da
Escola Politécnica da Universidade de São Paulo.

Existe um arquivo compilado na pasta raiz com o nome ./usestd, este arquivo
contém a implementação (em formato bruto MVN) das bibliotecas padrão.

No entanto, se for necessária a análise do funcionamento, deve-se fazer o
seguinte:

 $ . ./mvnfunctions.sh 
 $ makelib std.asm
 $ makelib stdio.asm
 $ makemain usestd.asm

após executar estes comandos, deve-se executar a mvn com os seguintes comandos:

 > p usestd
 > r 000 

existe um arquivo com um exemplo de execução na pasta (./exemplo_de_execucao.txt)

versão do bash e java utilizada para este exercício:

$  sakura --version
sakura version is 3.1.0

$  java -version
java version "1.7.0_45"
OpenJDK Runtime Environment (IcedTea 2.4.3) (ArchLinux build 7.u45_2.4.3-1-x86_64)
OpenJDK 64-Bit Server VM (build 24.45-b08, mixed mode)

$  bash --version
GNU bash, version 4.2.45(2)-release (x86_64-unknown-linux-gnu)
Copyright (C) 2011 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later
<http://gnu.org/licenses/gpl.html>

This is free software; you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
