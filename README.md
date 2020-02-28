# Shell-Simulator

###Simple commands###
ls -a -l                       : *show the ls results of CWD*
cat file                       : *show file contents*

EDITOR file                    : *gedit, nano, vi, emacs, etc*
	   
cat file > out; ls -l; cat out : *show out file exist and show its contents*
cat file >> out; ls -l         : *show out file size doubled*
cat < file                     : *show file contents*

cat file | grep printf         : *show lines containing printf*
cat < file | grep printf > out : *show pipe with I/O redirection*
cat file | grep printf | more  : *two pipes*
cat file | cat | cat | grep printf : *multiple pipes*
