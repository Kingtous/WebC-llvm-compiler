// this is a comment。这是个注释xxx
int param = 100;
// 求[1,param)整数和
int getNum() {
    int test[2][3] = {9, 8, 7, 6, 5, 4};
    int a[2] = {test[0][1], test[1][1]};
    while (test[0][0] > 0) {
        test[0][0] = test[0][0];
    }
    return test[0][0];
}
