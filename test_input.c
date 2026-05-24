#include <stdio.h>
#include <string.h>

void str_insert(char *s, int pos, char ch) {
    int len = strlen(s);
    memmove(s + pos + 1, s + pos, len - pos + 1);
    s[pos] = ch;
}

int main() {
    char s[20] = "hello";
    str_insert(s, 0, 'A');
    printf("Inserted at 0: %s\n", s);
    
    char s2[20] = "hello";
    str_insert(s2, 5, 'A');
    printf("Inserted at 5: %s\n", s2);
    
    return 0;
}
