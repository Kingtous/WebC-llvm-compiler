int global[5] = {1, 2, 3, 4, 5};

int sum(int a[], int length) {
    int total = 0;
    for (int i = 0; i < length; i = i + 1) {
        total = total + a[i];
    }
    return total;
}

int main() {
    int params = 5;
    int a[5] = {1, 2, 3, 4, 5};
    return sum(a, params);
}