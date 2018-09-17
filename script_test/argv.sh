echo Number of arguments: $#;
i:=0;
n:=$#;
while $[i < n] then
	echo Argument \#$i: "'${$[i++]}'";
