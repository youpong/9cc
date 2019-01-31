public class Fibonacci {
    public static void main(String[] args) {
	int n = Integer.parseInt(args[0]);
	System.out.println(fibonacci(n));
    }
    public static int fibonacci(int n) {
	if (n == 0)
	    return 0;
	else if (n == 1)
	    return 1;
	else
	    return fibonacci(n - 2) + fibonacci(n - 1);
    }
}
