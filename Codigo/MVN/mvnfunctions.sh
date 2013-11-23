# mvnrc 
# 
# Author: gpg
#
# Usage:
#  $ source ./mvnfunctions.sh 
#  $ makelib libraries.asm
#  $ makemain mainfile.asm
#    be happy :D

JAVARUN="java -cp"
MVNDL="java -jar MvnPcs4_wDumperLoader.jar"
MLR="$JAVARUN PCS2302_MLR.jar"
MONTA="$MLR montador.MvnAsm"
LINKA="$MLR linker.MvnLinker"
RELOCA="$MLR relocator.MvnRelocator"

MVNFILES=mvnfiles 
# Usage:
# makelib lib.asm 
function makelib () {
    $MONTA $1 && {
        echo `basename $1 .asm`.mvn >> $MVNFILES
        return 0
    }
    return 1
}

function prog_size() {
    lines=$(cat $1 | wc -l)
    echo $lines*2 | bc 1>&2
    echo $lines*2+100 | bc
}

function clean_reloca() {
    rm -f relocados.list
}

function reloca_var() {
    OUTNAME=`basename $1 .mvn`_relocado.mvn
    OUTSIZE=$(prog_size $1)
    echo RELOCANDO $1, comeco: $2, tamanho: $OUTSIZE 1>&2 
    START=$(printf "%X\n" $2)
    $RELOCA $1 $OUTNAME $2 1>&2 || return 1
    cat $OUTNAME 1>&2
    echo $OUTNAME >> relocados.list
    echo $OUTSIZE | bc
}

function makemain () {
    OUTNAME=`basename $1 .asm`.out 
    MAIN=`basename $1 .asm`.mvn 
    BINARY=`basename $1 .asm`

    TMPNAME=tmpfile.tmp

    # make main or die 
    $MONTA $1 || {
        return 1
    }
    {
        clean_reloca 

        SIZEREL=$(prog_size $MAIN) || return 1
        BUTTER=$(cat $MVNFILES | sort -u)
        echo Ligando as bagaca sizerel is $SIZEREL
        $LINKA $MAIN $BUTTER -s $OUTNAME && {
            echo Jogando tudo pra baixo da main 
            $RELOCA $OUTNAME $BINARY $SIZEREL && {
                echo "All Ok!"
                echo -e '\E[37;44m'"\033[1mYour binary is called: $BINARY\033[0m"
                borala
            } || {
                echo Error 
                return 0
            }
        } || {
            echo Error
            return 0
        }
    }
}

function cleanlib () {
    rm -f $MVNFILES
}

function clean_all_mvn () {
    rm -f *.dump
    rm -f *.lst
    rm -f *.mvn
}

function borala () {
    rlwrap $MVNDL
}
