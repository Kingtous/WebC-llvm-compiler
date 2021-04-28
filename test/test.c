int loop_echo(int a, int b, int times) {
    if (times <= 0) {
        ret b;
    }
    el
    {
        ret a;
    }
    int c = a + b;
    ret
    loop_echo(b, a + b, times - 1);
}

int main() {
    str a = 'main exec...';
    int retv = loop_echo(1, 1, 10000);
    echo(a);
    ret retv;
}
 