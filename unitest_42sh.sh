#!/bin/bash

SHB="/bin/bash --posix"
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
 diff ${OUT} ${OUTB} > /dev/null
 if [ $? = 0 ]
 then
	 echo -n ""
	#printf "${COMMAND} ${GREEN}OK${NC}\n"
 else
	printf "${RED}42sh: \"${COMMAND}\"${NC}\n"
	cat ${OUT}
	printf "${RED}bash: \"${COMMAND}\"${NC}\n"
	cat ${OUTB}
	echo "------------"
 fi
}

check_diff_error() {
 ${SH} ${COMMAND} > ${OUT} 2>&1
 RES_42SH=$?
 echo ${COMMAND} | ${SHB} > ${OUTB} 2>&1
 RES_BASH=$?
 if [ $RES_BASH = $RES_42SH ]
 then
	 echo -n ""
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

COMMAND=">"
check_diff_error

COMMAND=">>"
check_diff_error

COMMAND="<"
check_diff_error

COMMAND="<<"
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

COMMAND="true && true || echo 1"
check_diff

COMMAND="true && false || echo 3"
check_diff

COMMAND="false && true || echo 5"
check_diff

COMMAND="false && false || echo 7 ; true"
check_diff

COMMAND="true || true && echo 9"
check_diff

COMMAND="true || false && echo 11"
check_diff

COMMAND="false || true && echo 13"
check_diff

COMMAND="false || false && echo 15 ; true"
check_diff

COMMAND="true || false && ls"
check_diff

COMMAND="cd /tmp && cd / | ls"
check_diff

COMMAND="echo a > file | echo b > file; cat file ; rm file"
check_diff

COMMAND="echo a||echo b&&echo c&&echo d;false&&false||false&&false;true"
check_diff

COMMAND="echo lol && echo pouet && echo truc"
check_diff

COMMAND="false && echo lol;true"
check_diff

COMMAND="false && echo pouet;true"
check_diff

COMMAND="echo pouet || echo lol"
check_diff

COMMAND="true && false && ls"
check_diff

COMMAND="true && false || ls"
check_diff

COMMAND="true || false && ls"
check_diff

COMMAND="true || false || ls"
check_diff

COMMAND="false && true && ls"
check_diff

COMMAND="false && true || ls"
check_diff

COMMAND="false || true && ls"
check_diff

COMMAND="false || true || ls"
check_diff

COMMAND="echo check only && echo priority || echo operators;"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nredir\n~~~~~~~~~~~~~\n${NC}"

COMMAND="cat 4< auteur 3<&4 2<&3 <&2 | cat -e"
check_diff

COMMAND="echo lol > prout ; cat prout; rm prout"
check_diff

COMMAND="ls -z 1>&-"
check_diff

COMMAND="ls -z 1>&- 2>&1"
check_diff

COMMAND="ls -z 2>&1 1>&-"
check_diff

COMMAND="ls | cat -e > prout ; cat prout; rm prout"
check_diff

COMMAND="ls | wc -l > toto; echo abc | wc -l >> toto; cat -e toto ; ls > titi; cat titi; rm titi; rm -rf toto"
check_diff

COMMAND="echo 1 2 | cat -e > file && cat file && rm file"
check_diff

COMMAND="ls > toto ; date >> toto ; >> toto ls ; < toto cat ;"
check_diff

COMMAND="> file && cat file && rm file"
check_diff

COMMAND="echo > file lol >> file mdr && cat file && rm file"
check_diff

COMMAND="echo > file0 lol >> file1 mdr && cat file0 && cat file1 && rm file0 && rm file1"
check_diff

COMMAND="echo toto > in ; cat -e >out<in ; cat out ; rm out in"
check_diff

COMMAND="echo toto > in ; cat -e >out <in ; cat out ; rm out in"
check_diff

COMMAND="echo toto > file ; cat 3< file <&3 ; rm file"
check_diff

COMMAND="echo prout > file ; cat < file <&9 9<&0 ; rm file"
check_diff

COMMAND="echo def > /tmp/toto ; cat 9</tmp/toto 8<&9 7<&8 -e 6<&7 5<&6 4<&5 3<&4 2<&3 <&2; rm /tmp/toto"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\npipe\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo a | cat -e"
check_diff

COMMAND="echo a | cat | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nglobbing\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo src/*.sh"
check_diff

COMMAND="echo src/*.c"
check_diff

COMMAND="echo \"\*\""
check_diff

COMMAND="echo '*'"
check_diff

COMMAND="echo */*.h"
check_diff

COMMAND="echo */*.c"
check_diff

COMMAND="echo "*/*.h""
check_diff

COMMAND="echo "*/*.c""
check_diff

COMMAND="echo '*/*.h'"
check_diff

COMMAND="echo '*/*.c'"
check_diff

COMMAND="echo *"
check_diff

COMMAND="echo s?c ob? ???? ?"
check_diff

COMMAND="echo src/[a-zA-Z]*.c"
check_diff

COMMAND="echo {src,obj}"
check_diff

COMMAND="echo {a,c,b}"
check_diff

COMMAND="echo {a}"
check_diff

COMMAND="echo */*/*.{h,c,o}"
check_diff

COMMAND="echo {src,obj}/*"
check_diff

COMMAND="echo {src,obj}/*/{h,c,o}"
check_diff

COMMAND="echo {src,obj}/*/*.{h,c,o}"
check_diff

COMMAND="echo {src,obj,}/*/*.{h,c,o}"
check_diff

COMMAND="echo {src,obj,include}/*/*.{h,c,o}"
check_diff

COMMAND="echo {{src,obj},include}/*/*.{h,c,o}"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nbin\n~~~~~~~~~~~~~\n${NC}"

COMMAND="/bin/ls /"
check_diff

COMMAND="/bin/ls -laF /"
check_diff

COMMAND="/bin/ls -l -a -F /"
check_diff

COMMAND="ls ~"
check_diff

COMMAND="ls -laF ~"
check_diff

COMMAND="ls -l -a -F ~"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nmixe\n~~~~~~~~~~~~~\n${NC}"

COMMAND="ls this_file_does_not_exist 2>&1 > /tmp/redir_test_file;rm -f /tmp/redir_test_file;echo abc && echo def; echo 123 > /tmp/test_redir_file_stdout;echo \"---\" ; echo \"---\" ;cat /tmp/test_redir_file_stdout;rm -f /tmp/test_redir_file_stdout"
check_diff

COMMAND="echo 'abc' > /tmp/redir_stdin_close;/bin/cat -e <&- </tmp/redir_stdin_close;echo 'def' <&- </tmp/redir_stdin_close;echo end;cat -e /tmp/redir_stdin_close;rm -f /tmp/redir_stdin_close"
check_diff

COMMAND="echo lol | wc -l | xargs echo > /tmp/testfile ; cat /tmp/testfile;cat -e < /tmp/testfile | wc -c | xargs echo;cat -e < /tmp/testfile | wc -c | xargs echo > /tmp/testfile2;cat /tmp/testfile2 ; rm /tmp/testfile /tmp/testfile2"
check_diff

COMMAND="echo def > /tmp/redir_one_to_all;cat 9</tmp/redir_one_to_all 8<&9 7<&8 6<&7 -e 5<&6 4<&5 3<&4 2<&3 1<&2 <&1;rm -f /tmp/redir_one_to_all"
check_diff

COMMAND="echo abcd > /tmp/redir_multi_op_great >> /tmp/redir_multi_op_dgreat;file /tmp/redir_multi_op_great;file /tmp/redir_multi_op_dgreat;rm -f /tmp/redir_multi_op_great /tmp/redir_multi_op_dgreat"
check_diff

COMMAND="echo merde > /tmp/redir_echo_in;cat 3</tmp/redir_echo_in <&3;rm -f /tmp/redir_echo_in"
check_diff

COMMAND="ls;"
check_diff

COMMAND="true && true || echo 1"
check_diff

COMMAND="true && false || echo 3"
check_diff

COMMAND="false && true || echo 5"
check_diff

COMMAND="false && false || echo 7 ; true"
check_diff

COMMAND="true || true && echo 9"
check_diff

COMMAND="true || false && echo 11"
check_diff

COMMAND="false || true && echo 13"
check_diff

COMMAND="false || false && echo 15 ; true"
check_diff

COMMAND="echo a&&echo b;echo c||echo d"
check_diff

COMMAND="echo a&&echo b;echo c||echo d;"
check_diff

COMMAND=""
check_diff

COMMAND="cat toto"
check_diff

COMMAND="cat < NO_SUCH_file"
check_diff

COMMAND="echo lol && echo pouet && echo truc; cat pouet && echo lol; false && echo pouet"
check_diff

COMMAND="env env env env env env env -i env ls"
check_diff

COMMAND="cd /; cd -; cd /; cd ../; cd; pwd"
check_diff

COMMAND="ls | wc -l > toto; echo abc | wc -l >> toto; cat -e toto ; ls > titi; cat titi; rm titi; rm -rf toto"
check_diff

COMMAND=""
check_diff

COMMAND="ls /dev | grep tty | sort -r | rev > toto ; < toto cat | rev | wc -l > titi ; rm -rf titi"
check_diff

COMMAND="echo ~"
check_diff

COMMAND="echo ~/toutou"
check_diff

COMMAND="echo true || echo false && echo maarek && echo joseph"
check_diff


COMMAND="echo toto; echo tata; echo titi;"
check_diff

COMMAND="echo toto; echo tata; echo titi; echo jojo; echo jiji; echo jaja"
check_diff

COMMAND="echo toto tata titi tete tutu toutou tuitui touatoua touytouy merciiiiiiiiiiiii"
check_diff

COMMAND="rm -rf toto"
check_diff

COMMAND="echo "\$TERM""
check_diff

COMMAND="ls this_file_does_not_exist 2>&1 > /tmp/redir_test_file;rm -f /tmp/redir_test_file;echo abc && echo def; echo 123 > /tmp/test_redir_file_stdout;echo \"---\" ; echo \"---\" ;cat /tmp/test_redir_file_stdout;rm -f /tmp/test_redir_file_stdout"
check_diff

COMMAND="echo 'abc' > /tmp/redir_stdin_close;/bin/cat -e <&- </tmp/redir_stdin_close;echo 'def' <&- </tmp/redir_stdin_close;echo end;cat -e /tmp/redir_stdin_close;rm -f /tmp/redir_stdin_close"
check_diff

COMMAND="echo lol | wc -l | xargs echo > /tmp/testfile ; cat /tmp/testfile;cat -e < /tmp/testfile | wc -c | xargs echo;cat -e < /tmp/testfile | wc -c | xargs echo > /tmp/testfile2;cat /tmp/testfile2 ; rm /tmp/testfile /tmp/testfile2"
check_diff

COMMAND="echo a | cat -e"
check_diff

COMMAND="echo a | cat | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e"
check_diff

COMMAND="echo def > /tmp/redir_one_to_all;cat 9</tmp/redir_one_to_all 8<&9 7<&8 6<&7 -e 5<&6 4<&5 3<&4 2<&3 1<&2 <&1;rm -f /tmp/redir_one_to_all"
check_diff

COMMAND="echo abcd > /tmp/redir_multi_op_great >> /tmp/redir_multi_op_dgreat;file /tmp/redir_multi_op_great;file /tmp/redir_multi_op_dgreat;rm -f /tmp/redir_multi_op_great /tmp/redir_multi_op_dgreat"
check_diff

COMMAND="echo merde > /tmp/redir_echo_in;cat 3</tmp/redir_echo_in <&3;rm -f /tmp/redir_echo_in"
check_diff

COMMAND="ls;"
check_diff

COMMAND="true && true   || echo 1"
check_diff

COMMAND="true && false   || echo 3"
check_diff

COMMAND="false && true   || echo 5"
check_diff

COMMAND="false && false   || echo 7 ; true"
check_diff

COMMAND="true ||   true && echo 9"
check_diff

COMMAND="true ||   false && echo 11"
check_diff

COMMAND="false ||   true && echo 13"
check_diff

COMMAND="false ||   false && echo 15 ; true"
check_diff

COMMAND="echo a&&echo b;echo c||echo d"
check_diff

COMMAND="echo a&&echo b;echo c||echo d;"
check_diff

COMMAND="cat toto"
check_diff

COMMAND="cat < NO_SUCH_file"
check_diff

COMMAND="echo lol && echo pouet && echo truc; cat pouet && echo lol; false && echo pouet"
check_diff

COMMAND="env env env env env env env -i env ls"
check_diff

COMMAND="cd /; cd -; cd /; cd ../; cd;"
check_diff

COMMAND="ls /dev | grep tty | sort -r | rev > toto ; < toto cat | rev | wc -l > titi ; rm -rf titi"
check_diff

COMMAND="echo ~"
check_diff

COMMAND="echo ~/toutou"
check_diff

COMMAND="echo true || echo false && echo maarek && echo joseph"
check_diff

COMMAND="echo check only && echo priority || echo operators;"
check_diff

COMMAND="echo toto; echo tata; echo titi;"
check_diff

COMMAND="echo toto; echo tata; echo titi; echo jojo; echo jiji; echo jaja"
check_diff

COMMAND="echo toto tata titi tete tutu toutou tuitui touatoua touytouy merciiiiiiiiiiiii"
check_diff

COMMAND="rm -rf toto"
check_diff

COMMAND="echo \"\$TERM\""
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

COMMAND="cd ///// ; pwd"
check_diff

COMMAND="cd /////tmp ; pwd"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nbuiltin echo && quote\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo 1 2 3"
check_diff

COMMAND="echo -n 1 2 3"
check_diff

COMMAND="echo \"\$PATH\""
check_diff

COMMAND="echo '\$PATH'"
check_diff

COMMAND="echo \"\$PATH\$PWD\""
check_diff

COMMAND="echo '\$PATH\$PWD'"
check_diff

COMMAND="echo \"\$PATH\$NOEXIST\""
check_diff

COMMAND="echo '\$PATH\$NOEXIST'"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nenv setenv unsetenv\n~~~~~~~~~~~~~\n${NC}"

COMMAND="env -i env"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nblack quote\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo \`ls\`"
check_diff

COMMAND="echo \`ls | cat -e\` > file && cat file"
check_diff

COMMAND="echo test > file && echo \`cat file\` && rm file"
check_diff

COMMAND="echo test > file && echo \`cat file | cat -e\` && rm file"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\nsubshell\n~~~~~~~~~~~~~\n${NC}"

COMMAND="(cd / ; ls)"
check_diff

COMMAND="(cat -e) | (cat -n)"
check_diff

COMMAND="(cat -e; (cat -n; cat -v); ( ( cat -t ) ) )"
check_diff

COMMAND="(cat -n; cat -e)"
check_diff

COMMAND="(ls ; (cat | cat -e)) | (cat -n)"
check_diff

COMMAND="(ls ;cat -e) | cat -e  | (ls) | cat -e"
check_diff

COMMAND="(pwd; cat ) | (pwd; cat -e)"
check_diff

COMMAND="ls | (cat -e | cat -n)"
check_diff

COMMAND="ls | (cat -e | cat -n) | (rev| ( ( cat -e ) ) )"
check_diff

printf "${ORANGE}\n~~~~~~~~~~~~~\narithmetique\n~~~~~~~~~~~~~\n${NC}"

COMMAND="echo \$((1+3))"
check_diff

COMMAND="echo \$((1 +         3))"
check_diff

COMMAND="echo \$[1 +         3]"
check_diff

COMMAND="echo \$[ 1 +              3 - 50          ]"
check_diff

COMMAND="echo \$[ 1 +
          3 - 50
              ]m tmp tmp2"
check_diff

rm tmp tmp2
