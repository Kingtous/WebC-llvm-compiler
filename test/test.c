
void cal(fun_c<int> f) {
    f();
}

int handler() {
    echo('is', 5);
}

int main() {
    cal(handler);
    ret
    0;
}