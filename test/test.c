int global[5] = {1, 2, 3, 4, 5};

int sum2d(int a[][2], int length) {
    int total = 0;
    for (int i = 0; i < length; i = i + 1) {
        for (int j = 0; j < 2; j = j + 1) {
            total = total + a[i][j];
        }
    }
    return total;
}

int sum(int a[], int length) {
    int total = 0;
    for (int i = 0; i < length; i = i + 1) {
        total = total + a[i];
    }
    return total;
}

int ctotal = 0;

int recursive_call(int cnt) {
    if (cnt == 0) {
        return ctotal;
    }
    ctotal = ctotal + cnt;
    return recursive_call(cnt - 1);
}