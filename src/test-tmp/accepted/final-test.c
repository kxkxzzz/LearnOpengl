// test for

const int str_len = 18;
const char str[] = "abcdefg0123456789";

void solve() {
}

/*
    @is_digit
    @param ch
 */
const int is_digit(char ch) {
    int re = 0;
    if (ch >= '0' + 0) {
        if (ch <= '0' + 9) {
            re = 1;
        }
    }
    return re;
}

int is_letter(char ch) {
    int re = 0;
    if (ch >= 'a') {
        if (ch <= 'z') {
            re = 1;
        }
    }
    if (ch >= 'A') {
        if (ch <= 'Z') {
            re = 1;
        }
    }
    return re;
}

void input(void) {
    // input
}

long dpf(int n, int val[], int cost[]) {
    int dp[105];
    for (int i = 0; i < n; i = i + 1) {
        for (int j = 100; j >= val[i]; j = j - 1) {
            if (dp[j] < dp[j - cost[i]] + val[i]) {
                dp[j] = dp[j - cost[i]] + val[i];
            }
        }
    }
    return dp[100];
}

int main() {
    int cnt_digit = 0;
    int cnt_letter = 0;
    int i;
    for (i = 0; i < str_len; i = i + 1) {
        char ch = str[i];
        if (is_digit(ch)) {
            cnt_digit = cnt_digit + 1;
        }
        else if (is_letter(ch) == 1) {
            cnt_letter = cnt_letter + 1;
        }
    }

    int output;
    {
        output = cnt_digit * cnt_digit + (cnt_letter * cnt_letter);
        output = cnt_digit / cnt_digit * (cnt_digit - cnt_letter) - 1;
    }

    while (0) {
        int sum = 0;
        {
            for (int i = 0; i < 5; i = i + 1) {
                sum = sum + i;
            }
        }
        if (sum < 0) {
            return 1;
        }
    }

    return 0;
}
