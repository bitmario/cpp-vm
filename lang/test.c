int main() {
    int num;
    int i;
    int isPrime;
    int primeCount;
    primeCount = 0;
    num = 2;

    while (num < 10000) {
        isPrime = 1;
        i = 2;

        while (i < num) {
            if (num % i == 0) {
                isPrime = 0;
                i = num;
            }
            i = i + 1;
        }
        if (isPrime) {
            primeCount = primeCount + 1;
        }

        num = num + 1;
    }
    return primeCount;
}