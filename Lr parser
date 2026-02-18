import java.util.*;

public class LR0Parser {

    static Map<Character, List<String>> grammar = new LinkedHashMap<>();
    static List<Set<String>> states = new ArrayList<>();
    static Map<Integer, Map<String, String>> action = new LinkedHashMap<>();
    static Map<Integer, Map<Character, Integer>> goTo = new LinkedHashMap<>();
    static Set<String> terminals = new LinkedHashSet<>();
    static Set<Character> nonTerminals = new LinkedHashSet<>();

    static char startSymbol;
    static char augmentedStart;

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of productions: ");
        int n = sc.nextInt();
        sc.nextLine();

        System.out.println("Enter productions (use # for epsilon):");

        for (int i = 0; i < n; i++) {
            String p = sc.nextLine();
            char lhs = p.charAt(0);
            grammar.putIfAbsent(lhs, new ArrayList<>());
            nonTerminals.add(lhs);

            String rhs = p.substring(3);
            for (String prod : rhs.split("\\|")) {
                grammar.get(lhs).add(prod);
                for (char c : prod.toCharArray()) {
                    if (!Character.isUpperCase(c) && c != '#')
                        terminals.add("" + c);
                }
            }
        }

        terminals.add("$");

        startSymbol = grammar.keySet().iterator().next();
        augmentedStart = 'Z'; // augmented symbol
        grammar.put(augmentedStart, Arrays.asList("" + startSymbol));

        buildCanonicalCollection();
        buildParsingTable();
        printParsingTable();

        System.out.print("\nEnter input string (end with $): ");
        String input = sc.nextLine();
        parse(input);
    }

    // ================= CLOSURE =================

    static Set<String> closure(Set<String> items) {

        Set<String> closure = new LinkedHashSet<>(items);
        boolean changed;

        do {
            changed = false;

            Set<String> temp = new LinkedHashSet<>(closure);

            for (String item : temp) {
                int dotPos = item.indexOf('.');
                if (dotPos < item.length() - 1) {
                    char symbol = item.charAt(dotPos + 1);

                    if (Character.isUpperCase(symbol)) {
                        for (String prod : grammar.get(symbol)) {
                            String newItem = symbol + "->." + prod;
                            if (!closure.contains(newItem)) {
                                closure.add(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        } while (changed);

        return closure;
    }

    // ================= GOTO =================

    static Set<String> goTo(Set<String> state, char symbol) {

        Set<String> moved = new LinkedHashSet<>();

        for (String item : state) {
            int dotPos = item.indexOf('.');
            if (dotPos < item.length() - 1 &&
                item.charAt(dotPos + 1) == symbol) {

                String movedItem = item.substring(0, dotPos)
                        + symbol + "."
                        + item.substring(dotPos + 2);

                moved.add(movedItem);
            }
        }

        return closure(moved);
    }

    // ================= CANONICAL COLLECTION =================

    static void buildCanonicalCollection() {

        Set<String> startState = new LinkedHashSet<>();
        startState.add(augmentedStart + "->." + startSymbol);

        states.add(closure(startState));

        boolean changed;

        do {
            changed = false;

            List<Set<String>> newStates = new ArrayList<>();

            for (Set<String> state : states) {

                Set<Character> symbols = new LinkedHashSet<>();
                for (String item : state) {
                    int dotPos = item.indexOf('.');
                    if (dotPos < item.length() - 1)
                        symbols.add(item.charAt(dotPos + 1));
                }

                for (char sym : symbols) {
                    Set<String> next = goTo(state, sym);
                    if (!next.isEmpty() && !states.contains(next)
                            && !newStates.contains(next)) {
                        newStates.add(next);
                        changed = true;
                    }
                }
            }

            states.addAll(newStates);

        } while (changed);
    }

    // ================= TABLE =================

    static void buildParsingTable() {

        for (int i = 0; i < states.size(); i++) {
            action.put(i, new LinkedHashMap<>());
            goTo.put(i, new LinkedHashMap<>());
        }

        for (int i = 0; i < states.size(); i++) {

            Set<String> state = states.get(i);

            for (String item : state) {

                int dotPos = item.indexOf('.');

                // SHIFT
                if (dotPos < item.length() - 1) {
                    char symbol = item.charAt(dotPos + 1);
                    Set<String> next = goTo(state, symbol);
                    int j = states.indexOf(next);

                    if (!Character.isUpperCase(symbol))
                        action.get(i).put("" + symbol, "S" + j);
                    else
                        goTo.get(i).put(symbol, j);
                }

                // REDUCE
                else {
                    if (item.charAt(0) == augmentedStart) {
                        action.get(i).put("$", "ACC");
                    } else {
                        String prod = item.replace(".", "");
                        for (String t : terminals)
                            action.get(i).put(t, "R(" + prod + ")");
                    }
                }
            }
        }
    }

    // ================= PRINT =================

    static void printParsingTable() {

        System.out.println("\nLR(0) PARSING TABLE:");
        System.out.print("State\t");

        for (String t : terminals)
            System.out.print(t + "\t");

        System.out.println();

        for (int i = 0; i < states.size(); i++) {
            System.out.print(i + "\t");

            for (String t : terminals)
                System.out.print(action.get(i).getOrDefault(t, "-") + "\t");

            System.out.println();
        }
    }

    // ================= PARSER =================

    static void parse(String input) {

        Stack<Integer> stack = new Stack<>();
        stack.push(0);

        int i = 0;

        System.out.println("\nSTACK\tINPUT\tACTION");

        while (true) {

            int state = stack.peek();
            String curr = "" + input.charAt(i);
            String act = action.get(state).get(curr);

            System.out.println(stack + "\t" + input.substring(i) + "\t" + act);

            if (act == null) {
                System.out.println("REJECT");
                return;
            }

            if (act.equals("ACC")) {
                System.out.println("ACCEPT");
                return;
            }

            if (act.startsWith("S")) {
                int next = Integer.parseInt(act.substring(1));
                stack.push(next);
                i++;
            }

            else if (act.startsWith("R")) {
                String prod = act.substring(2, act.length() - 1);
                char lhs = prod.charAt(0);
                String rhs = prod.substring(3);

                if (!rhs.equals("#"))
                    for (int j = 0; j < rhs.length(); j++)
                        stack.pop();

                int top = stack.peek();
                stack.push(goTo.get(top).get(lhs));
            }
        }
    }
}
