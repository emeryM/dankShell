%{
#include <stdio.h>
#include <y.tab.h>
%}

%% 
end       return END;
[a-zA-Z]  return WORD;
[A-Z]     return UPPERWORD;
[a-Z]     return LOWERWORD;
[0-9]     return INTEGER;
[0-9.]    return DECIMAL;
";"       return SEMICOLON;
"("       return OPEN_PAREN;
")"       return CLOSE_PAREN;
%%