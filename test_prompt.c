#include <stdio.h>
int main() {
    char *prompt = "\nroot@sageos:/# ";
    printf("Prompt len: %lu\n", (unsigned long)sizeof(prompt));
    printf("Prompt len: %lu\n", (unsigned long)15);
    return 0;
}
