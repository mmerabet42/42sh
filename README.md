# 42SH

42sh is a 42 school project whose goal is to make a POSIX shell. But the most important aspect of this project was to work on our ability to contribute to a big project in a group.

We deviated from a base of mine but the final project was the result of the contribution of [Jeremi Raymond](https://github.com/jraymond96), [Sebastien Le Rest](https://github.com/SLeRest), [Gaetan Dufay](https://github.com/gdufay) and me [Mohammed Merabet](https://github.com/mmerabet42), we each worked on different aspect of the project but my main duty was to make all the abstraction and the genericity of a system that should simplify the addition of new features.

Thanks to that abstraction, it was easier to get the project at its full potential. For example adding features that were not mandatory, like shell scripting or even job control.

Thanks to our hard work we got a final grade of *121* !

## Features

### Mandatory part

* Prompt display.
* Command line edition.
* Bultins: `cd`, `env`, `setenv`, `unsetenv`, `echo` and `exit`.
* Executing commands from the `PATH` environnement variable.
* The logical operators `&&` and `||`.
* The `;` separator.

### Optional part

* Inhibitors `"` (double quotes), `'` (simple quotes) and `\` (backslash).
* Heredoc `<<` and file descriptor aggregations.
* Globbing : `*`, `?`, `[]`, `{}`, etc.
* Command substitution ``` `` ``` and `$()`.
* Subshells `(command list)` and command groups `{ command list }`.
* Local variables and `unset`, `set`, `export` builtins.
* `history` builtin, (the `!` feature is not handled).
* A complete line edition (i.e the ability to edit a line with various keyboard shortcut).
* The builtin `read`.
* Dynamic autocompletion.

## Very very much appreciated in the grading

* Job control with the `jobs`, `fg`,  and `bg` buitlins and the `&` operator.
* Shell script.

## Additional features

* Arithmetic expansions `$((...))` and `$[...]`.
* Positional argument expansion `$0`, `$1` ... and `$@`.
* The `PROMPT` environnement variable to change the prompt in real time.
* Functions with the `:=` operator `func_name := { command list }`.
* `if` and `while` keyword `if command then command` or `if command then { command list }` or even `if { command list } then { command list }`.
* The `.42shrc` file in the home directory, that is executed at the beginning of the shell.
* `-c` option to give a command in parameter `./42sh -c 'echo Hello World'`.

# Core Team

* [Jeremi Raymond](https://github.com/jraymond96), (The rigorous)
* [Sebastien Le Rest](https://github.com/SLeRest), (The bug finder)
* [Gaetan Dufay](https://github.com/gdufay), (The stagiaire)
* [Mohammed Merabet](https://github.com/mmerabet42), (The genius and The modest)




<p align="center">
  <b><i>**FINAL GRADE: 121**</i></b>
</p>
