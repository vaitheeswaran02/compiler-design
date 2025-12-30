    import java.util.Scanner;

public class NFA_TokenRecognizer {

    /* ---------- NFA for Identifier ---------- */
    static boolean isIdentifier(String str) {
        int i = 0;

        // q0: must start with a letter
        if (!Character.isLetter(str.charAt(i)))
            return false;

        // q1: letters or digits allowed
        for (i = 1; i < str.length(); i++) {
            if (!Character.isLetterOrDigit(str.charAt(i)))
                return false;
        }
        return true;
    }

    /* ---------- NFA for Integer Constant ---------- */
    static boolean isConstant(String str) {

        // digit+
        for (int i = 0; i < str.length(); i++) {
            if (!Character.isDigit(str.charAt(i)))
                return false;
        }
        return true;
    }

    /* ---------- NFA for Operators ---------- */
    static boolean isOperator(String str) {
        String[] operators = {
                "+", "-", "*", "/", "=", "==", "<", ">", "<=", ">="
        };

        for (String op : operators) {
            if (str.equals(op))
                return true;
        }
        return false;
    }

    /* ---------- MAIN FUNCTION ---------- */
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        String input;

        System.out.println("vaitheeswaran");
        System.out.println("==========================================");

        while (true) {
            System.out.print("\nEnter a token (type 'exit' to stop): ");
            input = sc.nextLine();

            // exit condition
            if (input.equals("exit")) {
                System.out.println("Program terminated.");
                break;
            }

            // epsilon handling
            if (input.equals("()") || input.equals("null")) {
                System.out.println("Epsilon (Empty String) Recognized");
                continue;
            }

            // token recognition
            if (isIdentifier(input))
                System.out.println("Valid Identifier");
            else if (isConstant(input))
                System.out.println("Valid Constant");
            else if (isOperator(input))
                System.out.println("Valid Operator");
            else
                System.out.println("Invalid Token");
                System.out.println("===================================================");
            
        }

        sc.close();
    }
}
