void init_monitor(int, char*[]);
void engine_start();
int  is_exit_status_bad();
void test_expr();

int main(int argc, char* argv[]) {
    /* Initialize the monitor. */
    init_monitor(argc, argv);

#ifndef TEST_EXPR
    /* Start engine. */
    engine_start();
    return is_exit_status_bad();
#else
    test_expr();
    return 0;
#endif
}
