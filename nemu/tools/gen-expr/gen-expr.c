#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"#include <stdlib.h>\n"
"#include <signal.h>\n"
"void div_handler(int signal) {"
"  exit(-1);"
"}"
"int main() { "
"  signal(SIGFPE, div_handler);"
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int choose(int num){
    return rand() % num;
}

static void gen_ch(char c){
    sprintf(buf+strlen(buf), "%c", c);
    buf[strlen(buf)] = '\0';
}

static void gen_num(){
    unsigned val = rand();
    sprintf(buf+strlen(buf), "%u", val);
    buf[strlen(buf)] = '\0';
}

static void gen_rand_op(){
    switch(choose(4)){
    case 0:
        strcat(buf+strlen(buf), "+");
        break;
    case 1:
        sprintf(buf+strlen(buf), "%s", "-");
        break;
    case 2:
        strcat(buf+strlen(buf), "*");
        break;
    case 3:
        sprintf(buf+strlen(buf), "%s", "/");
        break;
    }
    buf[strlen(buf)] = '\0';
}

static inline void gen_rand_expr() {
    if(choose(2))
        gen_ch(' ');

    if(strlen(buf) < 1024){
        switch (choose(3)) {
        case 0:
            gen_num();
            break;
        case 1:
            gen_ch('(');
            gen_rand_expr();
            gen_ch(')');
            break;
        default:
            gen_rand_expr();
            gen_rand_op();
            gen_rand_expr();
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    int seed = time(0);
    srand(seed);

    int loop = 1;
    if (argc > 1) {
        sscanf(argv[1], "%d", &loop);
    }

    for (int i = 0; i < loop; i++) {
        memset(buf, 0, strlen(buf));
        gen_rand_expr();

        sprintf(code_buf, code_format, buf);

        FILE* fp = fopen("/tmp/.code.c", "w");
        assert(fp != NULL);
        fputs(code_buf, fp);
        fclose(fp);

        int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
        if (ret != 0)
            continue;

        fp = popen("/tmp/.expr", "r");
        assert(fp != NULL);

        int result;
        fscanf(fp, "%d", &result);
        assert(pclose(fp) != -1);
        

        printf("result == %u, expr == %s\n", result, buf);
    }
    return 0;
}
