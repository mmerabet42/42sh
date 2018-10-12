#!/bin/bash

SHB=/bin/bash
SH=./42sh
OUT=tmp
OUTB=tmp2
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
NC='\033[0;m'
COMMAND=""

check_diff() {
 echo ${COMMAND} | ${SH} > ${OUT} 2>&1
 echo ${COMMAND} | ${SHB} > ${OUTB} 2>&1
 diff ${OUT} ${OUTB}
 if [ $? = 0 ]
 then
	printf "${COMMAND} ${GREEN}OK${NC}\n"
 else
	printf "${RED}42sh: \"${COMMAND}\"${NC}\n"
	echo "------------"
	cat ${OUT}
	printf "${RED}bash: \"${COMMAND}\"${NC}\n"
	echo "------------"
	cat ${OUTB}
 fi
}

check_diff_error() {
 echo ${COMMAND} | ${SH} > ${OUT} 2>&1
 RES_42SH=$?
 echo ${COMMAND} | ${SHB} > ${OUTB} 2>&1
 RES_BASH=$?
 if [ $RES_BASH = $RES_42SH ]
 then
	printf "${COMMAND} ${GREEN}OK${NC}\n"
 else
	printf "${RED}42sh: \"${COMMAND}\": $RES_42SH${NC}\n"
	printf "${RED}bash: \"${COMMAND}\": $RES_BASH${NC}\n"
	echo "------------"
 fi
}


printf "${ORANGE}\n~~~~~~~~~~~~~\nerr syntax\n~~~~~~~~~~~~~\n${NC}"

COMMAND="&&"
check_diff_error

COMMAND="|"
check_diff_error

COMMAND="||"
check_diff_error

COMMAND=";"
check_diff_error

printf "${ORANGE}\n~~~~~~~~~~~~~\nast\n~~~~~~~~~~~~~\n${NC}"

COMMAND="cd .. && ls -1 | cat -e"
check_diff

COMMAND="cd .. && ls -1 | cat -e | cat -e | cat -e | wc -c"
check_diff

COMMAND="echo mdr | cat -e > prout && echo prout >> prout lol && cat prout && rm prout"
check_diff

COMMAND="cat noexist || echo print"
check_diff

COMMAND="cat unitest_42sh.sh || echo dont print"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nredir\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo lol > prout ; cat prout; rm prout"
check_diff

COMMAND="echo 1 2 | cat -e > file && cat file && rm file"
check_diff

#COMMAND="ls > toto ; date >> toto ; >> toto ls ; < toto cat ;"
#check_diff

#COMMAND="> file && cat file && rm file"
#check_diff

COMMAND="echo > file lol >> file mdr && cat file && rm file"
check_diff

COMMAND="echo > file0 lol >> file1 mdr && cat file0 && cat file1 && rm file0 && rm file1"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nbuiltin cd\n~~~~~~~~~~~~~\n${NC}"

COMMAND="cd .. && ls"
check_diff

COMMAND="cd ~ ; cd - && ls"
check_diff

COMMAND="cd ../../../../../../../../../../../../../../..//../../../ ; pwd ; cd -"
check_diff

COMMAND="cd ~ ; cd -P goinfre ; pwd"
check_diff

COMMAND="cd ~ ; cd -L goinfre ; pwd"
check_diff


printf "${ORANGE}\n~~~~~~~~~~~~~\nbuiltin echo && quote\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo 1 2 3"
check_diff

COMMAND="echo -n 1 2 3"
check_diff

COMMAND="echo \"\$PATH\""
check_diff

COMMAND="echo \'\$PATH\'"
check_diff

COMMAND="echo \"\$PATH\$PWD\""
check_diff

COMMAND="echo \'\$PATH\$PWD\'"
check_diff

COMMAND="echo \"\$PATH\$NOEXIST\""
check_diff

COMMAND="echo \'\$PATH\$NOEXIST\'"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nenv setenv unsetenv\n~~~~~~~~~~~~~\n${NC}"

COMMAND="env -i env"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nblack quote\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo \`ls\`"
check_diff

#COMMAND="echo test > file && echo \`cat file\` && rm file"
#check_diff
#
#COMMAND="echo test > file && echo \`cat file | cat -e\` && rm file"
#check_diff

#rm tmp tmp2
