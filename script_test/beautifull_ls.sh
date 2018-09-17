COLOR := $'\033[91m';
COLOR_RESET := $'\033[0m';

echo -n "<|$COLOR";
echo -n -c "$COLOR_RESET|==|$COLOR" $(ls);
echo "$COLOR_RESET|>";
