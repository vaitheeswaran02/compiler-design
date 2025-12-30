import java.util.*;

public class NFA_TokenRecognition {

    static Set<String> keywords = new HashSet<>(Arrays.asList(
            "int", "float", "double", "char", "if", "else", "while", "for", "return"
    ));

    static Set<String> operators = new HashSet<>(Arrays.asList(
            "+", "-", "*", "/", "=", "==", "<", ">", "<=", ">="
    ));

    static Set<Character> separators = new HashSet<>(Arrays.asList(
            '(', ')', '{', '}', ';', ','
    ));

    // NFA for Identifier
    static boolean isIdentifier(String token) {
        int state = 0;

        for (char ch : token.toCharArray()) {
            switch (state) {
                case 0:
                    if (Character.isLetter(ch))
                        state = 1;
                    else
                        return false;
                    break;

                case 1:
                    if (Character.isLetterOrDigit(ch))
                        state = 1;
                    else
                        return false;
                    break;
            }
        }
        return state == 1;
    }

    // NFA for Integer Literal
    static boolean isNumber(String token) {
        int state = 0;

        for (char ch : token.toCharArray()) {
            switch (state) {
                case 0:
                    if (Character.isDigit(ch))
                        state = 1;
                    else
                        return false;
                    break;

                case 1:
                    if (Character.isDigit(ch))
                        state = 1;
                    else
                        return false;
                    break;
            }
        }
        return state == 1;
    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        System.out.println("Enter source code (single line):");
        String input = sc.nextLine();

        StringTokenizer st = new StringTokenizer(
                input, " +-*/=<>();{},", true
        );

        System.out.println("\n--- Token Recognition Output ---");

        while (st.hasMoreTokens()) {
            String token = st.nextToken().trim();

            if (token.isEmpty())
                continue;

            if (keywords.contains(token)) {
                System.out.println(token + " : Keyword");
            }
            else if (operators.contains(token)) {
                System.out.println(token + " : Operator");
            }
            else if (token.length() == 1 && separators.contains(token.charAt(0))) {
                System.out.println(token + " : Separator");
            }
            else if (isNumber(token)) {
                System.out.println(token + " : Numeric Literal");
            }
            else if (isIdentifier(token)) {
                System.out.println(token + " : Identifier");
            }
            else {
                System.out.println(token + " : Invalid Token");
            }
        }
        sc.close();
    }
}
