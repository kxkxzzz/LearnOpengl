int main() {
    int i;
    int n;
    while (i < n) {
        int j;
        j = 1;
        i = i + 1;
        for (j = 1; j < 5; j = j + 1) {
            i = i * n / n + i;
        }
    }
}